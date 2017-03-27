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
#include "iicps.h"
#include "sfp.h"
#include "util.h"

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

static const unsigned char *
fetchString(unsigned char *cbuf, int base, int len)
{
    int i;

    if (iicpsRecv(SFP_INFO_ADDRESS, base, cbuf, len) != len)
        fatal("Can't read %d from from SFP address %d", len, base);
    for (i = len - 1 ; i >= 0 ; i--) {
        if (cbuf[i] != ' ') break;
    }
    cbuf[i+1] = '\0';
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
    if (iicpsRecv(SFP_STATUS_ADDRESS, reg, cbuf, 1) != 1) {
        warn("Can't read SFP register %d", reg);
        return -1;
    }
    return cbuf[0];
}

void
sfpShowRxPower(void)
{
    unsigned char cbuf[2];
    if (iicpsRecv(SFP_STATUS_ADDRESS, STATUS_RX_POWER_ADDRESS, cbuf, 2) != 2) {
        warn("Can't read SFP Rx power");
        return;
    }
    printf("  Receiver power: %d uW\n", ((cbuf[0] << 8) | cbuf[1]) / 10);
}

static void
showTemperature(void)
{
    unsigned char cbuf[2];
    if (iicpsRecv(SFP_STATUS_ADDRESS, STATUS_TEMPERATURE_ADDRESS, cbuf, 2) != 2) {
        warn("Can't read SFP Rx power");
        return;
    }
    printf("     Temperature: %d.%02d C\n", cbuf[0], (cbuf[1] * 100) / 256);
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

void
sfpInit(void)
{
    int r110;
    unsigned char dateCode[DATE_STRING_CHARS+1];
    char iso8601[12];

    if (iicpsSetMux(IICPS_SELECT_SI570) < 0)
        fatal("Can't configure I2C MUX for SFP/SI570");
    printf("SFP:\n");
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
    sfpShowRxPower();
    showTemperature();
}
