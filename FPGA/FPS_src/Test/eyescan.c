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
 * Perform a transceiver eye scan
 *
 * This code is based on the example provided in Xilinx application note
 * XAPP743, "Eye Scan with MicroBlaze Processor MCS Application Note".
 */
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <xil_io.h>
#include <xparameters.h>
#include "eyescan.h"
#include "util.h"

#define PRESCALE_CODE  6

#if EYESCAN_TRANSCEIVER_WIDTH == 40
# define DRP_REG_ES_SDATA_MASK0_VALUE   0x0000
# define DRP_REG_ES_SDATA_MASK1_VALUE   0x0000
#elif EYESCAN_TRANSCEIVER_WIDTH == 20
# define DRP_REG_ES_SDATA_MASK0_VALUE   0xFFFF
# define DRP_REG_ES_SDATA_MASK1_VALUE   0x000F
#else
# error "Invalid EYESCAN_TRANSCEIVER_WIDTH value"
#endif

#define VRANGE         128
#define VSTRIDE        8

#define DRP_REG_ES_QUAL_MASK0     0x031
#define DRP_REG_ES_QUAL_MASK1     0x032
#define DRP_REG_ES_QUAL_MASK2     0x033
#define DRP_REG_ES_QUAL_MASK3     0x034
#define DRP_REG_ES_QUAL_MASK4     0x035
#define DRP_REG_ES_SDATA_MASK0    0x036
#define DRP_REG_ES_SDATA_MASK1    0x037
#define DRP_REG_ES_SDATA_MASK2    0x038
#define DRP_REG_ES_SDATA_MASK3    0x039
#define DRP_REG_ES_SDATA_MASK4    0x03A
#define DRP_REG_ES_PS_VOFF        0x03B
#define DRP_REG_ES_HORZ_OFFSET    0x03C
#define DRP_REG_ES_CSR            0x03D
#define DRP_REG_ES_ERROR_COUNT    0x14F
#define DRP_REG_ES_SAMPLE_COUNT   0x150
#define DRP_REG_ES_STATUS         0x151
#define DRP_REG_PMA_RSV2          0x082
#define DRP_REG_TXOUT_RXOUT_DIV   0x088
#define DRP_LANE_SELECT_SHIFT 11 /* 2048 bytes per DRP lane */

#define ES_CSR_CONTROL_RUN      0x01
#define ES_CSR_CONTROL_ARM      0x02
#define ES_CSR_CONTROL_TRIG0    0x04
#define ES_CSR_CONTROL_TRIG1    0x08
#define ES_CSR_CONTROL_TRIG2    0x10
#define ES_CSR_CONTROL_TRIG3    0x20
#define ES_CSR_EYE_SCAN_EN      0x100
#define ES_CSR_ERRDET_EN        0x200

#define ES_STATUS_DONE          0x1
#define ES_STATUS_STATE_MASK    0xE
#define ES_STATUS_STATE_WAIT    0x0
#define ES_STATUS_STATE_RESET   0x2
#define ES_STATUS_STATE_END     0x4
#define ES_STATUS_STATE_COUNT   0x6
#define ES_STATUS_STATE_READ    0x8
#define ES_STATUS_STATE_ARMED   0xA

#define ES_PS_VOFF_PRESCALE_MASK  0xF800
#define ES_PS_VOFF_PRESCALE_SHIFT 11
#define ES_PS_VOFF_VOFFSET_MASK   0x1FF

#define ADDR(base,reg) ((base)+((reg)<<2))

static enum eyescanFormat {
    FMT_ASCII_ART,
    FMT_NUMERIC,
    FMT_RAW
} eyescanFormat;

static void
drp_write(uint32_t base, int regOffset, int value)
{
    Xil_Out32(ADDR(base, regOffset), value);
}

static int
drp_read(uint32_t base, int regOffset)
{
    return Xil_In32(ADDR(base, regOffset));
}

static void
drp_rmw(uint32_t base, int regOffset, int mask, int value)
{
    uint16_t v = drp_read(base, regOffset);
    v &= ~mask;
    v |= value & mask;
    drp_write(base, regOffset, v);
}

static void
drp_set(uint32_t base, int regOffset, int bits)
{
    drp_rmw(base, regOffset, bits, bits);
}

static void
drp_clr(uint32_t base, int regOffset, int bits)
{
    drp_rmw(base, regOffset, bits, 0);
}

