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
 * Communicate with GTX transceiver through its dynamic reconfiguration port
 */

#include <stdio.h>
#include <stdint.h>
#include <xil_io.h>
#include <xparameters.h>
#include "drp.h"
#include "util.h"

#define DRP_LANE_SELECT_SHIFT 11 /* 2048 bytes per DRP lane */
#define ADDR(base,reg) ((base)+((reg)<<2))

static void
drp_write(uint32_t base, int regOffset, int value)
{
    Xil_Out32(ADDR(base, regOffset), value);
}

static int
drp_read(uint32_t base, int regOffset)
{
    return Xil_In32(ADDR(base, regOffset));
}

static void
drp_rmw(uint32_t base, int regOffset, int mask, int value)
{
    uint16_t v = drp_read(base, regOffset);
    v &= ~mask;
    v |= value & mask;
    drp_write(base, regOffset, v);
}

static void
drp_set(uint32_t base, int regOffset, int bits)
{
    drp_rmw(base, regOffset, bits, bits);
}

static void
drp_clr(uint32_t base, int regOffset, int bits)
{
    drp_rmw(base, regOffset, bits, 0);
}

static void
drp_showReg(uint32_t base, const char *msg)
{
    int i;
    static const uint16_t regMap[] = {
        0x011, 0x06B };
    printf("\n%s DRP REGISTERS AT %X:\n", msg, (unsigned int)base);
    for (i = 0 ; i < (sizeof regMap / sizeof regMap[0]) ; i++) {
        printf("  %03X: %04X\n", regMap[i], drp_read(base, regMap[i]));
    }
}

void
drpShowReg(void)
{
    drp_showReg(XPAR_DRP_BRIDGE_0_BASEADDR, "SFP GTX");
}
