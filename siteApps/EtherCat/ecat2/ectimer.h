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


#ifndef ECTIMER_H
#define ECTIMER_H



typedef struct {
	long rate;
    struct timespec time;

} _ec_timer;


#define MAX_EC_TIMERS	16
/*--------------------------------- */

int tmr_init( int tmr_nr, long rate );
int tmr_wait( int tmr_nr );

/*--------------------------------- */
#define MAX_STIMERS	50

#define ECT_ECWORK_TOTAL	40
#define ECT_IRQ				41
#define ECT_RW 				42
#define ECT_STS 			43


#endif


















