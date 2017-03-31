/*
 * Very simple TFTP server.
 * Any filename of the form BOOT*.bin is accepted and transfers
 * to/from the beginning of bootstrap flash memory.
 */
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <lwip/udp.h>
#include <xparameters.h>
#include "flash.h"
#include "util.h"

#define TFTP_PORT 69

#define TFTP_OPCODE_RRQ   1
#define TFTP_OPCODE_WRQ   2
#define TFTP_OPCODE_DATA  3
#define TFTP_OPCODE_ACK   4
#define TFTP_OPCODE_ERROR 5

#define TFTP_ERROR_ACCESS_VIOLATION 2
#define TFTP_PAYLOAD_CAPACITY   512
#define TFTP_PACKET_CAPACITY    ((2 * sizeof (uint16_t)) + TFTP_PAYLOAD_CAPACITY)

#define TRANSFER_TIMEOUT_SECONDS 30 /* Assume client is gone after this long */

#define FLASH_FILE_SIZE 20*1024*1024    /* Maximum write, all read */

/*
 * Time interval computation for diagnostics
 */
static uint32_t
timeInterval(void)
{
    uint32_t now = ticksSinceBoot();
    static uint32_t then;
    uint32_t diff = now - then;
    then = now;
    return diff;
}

/*
 * Send an error reply
 */
static void
replyERR(struct udp_pcb *pcb, const char *msg, struct ip_addr *addr, int port)
{
    int l = (2 * sizeof (uint16_t)) + strlen(msg) + 1;
    struct pbuf *p;
    uint16_t *p16;

    p = pbuf_alloc(PBUF_TRANSPORT, l, PBUF_RAM);
    if (p == NULL) {
        printf("Can't allocate TFTP ERROR pbuf\n");
        return;
    }
    if (debugFlags & DEBUGFLAGS_TFTP)
        printf("TFTP %10u ERROR \"%s\"\n", (unsigned int)timeInterval(), msg);
    p16 = (uint16_t *)p->payload;
    *p16++ = htons(TFTP_OPCODE_ERROR);
    *p16++ = htons(TFTP_ERROR_ACCESS_VIOLATION);
    strcpy((char *)p16, msg);
    udp_sendto(pcb, p, addr, port);
    pbuf_free(p);
}

/*
 * Send a success reply
 */
static void
replyACK(struct udp_pcb *pcb, int block, struct ip_addr *addr, int port)
{
    int l = 2 * sizeof (uint16_t);
    struct pbuf *p;
    uint16_t *p16;

    p = pbuf_alloc(PBUF_TRANSPORT, l, PBUF_RAM);
    if (p == NULL) {
        printf("Can't allocte TFTP ACK pbuf\n");
        return;
    }
    p16 = (uint16_t *)p->payload;
    *p16++ = htons(TFTP_OPCODE_ACK);
    *p16++ = htons(block);
    if (debugFlags & DEBUGFLAGS_TFTP)
        printf("TFTP %10u ACK block %d\n", (unsigned int)timeInterval(), block);
    udp_sendto(pcb, p, addr, port);
    pbuf_free(p);
}

/*
 * Send a data packet
 */
static int
replyDATA(struct udp_pcb *pcb, int block, int offset, int bytesLeft, struct ip_addr *addr, int port)
{
    int l, nSend;
    struct pbuf *p;
    uint16_t *p16;

    nSend = bytesLeft;
    if (nSend > TFTP_PAYLOAD_CAPACITY)
        nSend = TFTP_PAYLOAD_CAPACITY;
    l = (2 * sizeof(uint16_t)) + nSend;
    p = pbuf_alloc(PBUF_TRANSPORT, l, PBUF_RAM);
    if (p == NULL) {
        printf("Can't allocte TFTP DATA pbuf\n");
        return 0;
    }
    p16 = (uint16_t*)p->payload;
    *p16++ = htons(TFTP_OPCODE_DATA);
    *p16++ = htons(block);
    if (nSend) {
        if (flashRead(offset, (char *)p16, nSend) != nSend) {
            pbuf_free(p);
            replyERR(pcb, "FLASH READ FAILURE", addr, port);
            return 0;
        }
    }
    if (debugFlags & DEBUGFLAGS_TFTP)
        printf("TFTP %10u send %d (block %d) from %d\n",
                            (unsigned int)timeInterval(), nSend, block, offset);
    udp_sendto(pcb, p, addr, port);
    pbuf_free(p);
    return nSend;
}

static void
handlePacket(void *arg, struct udp_pcb *pcb, struct pbuf *p,
             struct ip_addr *fromAddr, uint16_t fromPort)
{
    unsigned char *cp = p->payload;
    int opcode = (cp[0] << 8) | cp[1];
    static int flashOffset;
    static int bytesLeft;
    static uint16_t previousBlockNumber;
    static int previousByteCount;
    static unsigned int secondsAtPreviousSend;
    static int transferInProgress;

