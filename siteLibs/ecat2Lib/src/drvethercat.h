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



#ifndef DRVETHERCAT_H
#define DRVETHERCAT_H


#define MAXSTR	(256+1)


/*----------------------------------------*/
/*                                        */
/* drvethercat                            */
/*                                        */
/*----------------------------------------*/



typedef struct {
	int dnr;
	char *rmem;
	char *wmem;

	int dreg_nr;
	domain_register *dreg_info;

} ioctl_trans;



/*------------------------------------------------------ */
#define EC_MAX_NUM_DEVICES 1
#define EC_DATAGRAM_NAME_SIZE 20
/** Size of the EtherCAT address field. */
#define EC_ADDR_LEN 4
#define EC_MAX_DOMAINS	10

/*------------------------------------------------------ */

void process_hooks( initHookState state );


ethcat *ethercatOpen( int domain_nr );

int drvGetRegisterDesc( ethcat *e, domain_register *dreg, int regnr, ecnode **pentry, int *token_num );
int drvGetLocalRegisterDesc( ethcat *e, domain_register *dreg, int *dreg_nr, ecnode **pentry, int *token_num );
int drvGetEntryDesc( ethcat *e, domain_register *dreg, int *dreg_nr, ecnode **pentry, int *token_num );
int drvDomainExists( int mnr, int dnr );

int drvGetValue( ethcat *e, int offs, int bit, epicsUInt32 *rval, int bitlen, int bitspec, int byteoffs, int bytelen );
int drvSetValue( ethcat *e, int offs, int bit, epicsUInt32 *rval, int bitlen, int bitspec, int byteoffs, int bytelen );

int drvGetValueMasked( ethcat *e, int offs, int bit, epicsUInt32 *val, int bitlen, epicsInt16 nobt, epicsUInt16 shift, epicsUInt32 mask, int byteoffs, int bytelen );
int drvSetValueMasked( ethcat *e, int offs, int bit, epicsUInt32 *val, int bitlen, epicsInt16 nobt, epicsUInt16 shift, epicsUInt32 mask, int byteoffs, int bytelen );

int drvGetValueString( ethcat *e, int offs, int bitlen, char *val, char *oval, int byteoffs, int bytelen );
int drvSetValueString( ethcat *e, int offs, int bitlen, char *val, char *oval, int byteoffs, int bytelen );

int drvGetValueFloat(
		ethcat *e,
		int offs,
		int bit,
		epicsUInt32 *val,
		int bitlen,
		int bitspec,
		int byteoffs,
		int bytelen,
		epicsType etype,
		double *fval
 );
int drvSetValueFloat(
		ethcat *e,
		int offs,
		int bit,
		epicsUInt32 *val,
		int bitlen,
		int bitspec,
		int byteoffs,
		int bytelen,
		epicsType etype,
		double *fval
 );

int parse_datatype_get_len( epicsType etype );

extern long long wt_counter[EC_MAX_DOMAINS];
extern long long delayed[EC_MAX_DOMAINS];
extern long long recd[EC_MAX_DOMAINS];
extern long long forwarded[EC_MAX_DOMAINS];
extern long long irqs_executed[EC_MAX_DOMAINS];
extern long long delayctr_cumulative[EC_MAX_DOMAINS];
extern long long dropped[EC_MAX_DOMAINS];

#endif


















