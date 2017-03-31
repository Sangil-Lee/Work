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

/*
 * Serve EPICS IOC requests
 */
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#include <xil_io.h>
#include <lwip/udp.h>
#include <xparameters.h>
#include "fastProtectionProtocol.h"
#include "epics.h"
#include "sysmon.h"
#include "util.h"

/*
 * Number of uint32_t values in a bitmap
 */
#define BITMAP_INT32_COUNT (FAST_PROTECT_PROTOCOL_MAX_LINKS *           \
                            FAST_PROTECT_PROTOCOL_MAX_NODES_PER_LINK * 2)

/*
 * Number of actual RAM entries per bitmap
 */
#define BITMAP_INT32_SIZE (CPOW2(FAST_PROTECT_PROTOCOL_MAX_LINKS) *           \
                           CPOW2(FAST_PROTECT_PROTOCOL_MAX_NODES_PER_LINK) * 2)

/*
 * Read back system monitor values
 */
static unsigned int
readSysmon(struct fastProtectPacket *reply)
{
    int argCount = 0;
    int l, n, c;

    /*
     * Mitigation node monitors
     */
    for (c = 0 ; c < FAST_PROTECT_PROTOCOL_SYSMON_CHANNEL_CAPACITY ; c += 2) { 
        reply->args[argCount++] = (sysmonValue(-1, -1, c+1) << 16) |
                                   sysmonValue(-1, -1, c);
    }

    /*
     * Acquisition node monitors
     */
    for (n = 0 ; n < FAST_PROTECT_PROTOCOL_MAX_NODES_PER_LINK ; n++) {
        for (l = 0 ; l < FAST_PROTECT_PROTOCOL_MAX_LINKS ; l++) {
            for (c=0 ; c<FAST_PROTECT_PROTOCOL_SYSMON_CHANNEL_CAPACITY ; c+=2) { 
                reply->args[argCount++] = (sysmonValue(l, n, c+1) << 16) |
                                           sysmonValue(l, n, c);
            }
        }
    }

    /*
     * Mitigation EVR SFP
     */
    reply->args[argCount++] = (sysmonSFPtemperature(-1, -1, 0) << 16) |
                               sysmonSFPrxPower(-1, -1, 0);
    /*
     * Mitigation link SFPs
     */
    for (l = 0 ; l < FAST_PROTECT_PROTOCOL_MAX_LINKS ; l++) {
        reply->args[argCount++] = (sysmonSFPtemperature(l, -1, 1) << 16) |
                                   sysmonSFPrxPower(l, -1, 1);
    }

    /*
     * Acquisition node SFPs
     */
    for (n = 0 ; n < FAST_PROTECT_PROTOCOL_MAX_NODES_PER_LINK ; n++) {
        for (l = 0 ; l < FAST_PROTECT_PROTOCOL_MAX_LINKS ; l++) {
            reply->args[argCount++] = (sysmonSFPtemperature(l, n, 1) << 16) |
                                       sysmonSFPrxPower(l, n, 1);
        }
        if (n < (FAST_PROTECT_PROTOCOL_MAX_NODES_PER_LINK - 1)) {
            for (l = 0 ; l < FAST_PROTECT_PROTOCOL_MAX_LINKS ; l++) {
                reply->args[argCount++] = (sysmonSFPtemperature(l, n, 0) << 16)|
                                           sysmonSFPrxPower(l, n, 0);
            }
        }
    }

    /*
     * Stale node status
     */
    reply->args[argCount++] = Xil_In32(XPAR_GPIO_STALE_NODES_BASEADDR+0);
    reply->args[argCount++] = Xil_In32(XPAR_GPIO_STALE_NODES_BASEADDR+8);

    return FAST_PROTECT_PROTOCOL_COUNT_TO_LENGTH(argCount);
}

/*
 * Process a command
 */
