#include <stdio.h>
#include <xil_io.h>
#include <xparameters.h>
#include "aurora.h"
#include "util.h"

/* CSR control/status */
#define AUCSR_GTX_RESET             0x1
#define AUCSR_AURORA_RESET          0x2

/* Status byte (two per CSR) */
#define AUSB_PLL_NOT_LOCKED         0x80 /* PLL B only */
#define AUSB_CHANNEL_UP             0x40
#define AUSB_FRAME_ERROR            0x20
#define AUSB_HARD_ERROR             0x10
#define AUSB_RX_RESET_DONE          0x8
#define AUSB_SOFT_ERROR             0x4
#define AUSB_TX_LOCK                0x2
#define AUSB_TX_RESET_DONE          0x1

void
auroraInit(void)
{
    Xil_Out32(XPAR_LINK_GPIO_BASEADDR, AUCSR_AURORA_RESET | AUCSR_GTX_RESET);
    microsecondSpin(1000);
    Xil_Out32(XPAR_LINK_GPIO_BASEADDR, AUCSR_AURORA_RESET);
    microsecondSpin(1000000);
    Xil_Out32(XPAR_LINK_GPIO_BASEADDR, 0);
    microsecondSpin(500000);
    auroraShowStatus();
}

static void
showStatusByte(const char *linkName, int sb)
{
    int b;
    
    printf("Aurora %s: ", linkName);
    printf("Link %s", sb & AUSB_CHANNEL_UP ? "up" : "down");
    for (b = 0x20 ; b ; b >>= 1) {
        switch (b & sb) {
        case AUSB_PLL_NOT_LOCKED: printf(", PLL not locked"); break;
        case AUSB_FRAME_ERROR:    printf(", Framing error");  break;
        case AUSB_HARD_ERROR:     printf(", Hard error");     break;
        case AUSB_RX_RESET_DONE:  printf(", Rx reset done");  break;
        case AUSB_SOFT_ERROR:     printf(", Soft error");     break;
        case AUSB_TX_LOCK:        printf(", Tx locked");      break;
        case AUSB_TX_RESET_DONE:  printf(", Tx reset done");  break;
        }
    }
    printf(".\n");
}

void
auroraShowStatus(void)
{
    uint32_t csr = Xil_In32(XPAR_LINK_GPIO_BASEADDR);

    showStatusByte("Toward Mitigation", csr >> 16);
    showStatusByte("Away From Mitigation", csr >> 24);
}
