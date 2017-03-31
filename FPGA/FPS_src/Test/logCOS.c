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
 * Read change-of-state log messages and forward to IOC
 */
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#include <xil_mmu.h>
#include <xil_io.h>
#include <lwip/udp.h>
#include <xparameters.h>
#include "fastProtectionProtocol.h"
#include "aurora.h"
#include "logCOS.h"
#include "sysmon.h"
#include "util.h"

/*
 * We must enable scatter/gather DMA since each change-of-state log
 * entry arrives as a single packet (TLAST asserted on last word) and
 * we want to buffer multiple entries without requiring processor
 * intervention.
 *
 * There is a one-to-one correspondence between buffer descriptors and
 * change-of-state log entries since we want to be able to forward log
 * entries to the IOC as soon as they arrive without having to wait for
 * a complete multi-entry buffer to fill.
 *
 * This is a very inefficient use of memory since there is a 64-byte buffer
 * descriptor for each 28-byte log entry.  But memory is plentiful.
 */
#define BUFFER_COUNT    1000000

/*
 * DMA buffer descriptor
 */
struct bufDesc {
    uint32_t         next;
    uint32_t         nextMSB;
    uint32_t         buffer;
    uint32_t         bufferMSB;
    uint32_t         rsvd0;
    uint32_t         rsvd1;
    uint32_t         control;
    uint32_t         status;
    /* No user application fields */
} __attribute__((aligned(0x40)));

/*
 * Merge into single structure and place on TLB entry (1 MB) boundary
 * Unfortunately this can't be done with an 'aligned' attribute since
 * the assembler has a limit if 15-bit (32 kB) alignment.
 */
struct logArena {
    struct bufDesc        bd[BUFFER_COUNT];
    fastProtectLogEntry_t lb[BUFFER_COUNT];
};
volatile static struct logArena *logArena;
static uint32_t bufferInterval, retryInterval;


#define BUFFER_DESCRIPTOR_STATUS_COMPLETE   0x80000000
#define BUFFER_DESCRIPTOR_STATUS_COUNT_MASK 0x7FFFFF

#define S2MM_DMACR      (XPAR_LOGDMA_AXI_DMA_0_BASEADDR+0x30)
#define S2MM_DMASR      (XPAR_LOGDMA_AXI_DMA_0_BASEADDR+0x34)
#define S2MM_CURDESC    (XPAR_LOGDMA_AXI_DMA_0_BASEADDR+0x38)
#define S2MM_TAILDESC   (XPAR_LOGDMA_AXI_DMA_0_BASEADDR+0x40)

#define S2MM_DMACR_RS           0x1
#define S2MM_DMACR_Reset        0x4
#define S2MM_DMACR_CYCLIC_BD    0x10
#define S2MM_DMACR_IOC_IrqEn    0x1000
#define S2MM_DMACR_Dly_IrqEn    0x2000
#define S2MM_DMACR_Err_IrqEn    0x4000
#define S2MM_DMACR_IRQThreshold_SHIFT   16
#define S2MM_DMACR_IRQDelay_SHIFT       24

#define S2MM_DMASR_Halted       0x1
#define S2MM_DMASR_Idle         0x2
#define S2MM_DMASR_SGIncld      0x8
#define S2MM_DMASR_DMAIntErr    0x10
#define S2MM_DMASR_DMASlvErr    0x20
#define S2MM_DMASR_DMADecErr    0x40
#define S2MM_DMASR_SGIntErr     0x100
#define S2MM_DMASR_SGSlvErr     0x200
#define S2MM_DMASR_SGDecErr     0x400
#define S2MM_DMASR_IOC_Irq      0x1000
#define S2MM_DMASR_Dly_Irq      0x2000
#define S2MM_DMASR_Err_Irq      0x4000
#define S2MM_DMACR_IRQThresholdSts_SHIFT   16
#define S2MM_DMACR_IRQDelaySts_SHIFT       24

/*
 * Our subscriber
 */
#define RETRYLIMIT  20
struct subscriber {
    struct udp_pcb *pcb;
    struct ip_addr  addr;
    uint16_t        port;
    uint32_t        subscriptionNumber;
    int             awaitACK;
    int             txCount;
};
static struct subscriber subscriber;
struct fastProtectLogPacket txBuf;

/*
 * Handle subscription requests
 */
static void
handlePacket(void *arg, struct udp_pcb *pcb, struct pbuf *p,
             struct ip_addr *fromAddr, uint16_t fromPort)
{
    static union {
        struct fastProtectSubscriptionPacket subscribe;
        struct fastProtectLogAckPacket       logAck;
    } un;

