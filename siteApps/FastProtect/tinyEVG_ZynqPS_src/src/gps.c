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
 * Glue between event generator firmware and NMEA library routines
 */

#include <stdio.h>
#include <xil_io.h>
#include <xparameters.h>
#include <xuartps.h>
#include "gps.h"
#include "NMEA.h"
#include "util.h"

static XUartPs uart;

#define EVG_REG                 (XPAR_AXI_GPIO_EVG_BASEADDR+0)
#define HEARTBEAT_INTERVAL_REG  (XPAR_AXI_GPIO_EVG_BASEADDR+8)
#define EVG_R_PPS_TOGGLE 0x1

#define GPS_REG     (XPAR_AXI_GPIO_GPS_BASEADDR+0)
#define GPS_R_GPS_PPS   0x8
#define GPS_R_GPS_3DF   0x1

static int ppsToggle;
static uint32_t seconds;

void
gpsInit(void)
{
    uint32_t csr;
    int seconds = 0;
    int announced = 0;
    uint32_t then, when3DFhigh, now;

    XUartPs_Config *config = XUartPs_LookupConfig(XPAR_PS7_UART_0_DEVICE_ID);
    XUartPs_CfgInitialize(&uart, config, config->BaseAddress);
    XUartPs_SetBaudRate(&uart, 9600);
    when3DFhigh = then = now = ticksSinceBoot();
    for (;;) {
        now = ticksSinceBoot();
        csr = Xil_In32(GPS_REG);
        if (csr & GPS_R_GPS_3DF) when3DFhigh = now;
        if ((now - when3DFhigh) > ((ticksPerSecond / 2) * 3)) {
            printf("GPS locked");
            if (announced) printf(" after %d s", seconds);
            printf(".\n");
            break;
        }
        if ((now - then) > ticksPerSecond) {
            then += ticksPerSecond;
            seconds++;
            if (seconds == 2) {
                printf("Await GPS lock.\n");
                announced = 1;
            }
            if (seconds == 300) {
                warn("No GPS lock.  Proceding with no time reference!");
                break;
            }
        }
    }
    printf("Heartbeat event interval: %u EVG Tx ticks\n",
                                (unsigned int)Xil_In32(HEARTBEAT_INTERVAL_REG));
    ppsToggle = Xil_In32(EVG_REG) & EVG_R_PPS_TOGGLE;
}

void
gpsPoll(void)
{
    u8 c;
    uint32_t csr;
    static int reportClocks = 3;

    csr = Xil_In32(EVG_REG);
    if ((csr & EVG_R_PPS_TOGGLE) != ppsToggle) {
        ppsToggle = csr & EVG_R_PPS_TOGGLE;
        if (seconds) seconds++;
        Xil_Out32(EVG_REG, seconds);
        if (reportClocks && (--reportClocks == 0)) {
            printf("GTX transmitter clock %u Hz.\n",
                        (unsigned int)Xil_In32(XPAR_AXI_GPIO_DIAG_BASEADDR));
            printf("GTX receiver clock %u Hz.\n",
                        (unsigned int)Xil_In32(XPAR_AXI_GPIO_DIAG_BASEADDR+8));
        }
    }
    if (XUartPs_Recv(&uart, &c, 1) == 1)
        NMEAconsume(c);
}

/*
 * Callbacks from NMEAconsume
 */
void
NMEAcallback(const char *sentence)
{
    if (debugFlags & DEBUGFLAGS_NMEA_DATA)
        printf("NMEA: \"%s\"\n", sentence);
}

void
NMEAtime(unsigned int posixSeconds, int fractionalSeconds)
{
    seconds = posixSeconds;
    Xil_Out32(EVG_REG, seconds);
    if (debugFlags & DEBUGFLAGS_NMEA_TIME)
        printf("POSIX Seconds %u.%03d\n", posixSeconds, fractionalSeconds);
}

void
NMEAerror(const char *message)
{
    printf("GPS time conversion failure: %s\n", message);
}