static void
drp_showReg(uint32_t base, const char *msg)
{
    int i;
    static const uint16_t regMap[] = {
        DRP_REG_ES_QUAL_MASK0,  DRP_REG_ES_QUAL_MASK1,  DRP_REG_ES_QUAL_MASK2,
        DRP_REG_ES_QUAL_MASK3,  DRP_REG_ES_QUAL_MASK4,  DRP_REG_ES_SDATA_MASK0,
        DRP_REG_ES_SDATA_MASK1, DRP_REG_ES_SDATA_MASK2, DRP_REG_ES_SDATA_MASK3,
        DRP_REG_ES_SDATA_MASK4, DRP_REG_ES_PS_VOFF,     DRP_REG_ES_HORZ_OFFSET,
        DRP_REG_ES_CSR,         DRP_REG_ES_ERROR_COUNT, DRP_REG_ES_SAMPLE_COUNT,
        DRP_REG_ES_STATUS, DRP_REG_PMA_RSV2, DRP_REG_TXOUT_RXOUT_DIV };
    printf("\nEYE SCAN REGISTERS AT %X (%s):\n", (unsigned int)base, msg);
    for (i = 0 ; i < (sizeof regMap / sizeof regMap[0]) ; i++) {
        printf("  %03X: %04X\n", regMap[i], drp_read(base, regMap[i]));
    }
}

/*
 * Eye scan initialization changes the value of PMA_RSV2[5].
 * This means that a subsequent PMA reset is required.
 * Ensure that main() invokes eye scan initialization before Aurora
 * initialization since the latter peforms the required a PMA reset.
 */
void
eyescanInit(void)
{
    int lane, r;
    uint32_t base;

    for (lane = 0 ; lane < EYESCAN_LANECOUNT ; lane++) {
        base = EYSCAN_BASEADDR+(lane<<DRP_LANE_SELECT_SHIFT);
        /* Aurora wizard configures PMA_RSV2[5]=0. Eye scan requires 1. */
        drp_set(base, DRP_REG_PMA_RSV2, 1 << 5);

        /* Enable statistical eye scan */
        drp_set(base, DRP_REG_ES_CSR, ES_CSR_EYE_SCAN_EN | ES_CSR_ERRDET_EN);

        /* Set prescale */
        drp_rmw(base, DRP_REG_ES_PS_VOFF, ES_PS_VOFF_PRESCALE_MASK,
                                    PRESCALE_CODE << ES_PS_VOFF_PRESCALE_SHIFT);

        /* Set 80 bit ES_SDATA_MASK to check 40 or 20 bit data */
        drp_write(base, DRP_REG_ES_SDATA_MASK0, DRP_REG_ES_SDATA_MASK0_VALUE);
        drp_write(base, DRP_REG_ES_SDATA_MASK1, DRP_REG_ES_SDATA_MASK1_VALUE);
        drp_write(base, DRP_REG_ES_SDATA_MASK2, 0xFF00);
        drp_write(base, DRP_REG_ES_SDATA_MASK3, 0xFFFF);
        drp_write(base, DRP_REG_ES_SDATA_MASK4, 0xFFFF);

        /* Enable all bits in ES_QUAL_MASK (count all bits) */
        for (r = DRP_REG_ES_QUAL_MASK0 ; r <= DRP_REG_ES_QUAL_MASK4 ; r++) {
            drp_write(base, r, 0xFFFF);
        }
    }
}

static int
xBorderCrank(int lane)
{
    /* 2 characters per horizontal position except for last */
    const int width = 33 * 2 - 1;
    static int i = 33 * 2;

    if (eyescanFormat == FMT_RAW) return 0;
    if (lane >= 0) {
        i = -1;
    }
    if (i <= width) {
        if ((i == -1) || (i == width / 2)) {
            printf("+");
        }
        else if (i == width) {
            printf("+\n");
        }
        else {
            printf("-");
        }
        i++;
    }
    return (i <= width);
}

/*
 * Map log2err(errorCount) onto grey scale or alphanumberic value.
 * Special cases for 0 which is the only value that maps to ' ' and for
 * full scale error (131070) which is the only value that maps to '@'.
 */
static int
plotChar(int errorCount)
{
    int i, c;

    if (errorCount <= 0)           c = ' ';
    else if (errorCount >= 131070) c = '@';
    else {
        int log2err = 31 - __builtin_clz(errorCount);
        if (eyescanFormat == FMT_NUMERIC) {
            c = (log2err <= 9) ? '0' + log2err : 'A' - 10 + log2err;
        }
        else {
            i = 1 + ((log2err * 8) / 18);
            c = " .:-=?*#%@"[i];
        }
    }
    return c;
}

