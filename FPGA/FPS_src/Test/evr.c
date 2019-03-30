#include <stdio.h>
#include <xil_io.h>
#include <xparameters.h>
#include "evr.h"
#include "util.h"

/* CSR control/status */
#define EVR_CONTROL_PLL_RESET           0x1
#define EVR_CONTROL_SOFT_RESET          0x2
#define EVR_CONTROL_GTX_RESET           0x4
#define EVR_CONTROL_PMA_RESET           0x8
#define EVR_STATUS_RX_FSM_RESET_DONE    0x1
#define EVR_STATUS_RX_RESET_DONE        0x2
#define EVR_STATUS_TIMESTAMP_VALID      0x4

void
evrInit(void)
{
    uint32_t then;
    int pass;

    Xil_Out32(XPAR_GPIO_EVR_GTX_BASEADDR, EVR_CONTROL_PMA_RESET |
                                          EVR_CONTROL_GTX_RESET |
                                          EVR_CONTROL_SOFT_RESET |
                                          EVR_CONTROL_PLL_RESET);
    microsecondSpin(10000);
    Xil_Out32(XPAR_GPIO_EVR_GTX_BASEADDR, EVR_CONTROL_PMA_RESET |
                                          EVR_CONTROL_GTX_RESET |
                                          EVR_CONTROL_SOFT_RESET);
    microsecondSpin(10000);
    Xil_Out32(XPAR_GPIO_EVR_GTX_BASEADDR, EVR_CONTROL_PMA_RESET |
                                          EVR_CONTROL_SOFT_RESET);
    microsecondSpin(10000);
    Xil_Out32(XPAR_GPIO_EVR_GTX_BASEADDR, EVR_CONTROL_PMA_RESET);
    microsecondSpin(10000);
    Xil_Out32(XPAR_GPIO_EVR_GTX_BASEADDR, 0);
    then = ticksSinceBoot();
    for (;;) {
        uint32_t csr = Xil_In32(XPAR_GPIO_EVR_GTX_BASEADDR);
        if ((csr & (EVR_STATUS_RX_FSM_RESET_DONE | EVR_STATUS_RX_RESET_DONE)) ==
                   (EVR_STATUS_RX_FSM_RESET_DONE | EVR_STATUS_RX_RESET_DONE)) {
            break;
        }
        if ((ticksSinceBoot() - then) > ticksPerSecond) {
            warn("EVR link reset did not complete.");
            break;
        }
    }
    then = ticksSinceBoot();
    pass = 0;
    for (;;) {
        uint32_t csr = Xil_In32(XPAR_GPIO_EVR_GTX_BASEADDR);
        if ((csr & EVR_STATUS_TIMESTAMP_VALID) == EVR_STATUS_TIMESTAMP_VALID) {
            break;
        }
        if ((ticksSinceBoot() - then) > ticksPerSecond) {
            if (++pass == 40) {
                warn("EVR is not receiving time stamps");
                break;
            }
            if (pass == 2) warn("Waiting for EVR");
            then += ticksPerSecond;
        }
    }
    evrShowStatus();
}

void
evrShowStatus(void)
{
    uint32_t csr = Xil_In32(XPAR_GPIO_EVR_GTX_BASEADDR);

    printf("Event receiver:\n");
    printf("          Reset done: %d\n", (csr & EVR_STATUS_RX_RESET_DONE) != 0);
    printf("      FSM reset done: %d\n", (csr&EVR_STATUS_RX_FSM_RESET_DONE)!=0);
    printf("     Timestamp valid: %d\n", (csr&EVR_STATUS_TIMESTAMP_VALID)!=0);
    if (csr & EVR_STATUS_TIMESTAMP_VALID) {
        uint32_t oldSeconds, newSeconds, ticks;
        for (;;) {
            oldSeconds = Xil_In32(XPAR_GPIO_TIMESTAMP_BASEADDR);
            ticks = Xil_In32(XPAR_GPIO_TIMESTAMP_BASEADDR + 8);
            newSeconds = Xil_In32(XPAR_GPIO_TIMESTAMP_BASEADDR);
            if (newSeconds == oldSeconds) break;
        }
        printf("           Timestamp: %u seconds, %u ticks\n",
                                (unsigned int)newSeconds, (unsigned int)ticks);
    }
}
