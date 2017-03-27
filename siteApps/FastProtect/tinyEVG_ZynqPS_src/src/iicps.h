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

#ifndef _XIICPS_H_
#define _XIICPS_H_

/*
 * Communicate with I2C bus devices connected to the processor
 */

#include <stdint.h>

#define IICPS_SELECT_PMBUS                  0x80
#define IICPS_SELECT_LPC                    0x40
#define IICPS_SELECT_HPC                    0x20
#define IICPS_SELECT_SI5324                 0x10
#define IICPS_SELECT_PORT_EXPANDER_AND_DDR3 0x08
#define IICPS_SELECT_EEPROM                 0x04
#define IICPS_SELECT_HDMI                   0x02
#define IICPS_SELECT_SI570                  0x01

int iicpsInit(void);
int iicpsSetMux(int bitmap);
int iicpsSend(int i2cAaddress, const uint8_t *buf, int len);
int iicpsRecv(int i2cAaddress, int offset, uint8_t *buf, int len);

#endif /* _XIICPS_H */
