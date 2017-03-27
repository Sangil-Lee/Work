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
#include <inttypes.h>
#include <string.h>
#include <xparameters.h>
#include <xuartps_hw.h>
#include "aurora.h"
#include "console.h"
#include "eyescan.h"
#include "sysmon.h"
#include "util.h"

#define LOGBUF_SIZE 4000
static char logBuf[LOGBUF_SIZE];
static int logging = 1, logIndex = 0;

/*
 * Provide this version of outbyte so that we can buffer startup messages.
 */
void
outbyte(char c)
{
    if (logging && (c != '\r')) {
        logBuf[logIndex++] = c;
        if (logIndex == LOGBUF_SIZE) logging = 0;
    }
    XUartPs_SendByte(STDOUT_BASEADDRESS, c);
}

static int
cmdMON(int argc, char **argv)
{
    sysmonDump();
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
  { "debug",    cmdDEBUG,       "Set debug flags"                 },
  { "eye",      eyescanCommand, "Perform transceiver eye scan"    },
  { "log",      cmdREPLAY,      "Replay message log"              },
  { "mon",      cmdMON,         "Show system monitor buffer"      },
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
    processCommand(argc, argv);
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
    c =  XUartPs_ReadReg(STDOUT_BASEADDRESS, XUARTPS_FIFO_OFFSET) & 0xFF;
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