    if (debugFlags & DEBUGFLAGS_LOG) {
        uint32_t addr = ntohl(fromAddr->addr);
        printf("LOG %d from %d.%d.%d.%d:%d\n", p->len,
                                               (int)((addr >> 24) & 0xFF),
                                               (int)((addr >> 16) & 0xFF),
                                               (int)((addr >>  8) & 0xFF),
                                               (int)((addr >>  0) & 0xFF),
                                               fromPort);
    }
    if ((p->len >= sizeof un.subscribe.magic)
     && (p->len <= sizeof un)
     && (fromPort != 0)) {
        memcpy(&un, p->payload, p->len);
        switch (un.subscribe.magic) {
        case FAST_PROTECT_PROTOCOL_MAGIC_SUBSCRIBE_LOG:
            if (p->len == sizeof un.subscribe) {
                if (debugFlags & DEBUGFLAGS_LOG) {
                    printf("Subscribe\n");
                }
                subscriber.addr = *fromAddr;
                subscriber.port = fromPort;
                subscriber.subscriptionNumber = un.subscribe.subscriptionNumber;
                subscriber.awaitACK = 0;
                txBuf.sequenceNumber = 0;
                auroraLogForce();
            }
            break;

        case FAST_PROTECT_PROTOCOL_MAGIC_ACKNOWLEDGE_LOG:
            if ((p->len == sizeof un.logAck)
             && (un.logAck.subscriptionNumber == txBuf.subscriptionNumber)
             && (un.logAck.sequenceNumber == txBuf.sequenceNumber)
             && (subscriber.awaitACK)) {
                if (debugFlags & DEBUGFLAGS_LOG) {
                    printf("ACK %u\n", (unsigned int)un.logAck.sequenceNumber);
                }
                subscriber.awaitACK = 0;
                subscriber.txCount = 0;
            }
            break;
        }
    }
    pbuf_free(p);
}

#define TLB_BOUNDARY (1024 * 1024)
#define TLB_COUNT ((sizeof(struct logArena) + (TLB_BOUNDARY-1)) / TLB_BOUNDARY)

void
logInit(void)
{
    int i;
    volatile struct bufDesc *bd;
    uint32_t arena;
    static char logMem[(TLB_COUNT+1)*TLB_BOUNDARY];

    bufferInterval = ticksPerSecond / 20;
    retryInterval = ticksPerSecond;
    printf("Send partial LOG packet after %u ticks\n", (unsigned int)bufferInterval);

    /* Place arena at required boundary */
    arena = ((uint32_t)logMem + (TLB_BOUNDARY - 1)) & ~(TLB_BOUNDARY - 1);
    logArena = (struct logArena *)arena;
    printf("Arena size:%d %x, base:%p %#x, tlbCount:%u memsize:%u %x\n",
                                                (int)sizeof(struct logArena),
                                                (int)sizeof(struct logArena),
                                                logMem, (int)arena, TLB_COUNT,
                                                (int)sizeof(logMem),
                                                (int)sizeof(logMem));

    /* Sanity check */
    if (!(Xil_In32(S2MM_DMASR) & S2MM_DMASR_SGIncld))
        fatal("LOG DMA has no scatter/gather");
    if (arena & ((1024*1024)-1))
        fatal("Log arena %u not on required boundary", (unsigned int)arena);
    if ((uint32_t)&logArena->bd[1] & (0x40-1)) {
        fatal("Log arena buffer descriptors %u not on required boundary",
                                                (unsigned int)&logArena->bd[1]);
    }

    /* Make arena non-cacheable */
    for (i = 0 ; i < TLB_COUNT ; i++, arena += TLB_BOUNDARY) {
        Xil_SetTlbAttributes(arena, NORM_NONCACHE);
    }

    /* Ensure that everything is in the startup state */
    Xil_Out32(S2MM_DMACR, S2MM_DMACR_Reset);
    for (i = 0 ; ; i++) {
        uint32_t cr;
        microsecondSpin(1);
        cr = Xil_In32(S2MM_DMACR);
        if ((cr & (S2MM_DMACR_Reset | S2MM_DMACR_RS)) == 0) break;
        if (i > 100) fatal("Can't reset LOG DMA -- S2MM_CR:%"PRIX32, cr);
    }

    /* Create ring of buffer descriptors */
    for (i = 0, bd = logArena->bd; i < BUFFER_COUNT ; i++, bd++) {
        bd->next = (uint32_t)&logArena->bd[(i+1)%BUFFER_COUNT];
        bd->buffer = (uint32_t)&logArena->lb[i];
        bd->control = sizeof logArena->lb[i];
        bd->status = 0; /* Hand ownership to DMA engine */
    }

    /* Set up registers */
    Xil_Out32(S2MM_CURDESC, (uint32_t)logArena->bd);
    Xil_Out32(S2MM_DMACR, S2MM_DMACR_CYCLIC_BD | S2MM_DMACR_RS);

    /* Start continuous DMA -- set tail descriptor to unreachable value */
    Xil_Out32(S2MM_TAILDESC, (uint32_t)(logArena->bd+BUFFER_COUNT));

    /* Set up UDP publisher */
    subscriber.pcb = udp_new();
    if (subscriber.pcb == NULL)
        fatal("Can't create publisher PCB");
    i = udp_bind(subscriber.pcb, IP_ADDR_ANY,
                                   FAST_PROTECT_PROTOCOL_PUBLISHER_UDP_PORT);
   if (i != ERR_OK) fatal("Can't bind to publisher port, error:%d", i);
   udp_recv(subscriber.pcb, handlePacket, NULL);
}

