/*
 * MIT License
 *
 * Copyright (c) 2106 Osprey DCS
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <xparameters.h>
#include <xuartps_hw.h>
#include <lwip/def.h>
#include <lwip/stats.h>
#include "aurora.h"
#include "console.h"
#include "evr.h"
#include "eyescan.h"
#include "sysmon.h"
#include "systemParameters.h"
#include "util.h"

#define LOGBUF_SIZE 4000
static char logBuf[LOGBUF_SIZE];
static int logging = 1, logIndex = 0;

/*
 * Console modes
 */
static enum consoleModes {
    cm_command,
    cm_rebootQuery,
    cm_macQuery,
    cm_netQuery } consoleMode = cm_command;

/*
 * Provide this version of outbyte so that we can buffer startup messages.
 */
void
outbyte(char c)
{
    static int wasCR;
    if (logging && (c != '\r')) {
        logBuf[logIndex++] = c;
        if (logIndex == LOGBUF_SIZE) logging = 0;
    }
    if ((c == '\n') && !wasCR) outbyte('\r');
    wasCR = (c == '\r');
    XUartPs_SendByte(STDOUT_BASEADDRESS, c);
}

static int
cmdMON(int argc, char **argv)
{
    char *endp;
    int bad = 0;
    int link = -1, node = 0;

    if (argc > 2) {
        node = strtol(argv[2], &endp, 0);
        if (*endp != '\0') bad = 1;
    }
    if (argc > 1) {
        link = strtol(argv[1], &endp, 0);
        if (*endp != '\0') bad = 1;
    }
    if (bad) {
        printf("Usage %s [link [node]]\n", argv[0]);
        return -1;
    }
    sysmonDump(link, node);
    return 0;
}

static int
cmdAURORA(int argc, char **argv)
{
    auroraShowStatus();
    return 0;
}

static int
cmdDEBUG(int argc, char **argv)
{
    char *endp;
    int d;

    if (argc > 1) {
        d = strtol(argv[1], &endp, 16);
        if (*endp == '\0') {
            debugFlags = d;
        }
    }
    printf("Debug flags: %x\n", debugFlags);
    if (debugFlags & DEBUGFLAGS_LOG_FORCE) auroraLogForce();
    return 0;
}

static int
cmdEVR(int argc, char **argv)
{
    evrShowStatus();
    return 0;
}

static int
cmdREPLAY(int argc, char **argv)
{
    int i;
    int restart = 0;

    logging = 0;
    if ((argc == 2) && (strcasecmp(argv[1], "-r") == 0)) {
        printf("Logging rearmed\n");
        restart = 1;
    }
    else if (argc != 1) {
        printf("Usage: log [-r]\n");
        return 1;
    }
    for (i = 0 ; i < logIndex ; i++) {
        char c = logBuf[i];
        if (c == '\n') outbyte('\r');
        outbyte(c);
    }
    if (restart) {
        logIndex = 0;
        logging = 1;
    }
    return 0;
}

static int
cmdSTATS(int argc, char **argv)
{
    stats_display();
    return 0;
}

/*
 * Reboot command requires modal callback
 */
static void
rebootQueryCallback(int argc, char **argv)
{
    if (argc == 1) {
        if (strcasecmp(argv[0], "Y") == 0) {
            microsecondSpin(5000);
            // Unlock SLCR
            Xil_Out32(XPS_SYS_CTRL_BASEADDR + 0x008, 0xDF0D);
            // Force system reset
            Xil_Out32(XPS_SYS_CTRL_BASEADDR + 0x200, 0x1);
            /* NOTREACHED */
        }
        if (strcasecmp(argv[0], "N") == 0) {
            consoleMode = cm_command;
            return;
        }
    }
    printf("Reboot FPGA (y or n)? ");
    fflush(stdout);
}
static int
cmdREBOOT(int argc, char **argv)
{
    char *cp = NULL;

    consoleMode = cm_rebootQuery;
    rebootQueryCallback(0, &cp);
    return 0;
}

