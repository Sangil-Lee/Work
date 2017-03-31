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




#ifndef DRVECATINT_H
#define DRVECATINT_H

/*


------------------------------------

 Various support macros

------------------------------------
#define _DMMTP(a,b) long devethercat_##a##_##b
#define _DMMTP2(a,b) _DMMTP(a,b)
#define _DMMTP3(a,b) devethercat_##a##_##b

#define _dev_init_record(rt) _DMMTP3( init_record, rt )
#define dev_init_record(rt) _DMMTP2( init_record, rt( rt##Record *record ) )
#define _dev_read(rt) _DMMTP3( read, rt )
#define dev_read(rt) _DMMTP2( read, rt( rt##Record *record ) )
#define _dev_write(rt) _DMMTP3( write, rt )
#define dev_write(rt) _DMMTP2( write, rt( rt##Record *record ) )
#define _dev_special_linconv(rt) _DMMTP3( special_linconv, rt )
#define dev_special_linconv(rt) _DMMTP2( special_linconv, rt( rt##Record *record, int after ) )
*/

#define drvethercatNAME    "EtherCAT driver: "
#define dmmDEBUGVAR    drvethercatDebug
#define DBG_all        0x12345




/*------------------------------------*/
/*                                    */
/* devethercat                        */
/*                                    */
/*------------------------------------*/
#define MAGIC 0xbc0d7413 /* crc("PSI EtherCAT") */

typedef struct {
    long      number;
    DEVSUPFUN report;
    DEVSUPFUN init;
    DEVSUPFUN init_record;
    DEVSUPFUN get_ioint_info;
    DEVSUPFUN readwrite;
    DEVSUPFUN special_linconv;
} devsup;



#define devsupport(rectype) \
devsup devethercat##rectype = {             \
    6,                                      \
    dev_report,                             \
    dev_init,                               \
    dev_init_record,                        \
    dev_get_ioint_info,                     \
    dev_rw_##rectype,             			\
    NULL                                    \
};											\
epicsExportAddress( dset, devethercat##rectype );


#define devsupport_linconv(rectype) \
devsup devethercat##rectype = {             \
    6,                                      \
    dev_report,                             \
    dev_init,                               \
    dev_init_record,                        \
    dev_get_ioint_info,                     \
    dev_rw_##rectype,                       \
    dev_special_linconv_##rectype           \
};											\
epicsExportAddress( dset, devethercat##rectype );



typedef enum {
	RIO_ERROR = 0x00,
	RIO_READ = 0x01,
	RIO_WRITE,
	RIO_RW,

} RECIOTYPE;

typedef struct {
	const char *recname;
	RECTYPE rtype;
	RECIOTYPE riotype;
	int bitlen;
	int special_calc;
} _rectype;


#define OK	1
#define NOTOK 0
#define FAIL NOTOK
#define FAILURE FAIL
#define EPICFAIL FAILURE

#define ARRAY_CONVERT	0xdd



#define pinfo(fmt, args...) printf( fmt, ##args )
#define perr(fmt, args...)  printf( fmt, ##args )
#define pwarn(fmt, args...)   printf( fmt, ##args )
#define pdbg(fmt, args...)    printk( fmt, ##args )

#define perrret(fmt, args...) { printf( fmt, ##args ); return 0; }
#define perrret1(fmt, args...) { printf( fmt, ##args ); return 1; }
#define perrret2(fmt, args...) { printf( fmt, ##args ); return; }
#define perrex(fmt, args...) { printf( fmt, ##args ); exit(1); }


ethcat *drvFindDomain( int dnr );
int dev_tokenize( char *s, const char *delims, char **tokens );
int dev_parse_expression( char *token );

extern _rectype rectypes[];


#endif

















