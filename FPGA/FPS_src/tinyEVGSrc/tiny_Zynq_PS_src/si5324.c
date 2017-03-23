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
 * Set up SI5324 clock generator to provide GTX ethernet reference.
 */
#include <stdio.h>
#include "iicps.h"
#include "si5324.h"
#include "util.h"

#define SI5324_ADDRESS 0x68

/* From Silicon Labs clock generation tool */
// HEADER
//  Date: Monday, December 19, 2016 9:19 AM
//  File Version: 3
//  Software Name: Precision Clock EVB Software
//  Software Version: 5.1
//  Software Date: July 23, 2014
//  Part number: Si5324
// END_HEADER
// PROFILE
//  Name: Si5324
// INPUT
//  Name: CKIN
//  Channel: 1
//  Frequency (MHz): 19.440000
//  N3: 1296
//  Maximum (MHz): 22.610215
//  Minimum (MHz): 19.340307
// END_INPUT
// INPUT
//  Name: CKIN
//  Channel: 2
//  Frequency (MHz): 114.285000
//  N3: 7619
//  CKIN2 to CKIN1 Ratio: 7619 / 1296
//  Maximum (MHz): 132.922246
//  Minimum (MHz): 113.698923
// END_INPUT
// PLL
//  Name: PLL
//  Frequency (MHz): 4875.000000
//  XA-XB Frequency (MHz): 114.285000
//  f3 (MHz): 0.015000
//  N1_HS: 10
//  N2_HS: 10
//  N2_LS: 32500
//  Phase Offset Resolution (ns): 2.05128
//  BWSEL_REG Option: Frequency (Hz)
//   3:    9
//   2:   19
//   1:   39
// END_PLL
// OUTPUT
//  Name: CKOUT
//  Channel: 1
//  Frequency (MHz): 81.250000
//  NC1_LS: 6
//  CKOUT1 to CKIN1 Ratio: 8125 / 1944
//  Maximum (MHz): 94.499998
//  Minimum (MHz): 80.833330
// END_OUTPUT
// OUTPUT
//  Name: CKOUT
//  Channel: 2
//  Frequency (MHz): 81.250000
//  NC_LS: 6
//  CKOUT2 to CKOUT1 Ratio: 1 / 1
//  Maximum (MHz): 94.499998
//  Minimum (MHz): 80.833330
// END_OUTPUT
// CONTROL_FIELD
//  Register-based Controls
//         FREE_RUN_EN: 0x1
//     CKOUT_ALWAYS_ON: 0x0
//          BYPASS_REG: 0x0
//           CK_PRIOR2: 0x1
//           CK_PRIOR1: 0x0
//           CKSEL_REG: 0x0
//               DHOLD: 0x0
//             SQ_ICAL: 0x1
//           BWSEL_REG: 0x3
//         AUTOSEL_REG: 0x2
//            HIST_DEL: 0x12
//               ICMOS: 0x3
//               SLEEP: 0x0
//          SFOUT2_REG: 0x5
//          SFOUT1_REG: 0x5
//           FOSREFSEL: 0x2
//              HLOG_2: 0x0
//              HLOG_1: 0x0
//            HIST_AVG: 0x18
//           DSBL2_REG: 0x0
//           DSBL1_REG: 0x0
//              PD_CK2: 0x0
//              PD_CK1: 0x0
//          FLAT_VALID: 0x1
//              FOS_EN: 0x0
//             FOS_THR: 0x1
//             VALTIME: 0x1
//               LOCKT: 0x1
//         CK2_BAD_PIN: 0x1
//         CK1_BAD_PIN: 0x1
//             LOL_PIN: 0x1
//             INT_PIN: 0x0
//          INCDEC_PIN: 0x1
//        CK1_ACTV_PIN: 0x1
//           CKSEL_PIN: 0x1
//         CK_ACTV_POL: 0x1
//          CK_BAD_POL: 0x1
//             LOL_POL: 0x1
//             INT_POL: 0x1
//            LOS2_MSK: 0x1
//            LOS1_MSK: 0x1
//            LOSX_MSK: 0x1
//            FOS2_MSK: 0x1
//            FOS1_MSK: 0x1
//             LOL_MSK: 0x1
//               N1_HS: 0x6
//              NC1_LS: 0x5
//              NC2_LS: 0x5
//               N2_LS: 0x7EF3
//               N2_HS: 0x6
//                 N31: 0x50F
//                 N32: 0x1DC2
//          CLKIN2RATE: 0x0
//          CLKIN1RATE: 0x0
//            FASTLOCK: 0x1
//             LOS1_EN: 0x3
//             LOS2_EN: 0x3
//             FOS1_EN: 0x1
//             FOS2_EN: 0x1
//    INDEPENDENTSKEW1: 0x0
//    INDEPENDENTSKEW2: 0x0
// END_CONTROL_FIELD
// REGISTER_MAP
static uint8_t regTab[] = {
  0, 0x54,
  1, 0xE4,
  2, 0x32,
  3, 0x15,
  4, 0x92,
  5, 0xED,
  6, 0x2D,
  7, 0x2A,
  8, 0x00,
  9, 0xC0,
 10, 0x00,
 11, 0x40,
 19, 0x29,
 20, 0x3E,
 21, 0xFF,
 22, 0xDF,
 23, 0x1F,
 24, 0x3F,
 25, 0xC0,
 31, 0x00,
 32, 0x00,
 33, 0x05,
 34, 0x00,
 35, 0x00,
 36, 0x05,
 40, 0xC0,
 41, 0x7E,
 42, 0xF3,
 43, 0x00,
 44, 0x05,
 45, 0x0F,
 46, 0x00,
 47, 0x1D,
 48, 0xC2,
 55, 0x00,
131, 0x1F,
132, 0x02,
137, 0x01,
138, 0x0F,
139, 0xFF,
142, 0x00,
143, 0x00,
136, 0x40 };
// END_REGISTER_MAP
// END_PROFILE