static int
eyescanStep(int lane)
{
    static int eyescanActive, eyescanAcquiring, eyescanLane;
    static int passCount;
    static int hRange, hStride;
    static int hOffset, vOffset, utFlag;
    static int errorCount;
    static uint32_t base;

    if ((lane >= 0) && !eyescanActive) {
        base = EYSCAN_BASEADDR + (lane << DRP_LANE_SELECT_SHIFT);
        /* Want 32 horizontal offsets on either side of baseline */
        int rxDiv = 1 << (drp_read(base, DRP_REG_TXOUT_RXOUT_DIV) & 0x3);
        hRange = 32 * rxDiv; 
        hStride = 2 * rxDiv;
        hOffset = -hRange;
        vOffset = -VRANGE;
        utFlag = 0;
        errorCount = 0;
        xBorderCrank(lane);
        eyescanLane = lane;
        eyescanAcquiring = 0;
        eyescanActive = 1;
        return 1;
    }
    if (xBorderCrank(-1)) {
        return 1;
    }
    if (eyescanActive) {
        if (eyescanAcquiring) {
            int status = drp_read(base, DRP_REG_ES_STATUS);
            if ((status & ES_STATUS_DONE) || (++passCount > 10000000)) {
                drp_clr(base, DRP_REG_ES_CSR, ES_CSR_CONTROL_RUN);
                eyescanAcquiring = 0;
                if (status == (ES_STATUS_STATE_END | ES_STATUS_DONE)) {
                    char border = vOffset == 0 ? '+' : '|';
                    errorCount += drp_read(base, DRP_REG_ES_ERROR_COUNT);
                    utFlag = !utFlag;
                    if (!utFlag) {
                        if (eyescanFormat == FMT_RAW) {
                            printf(" %6d", errorCount);
                            border = ' ';
                        }
                        else {
                            char c;
                            printf("%c", (hOffset == -hRange) ? border : ' ');
                            if ((errorCount==0) && (hOffset==0) && (vOffset==0))
                                c = '+';
                            else
                                c = plotChar(errorCount);
                            printf("%c", c);
                        }
                        errorCount = 0;
                        hOffset += hStride;
                        if (hOffset > hRange) {
                            printf("%c\n", border);
                            hOffset = -hRange;
                            vOffset += VSTRIDE;
                        }
                    }
                }
                else {
                    drp_showReg(base, "SCAN FAILURE");
                    eyescanActive = 0;
                }
            }
        }
        else if (vOffset > VRANGE) {
            xBorderCrank(eyescanLane);
            eyescanActive = 0;
        }
        else {
            int hSign;
            int vSign, vAbs;
            
            hSign = (hOffset < 0) ? 1 << 11 : 0;
            if (vOffset < 0) {
                vAbs = -vOffset;
                vSign = 1 << 8;
            }
            else {
                vAbs = vOffset;
                vSign = 0;
            }
            if (vAbs > 127) vAbs = 127;
            drp_rmw(base, DRP_REG_ES_PS_VOFF, ES_PS_VOFF_VOFFSET_MASK,
                                        (utFlag ? (1 << 9) : 0) | vSign | vAbs);
            drp_write(base, DRP_REG_ES_HORZ_OFFSET, hSign | (hOffset & 0x7FF));
            drp_set(base, DRP_REG_ES_CSR, ES_CSR_CONTROL_RUN);
            passCount = 0;
            eyescanAcquiring = 1;
        }
    }
    return eyescanActive;
}

int
eyescanCrank(void)
{
    return eyescanStep(-1);
}

int
eyescanCommand(int argc, char **argv)
{
    int i;
    char *endp;
    enum eyescanFormat format = FMT_ASCII_ART;
    int lane = -1;
    static const char *name[] = EYESCAN_LANE_NAMES;

    for (i = 1 ; i < argc ; i++) {
        if (argv[i][0] == '-') {
            switch (argv[i][1]) {
            case 'n': format = FMT_NUMERIC;   break;
            case 'r': format = FMT_RAW;       break;
            default: return 1;
            }
            if (argv[i][2] != '\0') return 1;
        }
        else {
            lane = strtol(argv[i], &endp, 0);
            if ((*endp != '\0') || (lane < 0) || (lane >= EYESCAN_LANECOUNT))
                return 1;
        }
    }
    if (lane < 0) lane = 0;
    eyescanFormat = format;
    if (lane < (sizeof name / sizeof name[0])) printf("%s\n", name[lane]);
    eyescanStep(lane);
    return 0;
}