static unsigned int
processCommand(struct fastProtectPacket *cmd, unsigned int cmdSize,
              struct fastProtectPacket *reply)
{
    if (debugFlags & DEBUGFLAGS_CMD) {
        printf("  CMD %X, %d\n", (unsigned int)cmd->magic,
                    (int)FAST_PROTECT_PROTOCOL_LOG_LENGTH_TO_COUNT(cmdSize));
    }
    switch(cmd->magic) {
    case FAST_PROTECT_PROTOCOL_MAGIC_COMMAND_WRITE_SCALAR:
        if (cmdSize == FAST_PROTECT_PROTOCOL_COUNT_TO_LENGTH(2)) {
            switch (cmd->args[0] & 0xFFFF) {
            case 0:
                {
                static uint32_t mitigationSetToggle;
                mitigationSetToggle ^= cmd->args[1];
                Xil_Out32(XPAR_GPIO_MITIGATION_BASEADDR, mitigationSetToggle);
                if (debugFlags & DEBUGFLAGS_MITIGATE)
                    printf("Set ouputs %X -- toggle now %X\n",
                                             (unsigned int)cmd->args[1],
                                             (unsigned int)mitigationSetToggle);
                }
                return FAST_PROTECT_PROTOCOL_COUNT_TO_LENGTH(0);
            }
        }
        break;

    case FAST_PROTECT_PROTOCOL_MAGIC_COMMAND_WRITE_BITMAP:
        if (cmdSize ==
                FAST_PROTECT_PROTOCOL_COUNT_TO_LENGTH(1+BITMAP_INT32_COUNT)) {
            int i;
            int output = cmd->args[0] & 0xFF;
            int bitmapSelect = (cmd->args[0] >> 8) & 0x1;
            volatile uint32_t *dest;

            if (debugFlags & DEBUGFLAGS_MITIGATE)
                    printf("Mitigate bitmap %d:%d\n", output, bitmapSelect);
            dest = (uint32_t *)XPAR_MITIGATIONBRAMCTRL_S_AXI_BASEADDR +
                             ((output << 1) | bitmapSelect) * BITMAP_INT32_SIZE;
            for (i = 0 ; i < BITMAP_INT32_COUNT ; i++, dest++) {
                *dest = cmd->args[i+1];
                if (debugFlags & DEBUGFLAGS_MITIGATE)
                    printf("%d %x\n", i, (unsigned int)cmd->args[i+1]);
            }
            return FAST_PROTECT_PROTOCOL_COUNT_TO_LENGTH(0);
        }
        break;

    case FAST_PROTECT_PROTOCOL_MAGIC_COMMAND_READ_SYSMON:
        return readSysmon(reply);
    }
    return 0;
}

/*
 * LWIP callback for EPICS command UDP port
 */
static void
handlePacket(void *arg, struct udp_pcb *pcb, struct pbuf *p,
             struct ip_addr *fromAddr, uint16_t fromPort)
{
    unsigned int replySize = 0;
    static unsigned int oldReplySize;
    static struct fastProtectPacket command, reply;

    if (debugFlags & DEBUGFLAGS_CMD) {
        uint32_t addr = ntohl(fromAddr->addr);
        printf("EPICS %d from %d.%d.%d.%d:%d\n", p->len,
                                                 (int)((addr >> 24) & 0xFF),
                                                 (int)((addr >> 16) & 0xFF),
                                                 (int)((addr >>  8) & 0xFF),
                                                 (int)((addr >>  0) & 0xFF),
                                                 fromPort);
    }
    if (p->len <= sizeof command) {
        /*
         * Must copy to ensure proper alignment
         */
        memcpy(&command, p->payload, p->len);
        if ((command.magic == reply.magic)
         && (command.sequenceNumber == reply.sequenceNumber)) {
            replySize = oldReplySize;
        }
        else {
            replySize = processCommand(&command, p->len, &reply);
            reply.magic = command.magic;
            reply.sequenceNumber = command.sequenceNumber;
        }
    }
    pbuf_free(p);
    if (replySize) {
        struct pbuf *r = pbuf_alloc(PBUF_TRANSPORT, replySize, PBUF_RAM);
        if (r) {
            int err;
            memcpy(r->payload, &reply, replySize);
            err = udp_sendto(pcb, r, fromAddr, fromPort);
            if (err != ERR_OK) printf("Can't reply, err:%d\n", (int)err);
            pbuf_free(r);
        }
        oldReplySize = replySize;
    }
}

void
epicsInit(void)
{
    struct udp_pcb *pcb;
    int i;

    /* Set up UDP server */
    pcb = udp_new();
    if (pcb == NULL)
        fatal("Can't create publisher PCB");
    i = udp_bind(pcb, IP_ADDR_ANY, FAST_PROTECT_PROTOCOL_COMMAND_UDP_PORT);
   if (i != ERR_OK) fatal("Can't bind to EPICS server port, error:%d", i);
   udp_recv(pcb, handlePacket, NULL);
}