static int
si5324GetReg(int reg)
{
    uint8_t u;

    if (iicpsRecv(SI5324_ADDRESS, reg, &u, 1) != 1)
        fatal("Can't read from SI5324 register %d", reg);
    return u;
}

static int
si5324SetReg(int reg, int value)
{
    uint8_t ubuf[2];

    ubuf[0] = reg;
    ubuf[1] = value;
    if (iicpsSend(SI5324_ADDRESS, ubuf, 2) != 2)
        fatal("Can't set SI5324 register %d to 0x%02x", reg, value);
    return 0;
}

static void
setMux(void)
{
    if (iicpsSetMux(IICPS_SELECT_SI5324) < 0)
        fatal("Can't configure I2C MUX for SI5324");
}

void
si5324Init(void)
{
    int r134, r135;
    int i;

    setMux();
    r134 = si5324GetReg(134);
    r135 = si5324GetReg(135);
    if ((r134 != 0x01) || (r135 != 0x82))
        fatal("Read 0x%04X from SI5324 PARTNUM registers.  Expected 0x0182.\n",
                                                        ((r134 << 8 ) | r135));
    printf("SI5324 Configuration\n   Reg Old   New\n");
    for (i = 0 ; i < sizeof regTab / sizeof regTab[0] ; i += 2) {
        int reg = regTab[i], nVal = regTab[i+1], oVal = si5324GetReg(reg);
        if (reg == 6) nVal = 0x0F; /* We want Out2=off, Out1=LVDS */
        printf("   %3d: %02X", reg, oVal);
        if (nVal != oVal) printf(" <- %02X", nVal);
        printf("\n");
        si5324SetReg(reg, nVal);
    }
}

int
si5324IsLocked(void)
{
    int r130;

    setMux();
    r130 = si5324GetReg(130);
    if ((r130 >= 0) && ((r130 & 0x1) == 0))
        return 1;
    return 0;
}

void
si5324AwaitLock(void)
{
    int seconds = 0;
    uint32_t then, now;

    then = ticksSinceBoot();
    while (!si5324IsLocked()) {
        if (((now = ticksSinceBoot()) - then) > ticksPerSecond) {
            seconds++;
            if (seconds == 2) printf("Wait for SI5324 to lock.\n");
            if ((seconds % 60) == 0) {
                printf("SI5324 won't lock\n");
                si5324Init();
            }
            then += ticksPerSecond;
        }
    }
    printf("SI5324 locked");
    if (seconds > 1) printf(" after %d s", seconds);
    printf(".\n");
}
