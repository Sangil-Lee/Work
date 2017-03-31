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
 * Communicate with I2C bus devices connected to the processor
 */

#include <stdio.h>
#include <xparameters.h>
#include <xiicps.h>
#include "iicps.h"
#include "util.h"


#define IIC_SCLK_RATE           100000
#define IIC_MUX_ADDRESS         0x74

static XIicPs iicps;

int
iicpsInit(void)
{
    int s;
    XIicPs_Config *ConfigPtr;

    ConfigPtr = XIicPs_LookupConfig(XPAR_XIICPS_0_DEVICE_ID);
    if (ConfigPtr == NULL) return -1;
    s = XIicPs_CfgInitialize(&iicps, ConfigPtr, ConfigPtr->BaseAddress);
    if (s != XST_SUCCESS) return -1;
    XIicPs_SetSClk(&iicps, IIC_SCLK_RATE);
    return 0;
}

int
iicpsSetMux(int bitmap)
{
    uint8_t a = bitmap;
    static int lastAddress = -1;

    if (bitmap != lastAddress) {
        if (iicpsSend(IIC_MUX_ADDRESS, &a, 1) != 1) {
            lastAddress = -1;
            return -1;
        }
        lastAddress = bitmap;
    }
    return 0;
}

int
iicpsSend(int i2cAddress, const uint8_t *buf, int len)
{
    int s;
    int pass = 0;

    s = XIicPs_MasterSendPolled(&iicps, (uint8_t *)buf, len, i2cAddress);
    if (s != XST_SUCCESS) return -1;
    while (XIicPs_BusIsBusy(&iicps)) {
        if (++pass >= 1000000) fatal("XIICPS locked up");
    }
    return len;
}

int
iicpsRecv(int i2cAaddress, int offset, uint8_t *buf, int len)
{
    int s;
    int pass = 0;

    if (offset >= 0) {
        uint8_t u = offset;
        if (iicpsSend(i2cAaddress, &u, 1) != 1) return -1;
    }
    s = XIicPs_MasterRecvPolled(&iicps, buf, len, i2cAaddress);
    if (s != XST_SUCCESS) return -1;
    while (XIicPs_BusIsBusy(&iicps)) {
        if (++pass >= 1000000) fatal("XIICPS locked up");
    }
    return len;
}
