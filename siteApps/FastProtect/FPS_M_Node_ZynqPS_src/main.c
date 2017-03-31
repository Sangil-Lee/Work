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
#include <lwip/init.h>
#include <lwip/tcp_impl.h>
#include <netif/xadapter.h>
#include "aurora.h"
#include "console.h"
#include "epics.h"
#include "evr.h"
#include "eyescan.h"
#include "flash.h"
#include "iic.h"
#include "iicps.h"
#include "logCOS.h"
#include "platform.h"
#include "platform_config.h"
#include "sfp.h"
#include "si5324.h"
#include "sysmon.h"
#include "systemParameters.h"
#include "tftp.h"
#include "util.h"

/*
 * Must be global since the platform-specific code provided by Xilinx
 * needs it to be able to work around a problem in the Ethernet hardware.
 */
struct netif netif;

/*
 * Set up network
 */
static void
configureNetwork(void)
{
    struct ip_addr address, netmask, gateway;
    extern void lwip_init(void); /* No LWIP header provides this! */

    lwip_init();
    address.addr = systemParameters.ipv4.address;
    netmask.addr = systemParameters.ipv4.netmask;
    gateway.addr = systemParameters.ipv4.gateway;
    if (!xemac_add(&netif, &address, &netmask, &gateway,
                      systemParameters.ethernetAddress, PLATFORM_EMAC_BASEADDR))
        fatal("Can't add network interface");
    netif_set_default(&netif);
    platform_enable_interrupts();
    netif_set_up(&netif);
}

uint32_t then, now, diff;
int maxDiff = 0;
int
main()
{
    init_platform();
    sysmonInit();
    flashInit();
    iicpsInit();
    iicInit();
    systemParametersFetch();
    si5324Init();
    configureNetwork();
    si5324AwaitLock();
    eyescanInit();
    sfpInit();
    evrInit();
    auroraInit();
    epicsInit();
    logInit();
    auroraLogEnable();
    tftpInit();
    for (;;) {
        sfpCrank();
        logCrank();
        consoleCheck();
        xemacif_input(&netif);
    }
    return 0;
}