/*
 * Network configuration
 */
static struct sysNetParms ipv4;
static void
netQueryCallback(int argc, char **argv)
{
    if (argc == 1) {
        if (strcasecmp(argv[0], "Y") == 0) {
            systemParameters.ipv4 = ipv4;
            systemParametersStash();
            consoleMode = cm_command;
            return;
        }
        if (strcasecmp(argv[0], "N") == 0) {
            consoleMode = cm_command;
            return;
        }
    }
    printf("    IP ADDRESS: %s\n", formatIP(&ipv4.address));
    printf("      NET MASK: %s\n", formatIP(&ipv4.netmask));
    printf("       GATEWAY: %s\n", formatIP(&ipv4.gateway));
    if ((consoleMode == cm_netQuery)
     || (ipv4.address != systemParameters.ipv4.address)
     || (ipv4.netmask != systemParameters.ipv4.netmask)
     || (ipv4.gateway != systemParameters.ipv4.gateway)) {
        printf("Write parameters to flash (y or n)? ");
        fflush(stdout);
        consoleMode = cm_netQuery;
    }
}
static int
cmdNET(int argc, char **argv)
{
    int bad = 0;
    int i;
    char *cp;
    int netLen = 24;
    char *endp;

    if (argc == 1) {
        ipv4 = systemParameters.ipv4;
    }
    else if (argc == 2) {
        cp = argv[1];
        i = parseIP(cp, &ipv4.address);
        if (i < 0) {
            bad = 1;
        }
        else if (cp[i] == '/') {
            netLen = strtol(cp + i + 1, &endp, 0);
            if ((*endp != '\0')
             || (netLen < 8)
             || (netLen > 24)) {
                bad = 1;
            }
        }
        ipv4.netmask = ~0 << (32 - netLen);
        ipv4.gateway = (ntohl(ipv4.address) & ipv4.netmask) | 1;
        ipv4.netmask = htonl(ipv4.netmask);
        ipv4.gateway = htonl(ipv4.gateway);
    }
    else {
        bad = 1;
    }
    if (bad) {
        printf("Command takes single optional argument of the form "
               "www.xxx.yyy.xxx[/n]\n");
        return 1;
    }
    cp = NULL;
    netQueryCallback(0, &cp);
    return 0;
}

/*
 * MAC configuration
 */
static unsigned char macBuf[6];
static void
macQueryCallback(int argc, char **argv)
{
    if (argc == 1) {
        if (strcasecmp(argv[0], "Y") == 0) {
            memcpy(systemParameters.ethernetAddress, macBuf, sizeof macBuf);
            systemParametersStash();
            consoleMode = cm_command;
            return;
        }
        if (strcasecmp(argv[0], "N") == 0) {
            consoleMode = cm_command;
            return;
        }
    }
    printf("   ETHERNET ADDRESS: %s\n", formatMAC(&macBuf));
    if ((consoleMode == cm_macQuery)
     || (memcmp(systemParameters.ethernetAddress, macBuf, sizeof macBuf))) {
        printf("Write parameters to flash (y or n)? ");
        fflush(stdout);
        consoleMode = cm_macQuery;
    }
}
static int
cmdMAC(int argc, char **argv)
{
    int bad = 0;
    int i;
    char *cp;

    if (argc == 1) {
        memcpy(macBuf, systemParameters.ethernetAddress,
                                    sizeof systemParameters.ethernetAddress);
    }
    else if (argc == 2) {
        i = parseMAC(argv[1], macBuf);
        if ((i < 0) || (argv[1][i] != '\0')) {
            bad = 1;
        }
    }
    else {
        bad = 1;
    }
    if (bad) {
        printf("Command takes single optional argument of the form "
               "aa:bb:cc:dd:ee:ff\n");
        return 1;
    }
    cp = NULL;
    macQueryCallback(0, &cp);
    return 0;
}