/*
 * Statistics
 */
static unsigned int totalRetries;
unsigned int
logRetryCount(void)
{
    return totalRetries;
}

/*
 * Copy filled entries to packet then send
 */
void
logCrank(void)
{
    uint32_t ticksNow = ticksSinceBoot();
    static uint32_t ticksWhenSent;
    static uint32_t ticksWhenStarted;
    static int retryCount;
    static int dmaIndex;

    
    if (subscriber.awaitACK) {
        if (((ticksWhenSent == 0)
         || ((ticksNow - ticksWhenSent) >= retryInterval))) {
            int l;
            struct pbuf *p;

            if (ticksWhenSent) {
                totalRetries++;
                if (++retryCount == RETRYLIMIT) {
                    if (debugFlags & DEBUGFLAGS_LOG) printf("RETRY LIMIT!\n");
                    subscriber.port = 0;
                    subscriber.awaitACK = 0;
                    subscriber.txCount = 0;
                    return;
                }
            }
            l = FAST_PROTECT_PROTOCOL_LOG_COUNT_TO_LENGTH(subscriber.txCount);
            p = pbuf_alloc(PBUF_TRANSPORT, l, PBUF_RAM);
            if (p) {
                int err;
                if (debugFlags & DEBUGFLAGS_LOG) printf("TX %d %u\n",
                                            subscriber.txCount,
                                            (unsigned int)txBuf.sequenceNumber);
                memcpy(p->payload, &txBuf, l);
                err = udp_sendto(subscriber.pcb, p, &subscriber.addr,
                                                               subscriber.port);
                pbuf_free(p);
                if (err != ERR_OK) {
                    printf("Can't publish LOG, err:%d\n", (int)err);
                }
            }
            else {
                printf("Can't allocate LOG pbuf\n");
            }
            ticksWhenSent = ticksNow;
        }
        return;
    }

    /*
     * Copy filled DMA buffers to transmit buffer
     */
    while (logArena->bd[dmaIndex].status & BUFFER_DESCRIPTOR_STATUS_COMPLETE) {
        volatile fastProtectLogEntry_t *src = &logArena->lb[dmaIndex];
        fastProtectLogEntry_t *dst = &txBuf.log[subscriber.txCount];
        *dst = *src;
        if (debugFlags & DEBUGFLAGS_LOG) {
            if (dst->status == (uint32_t)~0UL) {
                printf("  Marker T:%u:%09u", (unsigned int)dst->seconds,
                                             (unsigned int)dst->ticks);
            }
            else {
                printf("S:%06X T:%u:%09u", (unsigned int)dst->status,
                                           (unsigned int)dst->seconds,
                                           (unsigned int)dst->ticks);
                if (dst->status &
                    FAST_PROTECT_PROTOCOL_LOG_ENTRY_STATUS_VALID_ACQINFO_MASK) {
                    printf(" i:%08X %08X t:%08X %08X",
                                         (unsigned int)dst->inputStateHi,
                                         (unsigned int)dst->inputStateLo,
                                         (unsigned int)dst->inputTransitionsHi,
                                         (unsigned int)dst->inputTransitionsLo);
                }
            }
            printf("\n");
        }
        if (ticksWhenStarted == 0) ticksWhenStarted = ticksSinceBoot();
        dmaIndex = (dmaIndex == (BUFFER_COUNT - 1)) ? 0 : dmaIndex + 1;
        if (++subscriber.txCount == FAST_PROTECT_PROTOCOL_LOG_CAPACITY) {
            break;
        }
    }

    /*
     * Enable transmission when transmit buffer full or when transmit buffer
     * partially full and buffering time limit has been reached.
     */
    if ((subscriber.txCount == FAST_PROTECT_PROTOCOL_LOG_CAPACITY)
     || (ticksWhenStarted
      && ((uint32_t)(ticksSinceBoot() - ticksWhenStarted) >= bufferInterval))) {
        ticksWhenSent = 0;
        ticksWhenStarted = 0;
        if (subscriber.port) {
            retryCount = 0;
            subscriber.awaitACK = 1;
            txBuf.magic = FAST_PROTECT_PROTOCOL_MAGIC_PUBLISH_LOG;
            txBuf.subscriptionNumber = subscriber.subscriptionNumber;
            txBuf.sequenceNumber++;
        }
        else {
            subscriber.txCount = 0;
        }
    }
}
