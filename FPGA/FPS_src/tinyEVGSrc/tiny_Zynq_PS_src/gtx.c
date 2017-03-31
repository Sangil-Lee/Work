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
#include "gtx.h"
#include "util.h"

#define CSR_W_GTX_RESET         0x1
#define CSR_W_SOFT_RESET        0x2

#define CSR_R_GTX_RESET         0x1
#define CSR_R_SOFT_RESET        0x2
#define CSR_R_TX_RESET_DONE     0x4
#define CSR_R_TX_FSM_RESET_DONE 0x8
#define CSR_R_RX_RESET_DONE     0x10
#define CSR_R_RX_FSM_RESET_DONE 0x20

void
gtxInit(void)
{
    uint32_t csr;
    int pass = 0;

    csr = Xil_In32(XPAR_AXI_GPIO_GTX_CONTROL_BASEADDR);
    if ((csr & (CSR_R_SOFT_RESET|CSR_R_GTX_RESET)) != (CSR_R_SOFT_RESET|CSR_R_GTX_RESET)) {
        warn("GTX not in reset state on power-up (CSR:%x).", (unsigned int)csr);
        Xil_Out32(XPAR_AXI_GPIO_GTX_CONTROL_BASEADDR, CSR_W_SOFT_RESET|CSR_W_GTX_RESET);
        microsecondSpin(100);
    }
    Xil_Out32(XPAR_AXI_GPIO_GTX_CONTROL_BASEADDR, 0);
    for (;;) {
        microsecondSpin(100);
        csr = Xil_In32(XPAR_AXI_GPIO_GTX_CONTROL_BASEADDR);
        if ((csr & (CSR_R_TX_FSM_RESET_DONE | CSR_R_TX_RESET_DONE)) ==
                   (CSR_R_TX_FSM_RESET_DONE | CSR_R_TX_RESET_DONE))
            break;
        if (++pass == 1000) {
            if (!(csr & CSR_R_TX_FSM_RESET_DONE))
                warn("GTX transmitter FSM didn't reset.");
            if (!(csr & CSR_R_TX_RESET_DONE))
                warn("GTX transmitter didn't reset.");
            break;
        }
    }
    printf("GTX CSR 0x%x after pass %d.", (unsigned int)csr, pass);
    if (csr == (CSR_R_RX_FSM_RESET_DONE | 
                CSR_R_RX_RESET_DONE     |
                CSR_R_TX_FSM_RESET_DONE |
                CSR_R_TX_RESET_DONE)) {
        printf("  All resets complete.");
    }
    printf("\n");
}