    if (debugFlags & DEBUGFLAGS_TFTP) {
        uint32_t addr = ntohl(fromAddr->addr);
        printf("TFTP %10u size %d from %d.%d.%d.%d:%d  OP:%04x\n",
                                                   (unsigned int)timeInterval(),
                                                   p->len,
                                                   (int)((addr >> 24) & 0xFF),
                                                   (int)((addr >> 16) & 0xFF),
                                                   (int)((addr >>  8) & 0xFF),
                                                   (int)((addr      ) & 0xFF),
                                                   fromPort, opcode);
    }
    if ((opcode == TFTP_OPCODE_RRQ) || (opcode == TFTP_OPCODE_WRQ)) {
        char *name = (char *)cp + 2, *mode = NULL;
        int nullCount = 0, i = 2;
        previousBlockNumber = 0;
        if (transferInProgress
         && (((unsigned int)secondsSinceBoot() - secondsAtPreviousSend)
                                                  < TRANSFER_TIMEOUT_SECONDS)) {
            replyERR(pcb, "Busy", fromAddr, fromPort);
            return;
        }
        transferInProgress = 0;
        for (;;) {
            if (i >= p->len) {
                replyERR(pcb, "TFTP request too short", fromAddr, fromPort);
                return;
            }
            if (cp[i++] == '\0') {
                nullCount++;
                if (nullCount == 1)
                    mode = (char *)cp + i;
                if (nullCount == 2) {
                    int l;
                    if (debugFlags & DEBUGFLAGS_TFTP)
                        printf("NAME:%s  MODE:%s\n", name, mode);
                    if (strcasecmp(mode, "octet") != 0) {
                        replyERR(pcb, "Bad Type", fromAddr, fromPort);
                        return;
                    }
                    if (((l = strlen(name)) >= 8)
                     && (strncasecmp(name, "BOOT", 4) == 0)
                     && (strncasecmp(name+l-4, ".bin", 4) == 0)) {
                        transferInProgress = 1;
                    }
                    break;
                }
            }
        }
        if (!transferInProgress) {
            replyERR(pcb, "Bad Name", fromAddr, fromPort);
            return;
        }
        flashOffset = FLASH_OFFSET_BOOT_IMAGE;
        if (opcode == TFTP_OPCODE_RRQ) {
            bytesLeft = FLASH_FILE_SIZE;
            previousBlockNumber = 1;
            previousByteCount = replyDATA(pcb, previousBlockNumber, flashOffset,
                                                bytesLeft, fromAddr, fromPort);
            return;
        }
        replyACK(pcb, 0, fromAddr, fromPort);
    }
    else if (opcode == TFTP_OPCODE_DATA) {
        int block = (cp[2] << 8) | cp[3];
        uint16_t nextBlock = previousBlockNumber + 1;
        if (transferInProgress && (block == nextBlock)) {
            int nBytes = p->len - (2 * sizeof(uint16_t));
            previousBlockNumber = block;
            if (nBytes > 0) {
                if ((FLASH_FILE_SIZE - flashOffset) < nBytes) {
                    replyERR(pcb, "File too big", fromAddr, fromPort);
                    transferInProgress = 0;
                    return;
                }
                else {
                    if (flashWrite(flashOffset, (char *)cp+4, nBytes)!=nBytes) {
                        replyERR(pcb, "Write error", fromAddr, fromPort);
                        transferInProgress = 0;
                        return;
                    }
                    flashOffset += nBytes;
                }
            }
            if (nBytes < TFTP_PAYLOAD_CAPACITY) transferInProgress = 0;
        }
        else if (block != previousBlockNumber) {
            return;
        }
        replyACK(pcb, block, fromAddr, fromPort);
        secondsAtPreviousSend = secondsSinceBoot();
    }
    else if (transferInProgress && (opcode == TFTP_OPCODE_ACK)) {
        int block = (cp[2] << 8) | cp[3];
        if (debugFlags & DEBUGFLAGS_TFTP)
            printf("TFTP %10u recv ACK %d\n", (unsigned int)timeInterval(), block);
        if (block == previousBlockNumber) {
            previousBlockNumber++;
            flashOffset += previousByteCount;
            bytesLeft -= previousByteCount;
            if (previousByteCount == TFTP_PAYLOAD_CAPACITY)
                previousByteCount = replyDATA(pcb, previousBlockNumber, flashOffset,
                                                bytesLeft, fromAddr, fromPort);
            else
                transferInProgress = 0;
        }
        else if (block == (uint16_t)(previousBlockNumber - 1)) {
            previousByteCount = replyDATA(pcb, previousBlockNumber, flashOffset,
                                                bytesLeft, fromAddr, fromPort);
        }
        secondsAtPreviousSend = secondsSinceBoot();
    }
}

static void
tftpCallback(void *arg, struct udp_pcb *pcb, struct pbuf *p,
              struct ip_addr *fromAddr, uint16_t fromPort)
{
    /*
     * Processing a TFTP packet can take a long time if it results in a flash
     * erase operation so it would be good to keep polling for work while
     * waiting for the erase to complete.
     * Unfortunately it appears that recursive calls to the network input
     * routine breaks things, so for now we'll just live with blocking network
     * input while the erase is active.
     * Ignore runt packets.
     */
    if (p->len >= (2 * sizeof (uint16_t)))
        handlePacket(arg, pcb, p, fromAddr, fromPort);
    pbuf_free(p);
}

void tftpInit(void)
{
    int err;
    static struct udp_pcb *pcb;

    pcb = udp_new();
    if (pcb == NULL) {
        fatal("Can't create TFTP PCB");
        return;
    }
    err = udp_bind(pcb, IP_ADDR_ANY, TFTP_PORT);
    if (err != ERR_OK) {
        fatal("Can't bind to TFTP port, error:%d", err);
        return;
    }
    udp_recv(pcb, tftpCallback, NULL);
}
