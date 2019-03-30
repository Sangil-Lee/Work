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
 * Set up SFP module
 */
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "iic.h"
#include "iicps.h"
#include "sfp.h"
#include "sysmon.h"
#include "util.h"

#define NSFP 3

#define SFP_INFO_ADDRESS    0x50
#define SFP_STATUS_ADDRESS  0x51

#define INFO_STRING_CHARS   16
#define DATE_STRING_CHARS   6

#define VENDOR_NAME_ADDRESS 20
#define PART_NAME_ADDRESS   40
#define PART_NUMBER_ADDRESS 68
#define DATE_CODE_ADDRESS   84
#define STATUS_TEMPERATURE_ADDRESS 96
#define STATUS_RX_POWER_ADDRESS    104

/*
 * Connect to processor section or programmable logic
 */
static int (*i2cRecv)(int i2cAaddress, int offset, uint8_t *buf, int len);

static const unsigned char *
fetchString(unsigned char *cbuf, int base, int len)
{
    int i;

    if (i2cRecv(SFP_INFO_ADDRESS, base, cbuf, len) == len) {
        for (i = len - 1 ; i >= 0 ; i--) {
            if (cbuf[i] != ' ') break;
        }
        cbuf[i+1] = '\0';
    }
    else {
        memset(cbuf, '\0', len);
    }
    return cbuf;
}

static void
showString(const char *name, int base, int len)
{
    unsigned char cbuf[INFO_STRING_CHARS+1];

    fetchString(cbuf, base, len);
    printf("%16s: %s\n", name, cbuf);
}

static int
getReg(int reg)
{
    unsigned char cbuf[1];
    if (i2cRecv(SFP_STATUS_ADDRESS, reg, cbuf, 1) != 1) {
        return -1;
    }
    return cbuf[0];
}

static int
fetchRxPower(void)
{
    unsigned char cbuf[2];
    if (i2cRecv(SFP_STATUS_ADDRESS, STATUS_RX_POWER_ADDRESS, cbuf, 2) != 2) {
        return 0;
    }
    /* Microwatts */
    return ((cbuf[0] << 8) | cbuf[1]) / 10;
}

static int
fetchTemperature(void)
{
    unsigned char cbuf[2];
    if (i2cRecv(SFP_STATUS_ADDRESS, STATUS_TEMPERATURE_ADDRESS, cbuf, 2) != 2) {
        return 0;
    }
    /* Hundredths of degrees C */
    return (cbuf[0] * 100) | ((cbuf[1] * 100) / 256);
}

static char *
dateCodeToISO8601(char *iso8601, const unsigned char *dateCode)
{
    int i, bad = 0;
    for (i = 0 ; i < 6 ; i++) {
        if (!isprint(dateCode[i])) {
            bad = 1;
            break;
        }
    }
    if (dateCode[6] != '\0') bad = 1;
    if (bad) {
        strcpy(iso8601, "0000-00-00");
    }
    else {
        iso8601[0] = '2';
        iso8601[1] = '0';
        iso8601[2] = dateCode[0];
        iso8601[3] = dateCode[1];
        iso8601[4] = '-';
        iso8601[5] = dateCode[2];
        iso8601[6] = dateCode[3];
        iso8601[7] = '-';
        iso8601[8] = dateCode[4];
        iso8601[9] = dateCode[5];
        iso8601[10] = '\0';
    }
    return iso8601;
}

static void
sfpSelect(int idx)
{
    int s;
    if (idx == 0) {
        s = iicpsSetMux(IICPS_SELECT_SI570);
        i2cRecv = iicpsRecv;
    }
    else {
        s = iicSetMux(idx - 1);
        i2cRecv = iicRecv;
    }
    if (s < 0) fatal("Can't configure I2C MUX");
}
 
void
sfpInit(void)
{
    int sfpIdx, t;
    int r110;
    unsigned char dateCode[DATE_STRING_CHARS+1];
    char iso8601[12];

    for (sfpIdx = 0 ; sfpIdx < NSFP ; sfpIdx++) {
        printf("SFP %d:\n", sfpIdx);
        sfpSelect(sfpIdx);
        showString("Vendor name", VENDOR_NAME_ADDRESS, INFO_STRING_CHARS);
        showString("Part name", PART_NAME_ADDRESS, INFO_STRING_CHARS);
        showString("Part number", PART_NUMBER_ADDRESS, INFO_STRING_CHARS);
        printf("%16s: %s\n", "Date code", dateCodeToISO8601(iso8601,
                 fetchString(dateCode, DATE_CODE_ADDRESS, DATE_STRING_CHARS)));
        if ((r110 = getReg(110)) > 0) {
            if (r110 & 0xC0) printf("   Tx disabled (R110: %02X)\n", r110);
            if ((r110 & 0x30) != 0x30) printf("   Low speed (R110: %02X)\n", r110);
            if (r110 & 0x4) printf("   Tx Fault (R110: %02X)\n", r110);
        }
        printf("  Receiver power: %d uW\n", fetchRxPower());
        t = fetchTemperature();
        printf("     Temperature: %d.%02d C\n", t / 100, t % 100);
    }
}

static int stashOffset;
static void
stashString(int base, int len)
{
    int i;
    unsigned char cbuf[INFO_STRING_CHARS+1];

    fetchString(cbuf, base, len);
    for (i = 0 ; i < len ; i += 2) {
        int v = (cbuf[i] << 8) | cbuf[i+1];
        sysmonStashSFPword(stashOffset, v);
        stashOffset++;
    }
}
static void
stashWord(int v)
{
    sysmonStashSFPword(stashOffset, v);
    stashOffset++;
}

void
sfpCrank(void)
{
    uint32_t now;
    static int sfpIdx, valIdx;
    static uint32_t ticksAtLastUpdate;

    now = ticksSinceBoot();
    if ((now - ticksAtLastUpdate) < ticksPerSecond) return;
    ticksAtLastUpdate = now;
    sfpSelect(sfpIdx);
    switch (valIdx) {
    case 0: stashString(VENDOR_NAME_ADDRESS, INFO_STRING_CHARS);    break;
    case 1: stashString(PART_NAME_ADDRESS, INFO_STRING_CHARS);      break;
    case 2: stashString(PART_NUMBER_ADDRESS, INFO_STRING_CHARS);    break;
    case 3: stashString(DATE_CODE_ADDRESS, DATE_STRING_CHARS);      break;
    case 4: stashWord(fetchRxPower());                              break;
    case 5: stashWord(fetchTemperature());                          break;
    default:
        sysmonStashStatus();
        valIdx = -1;
        if (sfpIdx == (NSFP - 1)) {
            stashOffset = 0;
            sfpIdx = 0;
        }
        else {
            sfpIdx++;
        }
        break;
    }
    valIdx++;
}
