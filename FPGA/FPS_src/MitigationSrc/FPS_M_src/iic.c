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
 * A single IIC and multiple devices -- multiplexing of
 * incoming data is handled in the firmware.
 */

#include <stdio.h>
#include <xparameters.h>
#include "xil_io.h"
#include "iic.h"
#include "util.h"

#define IIC_REG_ISR             0x020
#define IIC_REG_SOFTR           0x040
#define IIC_REG_CR              0x100 
#define IIC_REG_SR              0x104
#define IIC_REG_TX_FIFO         0x108
#define IIC_REG_RX_FIFO         0x10C
#define IIC_REG_RX_FIFO_PIRQ    0x120
#define IIC_REG_GPO             0x124

#define ISR_TX_ERROR        0x02

#define CR_TX_FIFO_RESET    0x2
#define CR_TX_EN            0x1

#define SR_TX_FIFO_EMPTY    0x80
#define SR_RX_FIFO_EMPTY    0x40
#define SR_BB               0x04

#define TX_FIFO_START       0x100
#define TX_FIFO_STOP        0x200

#define IIC_IN(reg) Xil_In32(XPAR_IIC_0_BASEADDR + (reg))
#define IIC_OUT(reg,val) Xil_Out32(XPAR_IIC_0_BASEADDR + (reg), (val))

int
iicInit(void)
{
    IIC_OUT(IIC_REG_CR, CR_TX_EN);
    IIC_OUT(IIC_REG_SOFTR, 0xA); /* Soft reset */
    IIC_OUT(IIC_REG_CR, CR_TX_EN);
    IIC_OUT(IIC_REG_RX_FIFO_PIRQ, 0x0F); /* Full RX FIFO */
    microsecondSpin(1000);
    return 0;
}

/* SetMux -- Mux multiple SCL/SDA lines to a single AXI IIC */
int
iicSetMux(int i)
{
    IIC_OUT(IIC_REG_GPO, i);
    return i;
}

static void
show(const char *msg)
{
    int isr = IIC_IN(IIC_REG_ISR);
    int sr = IIC_IN(IIC_REG_SR);
    int cr = IIC_IN(IIC_REG_CR);

    printf("%s", msg);
    printf("  ISR:%02X", isr & 0xFF);
    printf("   CR:%02X", cr & 0xFF);
    if (cr & 0x40) printf(" GC_EN");
    if (cr & 0x20) printf(" RSTA");
    if (cr & 0x10) printf(" TXAKL");
    if (cr & 0x08) printf(" TX");
    if (cr & 0x04) printf(" MSMS");
    if (cr & 0x02) printf(" TX_FIFOreset");
    if (cr & 0x01) printf(" EN");
    printf("  SR:%02X", sr & 0xFF);
    if (sr & 0x80) printf(" TXempty");
    if (sr & 0x40) printf(" RXempty");
    if (sr & 0x20) printf(" RXfull");
    if (sr & 0x10) printf(" TXfull");
    if (sr & 0x08) printf(" SRW");
    if (sr & 0x04) printf(" BusBusy");
    if (sr & 0x02) printf(" AAS");
    if (sr & 0x01) printf(" ABGC");
    printf("\n");
}

int
iicRecv(int i2cAddress, int offset, uint8_t *buf, int len)
{
    uint32_t sr;
    int pass = 0;
    int nLeft = len;

    for (;;) {
        sr = IIC_IN(IIC_REG_SR);
        if ((sr & (SR_TX_FIFO_EMPTY | SR_RX_FIFO_EMPTY | SR_BB)) ==
                  (SR_TX_FIFO_EMPTY | SR_RX_FIFO_EMPTY))
            break;
        show("UNEXPECTED STATE");
        switch (pass) {
        case 0:
            IIC_OUT(IIC_REG_CR, CR_TX_FIFO_RESET | CR_TX_EN);
            show("FIFO RESET");
            IIC_OUT(IIC_REG_CR, CR_TX_EN);
            show("FIFO EN");
            break;

        case 1:
            IIC_OUT(IIC_REG_SOFTR, 0xA); /* Soft reset */
            IIC_OUT(IIC_REG_CR, CR_TX_EN);
            IIC_OUT(IIC_REG_RX_FIFO_PIRQ, 0x0F); /* Full RX FIFO */
            show("FULL RESET");

        default: return -1;
        }
        microsecondSpin(1000);
        pass++;
    }
    IIC_OUT(IIC_REG_TX_FIFO, TX_FIFO_START | (i2cAddress << 1));
    IIC_OUT(IIC_REG_TX_FIFO, offset);
    IIC_OUT(IIC_REG_TX_FIFO, TX_FIFO_START | (i2cAddress << 1) | 0x01);
    IIC_OUT(IIC_REG_TX_FIFO, len);
    pass = 0;
    while (nLeft) {
        if (IIC_IN(IIC_REG_SR) & SR_RX_FIFO_EMPTY) {
            if (++pass == 1000) {
                show("TIMEOUT");
                return -1;
            }
            microsecondSpin(100);
        }
        else {
            *buf++ = IIC_IN(IIC_REG_RX_FIFO);
            nLeft--;
        }
    }
    return len;
}
