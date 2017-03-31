#include <stdio.h>
#include <xil_io.h>
#include <xparameters.h>
#include "aurora.h"
#include "util.h"

/* CSR control/status */
#define AUCSR_GTX_RESET             0x1
#define AUCSR_AURORA_RESET          0x2
#define AUCSR_LOG_ENABLE            0x4
#define AUCSR_LOG_FORCE             0x8

/* Status byte */
#define AUSB_CHANNEL_UP             0x80
#define AUSB_FRAME_ERROR            0x20
#define AUSB_HARD_ERROR             0x10
#define AUSB_SOFT_ERROR             0x8
#define AUSB_TX_LOCK                0x4
#define AUSB_TX_RESET_DONE          0x2
#define AUSB_RX_RESET_DONE          0x1

uint32_t csrShadow = 0;

void
static csrSet(uint32_t bits)
{
    csrShadow |= bits;
    Xil_Out32(XPAR_GPIO_AURORA_BASEADDR, csrShadow);
}

static void
csrClr(uint32_t bits)
{
csrShadow &= ~bits;
    Xil_Out32(XPAR_GPIO_AURORA_BASEADDR, csrShadow);
}

void
auroraInit(void)
{
    csrSet(AUCSR_AURORA_RESET | AUCSR_GTX_RESET);
    microsecondSpin(1000);
    csrClr(AUCSR_GTX_RESET);
    microsecondSpin(1000000);
    csrClr(AUCSR_AURORA_RESET);
    microsecondSpin(500000);
    auroraShowStatus();
}

void
auroraLogEnable(void)
{
    csrSet(AUCSR_LOG_ENABLE);
}

void
auroraLogForce(void)
{
    csrSet(AUCSR_LOG_FORCE);
    csrClr(AUCSR_LOG_FORCE);
}

static void
showStatusByte(int linkIndex, int sb)
{
    int b;
    
    printf("Aurora link %d: ", linkIndex);
    printf("Link %s", sb & AUSB_CHANNEL_UP ? "up" : "down");
    for (b = 0x20 ; b ; b >>= 1) {
        switch (b & sb) {
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
    int linkIndex = 0;
    int r, i;

    for (r = 0 ; r < 2 ; r++) {
        uint32_t a = XPAR_GPIO_AURORA_BASEADDR + (r ? 8 : 0);
        uint32_t csr = Xil_In32(a);
        for (i = 0 ; i < 4 ; i++) {
            showStatusByte(linkIndex, csr);
            if (++linkIndex == AURORA_LINK_COUNT) return;
            csr >>= 8;
        }
    }
}
