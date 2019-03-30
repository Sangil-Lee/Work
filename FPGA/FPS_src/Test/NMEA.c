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
 * NMEA parsing
 */

#include <stdio.h>
#include <ctype.h>
#include "NMEA.h"

/*
 * Simplified version of timegm that works in the 21st century.
 * First argument is years past 1900 (100-199).
 * Second argument is months past January (0-11).
 * Third argument is day of month (1-31).
 */
static unsigned int
timegm(int year, int mon, int mday, int hour, int min, int sec)
{
    int yday;
    static const int daysBeforeMonth[] = 
        {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};

    yday = daysBeforeMonth[mon] + mday;
    if (((year % 4) == 0) && (mon > 1)) yday++;

    /*
     * Y2.1k issue
     * Full expression would subtract
     * ((year-1)/100)*86400 + ((year+299)/400)*86400
     * but since we only have a two digit year (and hence a value
     * of tm_year greater or equal to 100 and less than 200) the
     * extra terms cancel each other out.
     * Works past the end of (signed) 32-bit POSIX time by
     * forcing unsigned arithmetic.
     */
    return sec + min*60 + hour*3600 + yday*86400 +
                                    (year-70)*31536000U + ((year-69)/4)*86400U;
}

static int
hexval(char c)
{
    if ((c >= '0') && (c <= '9')) return c - '0';
    if ((c >= 'A') && (c <= 'F')) return c - 'A' + 10;
    if ((c >= 'a') && (c <= 'f')) return c - 'a' + 10;
    NMEAerror("Bad hexadecimal character");
    return -1;
}

static int
decval(char c)
{
    if ((c >= '0') && (c <= '9')) return c - '0';
    return -1;
}

static void
parse(const char *sentence)
{
    const char *cp = sentence;
    const char *fmt = {"$GPRMC,%2d%2d%2d%f,A,,,,,,,%2d%2d%2d"};
    int v;
    int value;
    int width;
    int vIdx = 0;
    int vBuf[7]; /* hh, mm, ss, ms, DD, MM, YY */

    while (*fmt) {
        if (*cp == '\0') {
            NMEAerror("Sentence too short");
            return;
        }
        switch (*fmt) {
        case '%':
            value = 0;
            fmt++;
            if ((width = decval(*fmt)) > 0) {
                fmt++;
            }
            else if (*fmt == 'f') {
                if (*cp == '.') cp++;
                width = -1;
            }
            else if (*fmt !='d') {
                NMEAerror("Mangled format!");
                return;
            }
            fmt++;
            for (;;) {
                v = decval(*cp);
                if (v < 0) {
                    if (width < 0) break;
                    NMEAerror("Bad digit");
                    return;
                }
                value = (value * 10) + v;
                cp++;
                if ((width > 0) && (--width == 0))
                    break;
            }
            vBuf[vIdx++] = value;
            break;

        case ',':
            if (*cp++ == ',') {
                fmt++;
            }
            break;

        default:
            if (*cp++ != *fmt++) fmt = "";
            break;
        }
    }
    NMEAcallback(sentence);
    if (vIdx == 7) {
        NMEAtime(timegm(vBuf[6]+100, vBuf[5]-1, vBuf[4]-1,
                                           vBuf[0], vBuf[1], vBuf[2]), vBuf[3]);
    }
}

void
NMEAconsume(int c)
{
    int v;
    static int chksum;
    static int val;
    static enum { stIdle, stStart, stPayload, stChkHi, stChkLo } state = stIdle;
    static char sentence[90];
    static int sIdx;

    if (c == '$') {
        if (state != stIdle) NMEAerror("Unexpected $");
        sIdx = 0;
        state = stStart;
    }
    if (state != stIdle) {
        if (sIdx >= (sizeof(sentence)-1)) {
            NMEAerror("Sentence too long");
            state = stIdle;
            return;
        }
        sentence[sIdx++] = c;
    }
    switch (state) {
    case stIdle:
        break;

    case stStart:
        chksum = 0;
        state = stPayload;
        break;

    case stPayload:
        if (c == '*') {
            state = stChkHi;
        }
        else {
            chksum ^= c;
        }
        break;

    case stChkHi:
        if ((v = hexval(c)) < 0) {
            state = stIdle;
        }
        else {
            val = v << 4;
            state = stChkLo;
        }
        break;

    case stChkLo:
        sentence[sIdx] = '\0';
        if ((v = hexval(c)) >= 0) {
            if ((val | v) == chksum) {
                parse(sentence);
            }
            else {
                NMEAerror("Checksum mismatch");
            }
        }
        state = stIdle;
        break;
    }
}
