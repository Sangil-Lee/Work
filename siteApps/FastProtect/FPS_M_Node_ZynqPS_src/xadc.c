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
#include <xil_io.h>
#include <xparameters.h>
#include "xadc.h"
#include "util.h"

#define In32(r_offset)    Xil_In32(XPAR_XADC_WIZ_0_BASEADDR+(r_offset))

#define R_TEMP          0x200 /* On-chip Temperature */
#define R_VCCINT        0x204 /* VCCINT */
#define R_VCCAUX        0x208 /* VCCAUX */
#define R_VBRAM         0x218 /* VBRAM */
#define R_VCCPINT       0x234 /* VCCPINT */
#define R_VCCPAUX       0x238 /* VCCPAUX */
#define R_VCCO_DDR      0x23C /* VCCO_DDR */
#define R_CFR0          0x300 /* Configuration Register 0 */
#define R_CFR1          0x304 /* Configuration Register 1 */
#define R_CFR2          0x308 /* Configuration Register 2 */
#define R_SEQ00         0x320 /* Seq Reg 00 -- Channel Selection */
#define R_SEQ01         0x324 /* Seq Reg 01 -- Channel Selection */
#define R_SEQ02         0x328 /* Seq Reg 02 -- Average Enable */
#define R_SEQ03         0x32C /* Seq Reg 03 -- Average Enable */
#define R_SEQ04         0x330 /* Seq Reg 04 -- Input Mode Select */
#define R_SEQ05         0x334 /* Seq Reg 05 -- Input Mode Select */
#define R_SEQ06         0x338 /* Seq Reg 06 -- Acquisition Time Select */
#define R_SEQ07         0x33C /* Seq Reg 07 -- Acquisition Time Select */

void
showreg(const char *msg, unsigned int reg)
{
    printf("%8s: %04X\n", msg, reg);
}

void
xadcShow(void)
{
    printf("XADC Configuration:\n");
    showreg("R_CFR0", In32(R_CFR0));
    showreg("R_CFR1", In32(R_CFR1));
    showreg("R_CFR2", In32(R_CFR2));
    showreg("R_SEQ00", In32(R_SEQ00));
    showreg("R_SEQ01", In32(R_SEQ01));
    showreg("R_SEQ02", In32(R_SEQ02));
    showreg("R_SEQ03", In32(R_SEQ03));
    showreg("R_SEQ04", In32(R_SEQ04));
    showreg("R_SEQ05", In32(R_SEQ05));
    showreg("R_SEQ06", In32(R_SEQ06));
    showreg("R_SEQ07", In32(R_SEQ07));
}

int
xadcVal(unsigned int chan)
{
    static short map[] = { R_TEMP, R_VCCINT, R_VCCAUX, R_VBRAM,
                           R_VCCPINT, R_VCCPAUX, R_VCCO_DDR };
    if (chan >= (sizeof map / sizeof map[0])) return 0;
    return In32(map[chan]);
}

