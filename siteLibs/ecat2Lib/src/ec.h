/*
 *
 * (c) 2013 Dragutin Maier-Manojlovic     dragutin.maier-manojlovic@psi.ch
 * Date: 01.10.2013
 *
 * Paul Scherrer Institute (PSI)
 * Switzerland
 *
 * This file and the package it belongs to have to be redistributed as is,
 * with no changes.
 * Any changes to this file mean that further distribution is not allowed.
 * Bugs/errors/patches/feature requests can be sent directly to the author,
 * and if they are accepted, will be included in the next version or release.
 *
 * Disclaimer:
 * The software (code, tools) is provided "as is", without warranty of any kind.
 * Author makes no warranties, express or implied, that the code is free of
 * errors, or is consistent with any particular standard, or that it
 * will meet your requirements for any particular application.
 * It should not be relied on for solving a problem whose correct or incorrect
 * solution could result in injury to a person or a loss of property.
 * The author disclaim all liability for direct, indirect or
 * consequential damage resulting from your use of the code or tools.
 * If you do use it, it is at your own risk.
 *
 */



#ifndef EC_H
#define EC_H

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <ctype.h>
#include <alarm.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/signal.h>
#include <sys/errno.h>
#include <sys/syscall.h>
#include <features.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <linux/types.h>


#include <devLib.h>
#include <devSup.h>
#include <epicsThread.h>
#include <epicsMutex.h>
#include <epicsEvent.h>
#include <epicsExport.h>
#include <epicsTypes.h>
#include <dbScan.h>
#include <errlog.h>
#include <dbAccess.h>
#include <aiRecord.h>
#include <aoRecord.h>
#include <biRecord.h>
#include <boRecord.h>
#include <mbbiRecord.h>
#include <mbboRecord.h>
#include <longinRecord.h>
#include <longoutRecord.h>
#include <stringinRecord.h>
#include <stringoutRecord.h>
#include <aaiRecord.h>
#include <aaoRecord.h>
#include <initHooks.h>
#define HAVE_VME
#include <epicsTypes.h>
#include <epicsThread.h>
#include <epicsExit.h>
#include <iocsh.h>
#include <callback.h>
#include <recGbl.h>

#include <time.h>

#include "ecrt.h"

#include "eccommon.h"

#include "drvethercat.h"
#include "drvethercatint.h"

#include "ecnode.h"
#include "eccfg.h"
#include "ectimer.h"
#include "ectools.h"
#include "ecengine.h"
#include "ecdebug.h"

#endif


















