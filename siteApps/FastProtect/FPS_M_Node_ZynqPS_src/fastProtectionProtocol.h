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
 * FPGA/IOC communication
 * All structure entries are uint32_t to make it easy to implement
 * endian swapping, if that's ever required.
 */

#ifndef _FAST_PROTECTION_PROTOCOL_H_
#define _FAST_PROTECTION_PROTOCOL_H_

#include <stdint.h>

#define FAST_PROTECT_PROTOCOL_PUBLISHER_UDP_PORT    31784
#define FAST_PROTECT_PROTOCOL_COMMAND_UDP_PORT      31785

#define FAST_PROTECT_PROTOCOL_MAGIC_COMMAND_WRITE_SCALAR    0xCD01
#define FAST_PROTECT_PROTOCOL_MAGIC_COMMAND_READ_SCALAR     0xCD02
#define FAST_PROTECT_PROTOCOL_MAGIC_COMMAND_WRITE_BITMAP    0xCD03
#define FAST_PROTECT_PROTOCOL_MAGIC_COMMAND_READ_BITMAP     0xCD04
#define FAST_PROTECT_PROTOCOL_MAGIC_COMMAND_READ_SYSMON     0xCD06
#define FAST_PROTECT_PROTOCOL_MAGIC_SUBSCRIBE_LOG           0xCD80
#define FAST_PROTECT_PROTOCOL_MAGIC_PUBLISH_LOG             0xCD81
#define FAST_PROTECT_PROTOCOL_MAGIC_ACKNOWLEDGE_LOG         0xCD82
#define FAST_PROTECT_PROTOCOL_ARG_CAPACITY      360
#define FAST_PROTECT_PROTOCOL_LOG_CAPACITY      60

#define FAST_PROTECT_PROTOCOL_MAX_LINKS                 8
#define FAST_PROTECT_PROTOCOL_MAX_NODES_PER_LINK        5
#define FAST_PROTECT_PROTOCOL_SYSMON_CHANNEL_CAPACITY   8

/*
 * Command/Response
 */
struct fastProtectPacket {
    uint32_t    magic;
    uint32_t    sequenceNumber;
    uint32_t    args[FAST_PROTECT_PROTOCOL_ARG_CAPACITY];
};
#define FAST_PROTECT_PROTOCOL_LENGTH_TO_COUNT(l)                             \
                              (FAST_PROTECT_PROTOCOL_ARG_CAPACITY -          \
                              ((sizeof(struct fastProtectPacket) - (l)) /    \
                                       sizeof(uint32_t)))
#define FAST_PROTECT_PROTOCOL_COUNT_TO_LENGTH(c)                             \
                              (sizeof(struct fastProtectPacket) -            \
                              ((FAST_PROTECT_PROTOCOL_ARG_CAPACITY - (c)) *  \
                                       sizeof(uint32_t)))

/*
 * Subscribe to change-of-state logging
 */
struct fastProtectSubscriptionPacket {
    uint32_t    magic;
    uint32_t    subscriptionNumber;
};

/*
 * Change-of-state logging (published packets)
 */
#define FAST_PROTECT_PROTOCOL_LOG_ENTRY_STATUS_OUTPUTS_MASK       0x0000FFFF
#define FAST_PROTECT_PROTOCOL_LOG_ENTRY_STATUS_ACQINDEX_MASK      0x003F0000
#define FAST_PROTECT_PROTOCOL_LOG_ENTRY_STATUS_ACQINDEX_SHIFT     16
#define FAST_PROTECT_PROTOCOL_LOG_ENTRY_STATUS_VALID_ACQINFO_MASK 0x00400000
typedef struct { /* Single log entry */
    uint32_t    status;
    uint32_t    seconds;
    uint32_t    ticks;
    uint32_t    inputStateHi;
    uint32_t    inputStateLo;
    uint32_t    inputTransitionsHi;
    uint32_t    inputTransitionsLo;
} fastProtectLogEntry_t;

struct fastProtectLogPacket {
    uint32_t              magic;
    uint32_t              subscriptionNumber;
    uint32_t              sequenceNumber;
    fastProtectLogEntry_t log[FAST_PROTECT_PROTOCOL_LOG_CAPACITY];
};
#define FAST_PROTECT_PROTOCOL_LOG_LENGTH_TO_COUNT(l)                         \
                              (FAST_PROTECT_PROTOCOL_LOG_CAPACITY -          \
                              ((sizeof(struct fastProtectLogPacket) - (l)) / \
                                       sizeof(fastProtectLogEntry_t)))
#define FAST_PROTECT_PROTOCOL_LOG_COUNT_TO_LENGTH(c)                         \
                              (sizeof(struct fastProtectLogPacket) -         \
                              ((FAST_PROTECT_PROTOCOL_LOG_CAPACITY - (c)) *  \
                                       sizeof(fastProtectLogEntry_t)))

struct fastProtectLogAckPacket {
    uint32_t magic;
    uint32_t subscriptionNumber;
    uint32_t sequenceNumber;
};

/*
 * Command codes
 */
#define FAST_PROTECT_PROTOCOL_CMD_ENABLE_OUTPUT     0x2

#endif /* _FAST_PROTECTION_PROTOCOL_H_ */
