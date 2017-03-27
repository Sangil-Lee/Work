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
 * Communicate with I2C bus devices connected to the programmable logic
 * For now just a single controller and a single device...
 */

#include <stdio.h>
#include <xparameters.h>
#include "xiic_l.h"
#include "iic.h"
#include "util.h"

int
iicInit(void)
{
    XIic_DynInit(XPAR_IIC_0_BASEADDR);
    return 0;
}

/* SetMux -- could be used to mux multile SCL/SDA lines to a single AXI IIC */
int
iicSetMux(int i)
{
    return 0;
}


int
iicSend(int i2cAddress, const uint8_t *buf, int len)
{
    return XIic_DynSend(XPAR_IIC_0_BASEADDR, i2cAddress, (uint8_t *)buf, len, XIIC_STOP);
}

int
iicRecv(int i2cAddress, int offset, uint8_t *buf, int len)
{
    if (offset >= 0) {
        uint8_t u = offset;
        if (XIic_DynSend(XPAR_IIC_0_BASEADDR, i2cAddress, &u, 1, XIIC_REPEATED_START) != 1) return -1;
    }
    return XIic_DynRecv(XPAR_IIC_0_BASEADDR, i2cAddress, buf, len);
}