/*
 * List of commands
 */
struct commandInfo {
    const char *name;
    int       (*handler)(int argc, char **argv);
    const char *description;
};
static struct commandInfo commandTable[] = {
  { "aurora",   cmdAURORA,      "Show Aurora link status"         },
  { "buf",      cmdMON,         "Show system monitor buffer"      },
  { "debug",    cmdDEBUG,       "Set debug flags"                 },
  { "evr",      cmdEVR,         "Show EVR link status"            },
  { "eye",      eyescanCommand, "Perform transceiver eye scan"    },
  { "log",      cmdREPLAY,      "Replay message log"              },
  { "mac",      cmdMAC,         "Set ethernet MAC address"        },
  { "net",      cmdNET,         "Set network parameters"          },
  { "reboot",   cmdREBOOT,      "Reboot system"                   },
  { "stats",    cmdSTATS,       "Show network statistics"         },
};

/*
 * Process a command
 */
static void
processCommand(int argc, char **argv)
{
    int i;
    int len;
    int matched = -1;

    if (argc <= 0)
        return;
    len = strlen(argv[0]);
    for (i = 0 ; i < sizeof commandTable / sizeof commandTable[0] ; i++) {
        if (strncasecmp(argv[0], commandTable[i].name, len) == 0) {
            if (matched >= 0) {
                printf("Not unique.\n");
                return;
            }
            matched = i;
        }
    }
    if (matched >= 0) {
        (*commandTable[matched].handler)(argc, argv);
        return;
    }
    if ((strncasecmp(argv[0], "help", len) == 0) || (argv[0][0] == '?')) {
        printf("Commands:\n");
        for (i = 0 ; i < sizeof commandTable / sizeof commandTable[0] ; i++) {
            printf("%8s -- %s\n", commandTable[i].name,
                                  commandTable[i].description);
        }
    }
    else {
        printf("Invalid command\n");
    }
}

/*
 * Process a line of input.
 * Silently toss lines with too many words.
 */
static void
processLine(char *buf)
{
    int argc = 0;
    char *argv[10];
    char *last;

    for (;;) {
        argv[argc] = strtok_r(buf, " ", &last);
        if (argv[argc] == NULL) break;
        argc++;
        if (argc == (sizeof argv / sizeof argv[0])) return;
        buf = NULL;
    }
    switch (consoleMode) {
    case cm_rebootQuery: rebootQueryCallback(argc, argv);        break;
    case cm_netQuery:    netQueryCallback(argc, argv);           break;
    case cm_macQuery:    macQueryCallback(argc, argv);           break;
    default:             processCommand(argc, argv);
    }
}

/*
 * Check for and handle character from console.
 * Force characters to canonical form.
 */
void
consoleCheck(void)
{
    int c;
    static char lbuf[100];
    static int idx = 0;

    if (eyescanCrank()) return;
    if (!XUartPs_IsReceiveData(STDOUT_BASEADDRESS)) return;
    c = XUartPs_ReadReg(STDOUT_BASEADDRESS, XUARTPS_FIFO_OFFSET) & 0xFF;
    if (c > '\177') return;
    if (c == '\177') c = '\b';
    else if (c == '\t') c = ' ';
    else if (c == '\r') c = '\n';
    switch (c) {
    case '\n':
        logging = 0;
        outbyte('\r');
        outbyte('\n');
        lbuf[idx] = '\0';
        idx = 0;
        processLine(lbuf);
        break;

    case '\b':
        if (idx) {
            outbyte('\b');
            outbyte(' ');
            outbyte('\b');
            idx--;
        }
        break;
        
    default:
        if ((c >= ' ') && (idx < ((sizeof lbuf) - 1))) {
            outbyte(c);
            lbuf[idx++] = c;
        }
        break;
    }
}
