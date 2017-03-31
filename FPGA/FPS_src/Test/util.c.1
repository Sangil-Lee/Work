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
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
//#include <time.h>
#include <math.h>
#include <xil_io.h>
#include <xuartps_hw.h>
#include <xparameters.h>
#include "util.h"

int debugFlags;

const int ticksPerSecond = (XPAR_CPU_CORTEXA9_CORE_CLOCK_FREQ_HZ / 2);
const double xTicksPerSecond = (XPAR_CPU_CORTEXA9_CORE_CLOCK_FREQ_HZ / 2.0);

void
fatal(const char *fmt, ...)
{
    va_list args;

    printf("***************************************************************\n");
    for (;;) {
        printf("*** Fatal:  ");
        va_start(args, fmt);
        vprintf(fmt, args);
        va_end(args);
        printf("\n");
        microsecondSpin(2000000);
    }
}

void
warn(const char *fmt, ...)
{
    va_list args;

    printf("*** Warning:  ");
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
    printf("\n");
}

void
microsecondSpin(unsigned int us)
{
    uint32_t then, limit;
    then = Xil_In32(XPAR_PS7_GLOBALTIMER_0_S_AXI_BASEADDR+0x00);
    limit = us * ((ticksPerSecond + 500000) / 1000000);
    while ((Xil_In32(XPAR_PS7_GLOBALTIMER_0_S_AXI_BASEADDR+0x00)-then) < limit);
}

unsigned int
ticksSinceBoot(void)
{
    return Xil_In32(XPAR_PS7_GLOBALTIMER_0_S_AXI_BASEADDR+0x00);
}

double
secondsSinceBoot(void)
{
    uint32_t tLo, tHi, tHn;

    tHi = Xil_In32(XPAR_PS7_GLOBALTIMER_0_S_AXI_BASEADDR+0x04);
    for (;;) {
        tLo = Xil_In32(XPAR_PS7_GLOBALTIMER_0_S_AXI_BASEADDR+0x00);
        tHn =  Xil_In32(XPAR_PS7_GLOBALTIMER_0_S_AXI_BASEADDR+0x04);
        if (tHn == tHi) break;
        tHi = tHn;
    }
    /* Double division is a lot faster than 64 bit integer division */
    /* Double significand provides better than 1 second resolution
     * for up to 1.7 years of operation (2^54/333e6 seconds */
    return (ldexp((double)tHi, 32) + (double)tLo) / xTicksPerSecond;
}
