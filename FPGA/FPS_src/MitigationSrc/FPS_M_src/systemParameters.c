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

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "flash.h"
#include "lwip/def.h"
#include "systemParameters.h"
#include "util.h"

struct systemParameters systemParameters;

/*
 * Serializer/deserializers
 * Note -- Format routines share common static buffer.
 */
static char cbuf[40];
char *
formatMAC(void *val)
{
    const unsigned char *addr = (const unsigned char *)val;
    sprintf(cbuf, "%02X:%02X:%02X:%02X:%02X:%02X", addr[0], addr[1], addr[2],
                                                   addr[3], addr[4], addr[5]);
    return cbuf;
}

int
parseMAC(const char *str, void *val)
{
    const char *cp = str;
    int i = 0;
    long l;
    char *endp;

    for (;;) {
        l = strtol(cp, &endp, 16);
        if ((l < 0) || (l > 255))
            return -1;
        *((unsigned char *)val + i) = l;
        if (++i == 6)
            return endp - str;
        if (*endp++ != ':')
            return -1;
        cp = endp;
    }
}

char *
formatIP(void *val)
{
    unsigned long l = ntohl(*(unsigned long *)val);
    sprintf(cbuf, "%d.%d.%d.%d", (int)(l >> 24) & 0xFF, (int)(l >> 16) & 0xFF,
                                 (int)(l >>  8) & 0xFF, (int)(l >>  0) & 0xFF);
    return cbuf;
}

int
parseIP(const char *str, void *val)
{
    const char *cp = str;
    unsigned long addr = 0;
    int i = 0;
    long l;
    char *endp;

    for (;;) {
        l = strtol(cp, &endp, 10);
        if ((l < 0) || (l > 255))
            return -1;
        addr = (addr << 8) | l;
        if (++i == 4) {
            *(unsigned long *)val = htonl(addr);
            return endp - str;
        }
        if (*endp++ != '.')
            return -1;
        cp = endp;
    }
}

static unsigned int
formChecksum(void)
{
    unsigned char *cp = (unsigned char *)&systemParameters;
    unsigned long checksum = 1;
    int i;

    for (i = 0 ; i < sizeof systemParameters ; i++)
        checksum += *cp++;
    return checksum;
}

void
systemParametersFetch(void)
{
    unsigned long checksum = formChecksum();

    if ((flashRead(FLASH_OFFSET_SYSTEM_PARAMETERS, (char *)&systemParameters,
                      sizeof systemParameters) == sizeof systemParameters)
     && (flashRead(FLASH_OFFSET_SYSTEM_PARAMETERS + sizeof systemParameters,
                      (char *)&checksum, sizeof checksum) == sizeof checksum)
     && (checksum == formChecksum())
     && (ntohl(systemParameters.ipv4.netmask) >= 0xFF000000)
     && (ntohl(systemParameters.ipv4.netmask) <= 0xFFFFFFF0)
     && ((systemParameters.ipv4.address & systemParameters.ipv4.netmask) ==
         (systemParameters.ipv4.gateway & systemParameters.ipv4.netmask))) {
        printf("System parameters from flash memory:\n");
    }
    else {
        printf("Corrupt system parameters.  Assigning default values.\n");
        systemParameters.ethernetAddress[0] = 'J'; /* Local addr bit set */
        systemParameters.ethernetAddress[1] = 'U';
        systemParameters.ethernetAddress[2] = 'N';
        systemParameters.ethernetAddress[3] = 'K';
        systemParameters.ethernetAddress[4] = '0';
        systemParameters.ethernetAddress[5] = '1';
        systemParameters.ipv4.address = htonl((192<<24) |
                                              (168<<16) |
                                              (  1<< 8) |
                                                10);
        systemParameters.ipv4.netmask = htonl((255<<24) |
                                              (255<<16) |
                                              (255<< 8) |
                                                 0);
        systemParameters.ipv4.gateway = htonl((192<<24) |
                                              (168<<16) |
                                              (  1<< 8) |
                                                 1);

    }
    printf(" ETHERNET ADDRESS: %s\n",
                                formatMAC(&systemParameters.ethernetAddress));
    printf("       IP ADDRESS: %s\n", formatIP(&systemParameters.ipv4.address));
    printf("         NET MASK: %s\n", formatIP(&systemParameters.ipv4.netmask));
    printf("          GATEWAY: %s\n", formatIP(&systemParameters.ipv4.gateway));

}

void
systemParametersStash(void)
{
    unsigned long checksum = formChecksum();
    if ((flashWrite(FLASH_OFFSET_SYSTEM_PARAMETERS, (char *)&systemParameters,
                      sizeof systemParameters) != sizeof systemParameters)
     || (flashWrite(FLASH_OFFSET_SYSTEM_PARAMETERS + sizeof systemParameters,
                      (char *)&checksum, sizeof checksum) != sizeof checksum)) {
        printf("Failed to store system parameters in flash memory!\n");
    }
}
