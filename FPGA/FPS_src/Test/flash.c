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
 * Transfer data to/from bootstrap flash memory.
 * Many of these routines use static buffers and as such are not thread safe!
 *
 * Magic numbers in initialization routine were obtained from
 * Xilinx code provided in xilisf_qspips_stm_polled_example.c.
 */

#include <stdio.h>
#include <string.h>
#include "xparameters.h"
#include <xilisf.h>
#include "util.h"

#define MAX_TRANSFER   256

#define LQSPI_CR_FAST_QUAD_READ 0x0000006B
#define LQSPI_CR_1_DUMMY_BYTE   0x00000100
#define DUAL_QSPI_CONFIG_WRITE  (XQSPIPS_LQSPI_CR_TWO_MEM_MASK | \
                                XQSPIPS_LQSPI_CR_SEP_BUS_MASK | \
                                LQSPI_CR_1_DUMMY_BYTE | \
                                LQSPI_CR_FAST_QUAD_READ)
static XIsf Isf;
static XQspiPs_Config *config; 

void
flashInit(void)
{
    static XQspiPs xQSPI;
    static uint8_t writeBuffer[MAX_TRANSFER + XISF_CMD_SEND_EXTRA_BYTES];

    if ((config = XQspiPs_LookupConfig(XPAR_XQSPIPS_0_DEVICE_ID)) == NULL)
        fatal("Can't find QSPI configuration");
    if (XQspiPs_CfgInitialize(&xQSPI, config, config->BaseAddress) != XST_SUCCESS)
        fatal("Can't initialize QSPI instance");
    XIsf_SetSpiConfiguration(&Isf, &xQSPI, XQSPIPS_FORCE_SSELECT_OPTION |
                                           XQSPIPS_MANUAL_START_OPTION  |
                                           XQSPIPS_HOLD_B_DRIVE_OPTION,
                                                          XISF_SPI_PRESCALER);
    if (config->ConnectionMode != XQSPIPS_CONNECTION_MODE_PARALLEL)
        fatal("QSPI flash not configured as dual parallel");
    XQspiPs_SetLqspiConfigReg(&xQSPI, DUAL_QSPI_CONFIG_WRITE);
    XIsf_Initialize(&Isf, &xQSPI, 0, writeBuffer);
    printf("Flash sector size %d, count %d (%d MB)\n",
                              (int)Isf.SectorSize, (int)Isf.NumSectors,
                              (int)(Isf.SectorSize*Isf.NumSectors/(1024*1024)));
}

/*
 * Have to read into local buffer since XIsf_Read() reads an extra byte
 */
int
flashRead(unsigned int offset, char *buf, unsigned int count)
{
    unsigned int remaining = count;
    XIsf_ReadParam rparm;

    while (remaining) {
        static uint8_t myBuf[MAX_TRANSFER + 1];
        int n = remaining <= MAX_TRANSFER ? remaining : MAX_TRANSFER;
        rparm.Address = offset;
        rparm.NumBytes = n;
        rparm.ReadPtr = myBuf;
        rparm.NumDummyBytes = 1;
        if (XIsf_Read(&Isf, XISF_READ, &rparm) != XST_SUCCESS) {
            printf("Flash read of %d at %#x failed.\n", n, offset);
            return -1;
        }
        memcpy(buf, myBuf, n);
        buf += n;
        offset += n;
        remaining -= n;
    }
    return count;
}

int
flashWrite(unsigned int offset, const char *buf, unsigned int count)
{
    unsigned int remaining = count;
    XIsf_WriteParam wparm;
    static unsigned int tail = ~0;
    static int lastSectorErased = -1;
    static int isEnabled;

    while (remaining) {
        int sector = offset / Isf.SectorSize;
        int sBase = sector * Isf.SectorSize;
        int sOffset = offset - sBase;
        int sSpace = Isf.SectorSize - sOffset;
        int n = remaining <= sSpace ? remaining : sSpace;
        if (n > MAX_TRANSFER) n = MAX_TRANSFER;
        if ((offset < tail) || (sector != lastSectorErased)) {
            if (!isEnabled) {
                if (XIsf_WriteEnable(&Isf, XISF_WRITE_ENABLE) != XST_SUCCESS) {
                    printf("Flash write enable failed.\n");
                    return count - remaining;
                }
                isEnabled = 1;
            }
            if (XIsf_Erase(&Isf, XISF_SECTOR_ERASE, sBase) != XST_SUCCESS) {
                printf("Flash erase of sector %d failed.\n", sector);
                lastSectorErased = -1;
                return count - remaining;
            }
            lastSectorErased = sector;
        }
        wparm.Address = offset;
        wparm.NumBytes = n;
        wparm.WritePtr = (unsigned char *)buf;
        if (XIsf_Write(&Isf, XISF_QUAD_IP_PAGE_WRITE, &wparm) != XST_SUCCESS) {
            printf("Flash write of %d at %#x failed.\n", n, offset);
            return -1;
        }
        buf += n;
        offset += n;
        remaining -= n;
        tail = offset;
    }
    return count;
}
