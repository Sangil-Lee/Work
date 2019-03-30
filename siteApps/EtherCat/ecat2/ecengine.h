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


#ifndef ECENGINE_H
#define ECENGINE_H


/*--------------------------------- */
void ec_worker_thread( void *data );
void ec_irq_thread( void *data );

void ec_shc_thread( void *data );
void set_master_health( ethcat *ec, int health );
int get_master_health( ethcat *ec );
void set_master_link_up( ethcat *ec, int link_up );
int get_master_link_up( ethcat *ec );
void set_slave_health( ethcat *ec, ecnode *s, int health );
int get_slave_health( ethcat *ec, ecnode *s );
void set_slave_aggregate_health( ethcat *ec, int health );
int get_slave_aggregate_health( ethcat *ec );

EC_ERR drvGetSysRecData( ethcat *e, system_rec_data *sysrecdata, dbCommon *record, epicsUInt32 *val );
int drvGetBlock(
		ethcat *e,
		char *buf,
		int offs,
		int len
);
int drvSetBlock(
		ethcat *e,
		char *buf,
		int offs,
		int len
);

/*--------------------------------- */



#define IOCTL_MSG_DREC _IOWR(PSI_ECAT_VERSION_MAGIC, 100, int)


#define S_INIT 		0x01
#define S_PREOP		0x02
#define S_SAFEOP 	0x04
#define S_OP 		0x08

#endif


















