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

#include "ec.h"

/*---------------------------------------------------------------------------------------------- */



typedef struct _dev_ethercat_private {
	ethcat *e;

	system_rec_data sysrecdata;

	int dreg_nr;
	domain_register dreg_info;

	ecnode *pe;

	/* aai & aao */
	int ftvl_len;
	int ftvl_type;

} devethercat_private;


_rectype rectypes[] = {
		{ "ai", 		REC_AI, 			RIO_READ,  sizeof(epicsUInt32) },
		{ "aai", 		REC_AAI, 			RIO_READ,  sizeof(epicsUInt32) },
		{ "bi", 		REC_BI, 			RIO_READ,  1 },
		{ "mbbi", 		REC_MBBI, 			RIO_READ,  sizeof(epicsUInt32) },
		{ "mbbiDirect", REC_MBBIDIRECT, 	RIO_READ,  sizeof(epicsUInt32) },
		{ "longin", 	REC_LONGIN, 		RIO_READ,  sizeof(epicsUInt32) },
		{ "stringin", 	REC_STRINGIN, 		RIO_READ,  sizeof(epicsUInt8) },
		{ "ao", 		REC_AO, 			RIO_WRITE, sizeof(epicsUInt32) },
		{ "aao", 		REC_AAO, 			RIO_WRITE, sizeof(epicsUInt32) },
		{ "bo", 		REC_BO, 			RIO_WRITE, 1 },
		{ "mbbo", 		REC_MBBO, 			RIO_WRITE, sizeof(epicsUInt32) },
		{ "mbboDirect", REC_MBBODIRECT, 	RIO_WRITE, sizeof(epicsUInt32) },
		{ "longout", 	REC_LONGOUT, 		RIO_WRITE, sizeof(epicsUInt32) },
		{ "stringout", 	REC_STRINGOUT, 		RIO_WRITE, sizeof(epicsUInt8) },

		{ NULL }
};
static RECTYPE dev_get_record_type( dbCommon *prec, int *ix )
{
	FN_CALLED;

	if( !prec->rdes->name )
		return REC_ERROR;

	for( *ix = 0; rectypes[*ix].recname; (*ix)++ )
		if( !strcmp( rectypes[*ix].recname, prec->rdes->name ) )
			return rectypes[*ix].rtype;

	return REC_ERROR;
}

static int dev_get_record_bitlen( dbCommon *prec )
{
	FN_CALLED;
	int i;
	devethercat_private *p = (devethercat_private *)prec->dpvt;

	if( !prec->rdes->name )
		return REC_ERROR;

	for( i = 0; rectypes[i].recname; i++ )
		if( !strcmp( rectypes[i].recname, prec->rdes->name ) )
		{
			switch( rectypes[i].rtype )
			{
				case REC_AAI:		return rectypes[i].bitlen * ((aaiRecord *)prec)->nelm;
				case REC_AAO:		return rectypes[i].bitlen * ((aaoRecord *)prec)->nelm;
				case REC_STRINGIN:
				case REC_STRINGOUT:
									return p->dreg_info.bytelen * 8;

				default:
									break;
			}
			return rectypes[i].bitlen;
		}
	return 0;
}




/*-------------------------------------------------------------------------------------*/
/*                                                                                     */
/* devethercat device support                                                          */
/*                                                                                     */
/*-------------------------------------------------------------------------------------*/

long dev_report( int interest )
{
	FN_CALLED;

	return 0;
}


static long dev_init( int phase )
{
	FN_CALLED;

	if( phase == 0 )
		initHookRegister( &process_hooks );
	return 0;
}



static long dev_get_ioint_info( int dir, dbCommon *prec, IOSCANPVT *io)
{
	ethcat *e;
	int i, ix, rec_bitlen;
	RECTYPE retv;
	devethercat_private *p;
	static int irq_rec = 0;

	FN_CALLED;

	if( !prec->dpvt )
	{
		errlogSevPrintf( errlogFatal, "%s: record %s not initialized properly (1)\n", __func__, prec->name );
		return S_dev_badArgument;
	}

	e = (p = (devethercat_private *)prec->dpvt)->e;
	if( !e )
	{
		errlogSevPrintf( errlogFatal, "%s: record %s not initialized properly (2)\n", __func__, prec->name );
		return S_dev_badArgument;
	}

	retv = dev_get_record_type( prec, &ix );
	if( retv == REC_ERROR )
		return S_dev_badArgument;

	if( !p->sysrecdata.system )
	{
		/* system health records get special attention */
		rec_bitlen = dev_get_record_bitlen( prec );
		if( rec_bitlen < 1 )
			return S_dev_badArgument;

		if( rec_bitlen < 8)
			for( i = 0; i < p->dreg_info.bitlen; i++ )
				*(e->irq_r_mask + p->dreg_info.offs) |= (0x01 << (p->dreg_info.bit+i));
		else
			memset( e->irq_r_mask + p->dreg_info.offs, 0xff, rec_bitlen / 8 );

		printf( "Record %s registered as I/O Intr, offset %04x, ", prec->name, p->dreg_info.offs );
		if( rec_bitlen < 8)
			printf( "bit %d, bitlen %d", p->dreg_info.bit, rec_bitlen );
		else
			printf( "bytes %d", rec_bitlen / 8 );
	}
	else
		printf( "Record %s registered as I/O Intr (EtherCAT system record)", prec->name );

	switch( rectypes[ix].riotype )
	{
		case RIO_READ: 	*io = e->r_scan; break;
		case RIO_WRITE:	*io = e->w_scan; break;
		default:		return S_dev_success; /* to supress gcc warning */
	}

	printf( ", %d I/O Intr records total so far\n", ++irq_rec );

 	return S_dev_success;
}



/*----------------------------------------------------------*/
/*                                                          */
/* init_record                                              */
/*                                                          */
/*----------------------------------------------------------*/
int dev_trim_whitespaces( char *out, int len, const char *str )
{
	const char *end;
	int out_size;
	FN_CALLED;

	if( len == 0 )
		return FAIL;

	while( isspace(*str) )str++;

	if( !*str == 0 )
	{
		*out = 0;
		return OK;
	}

	end = str + strlen(str) - 1;
	while( end > str && isspace(*end) )
		end--;
	end++;

	out_size = (end - str) < len-1 ? (end - str) : len-1;

	memcpy( out, str, out_size );
	out[out_size] = 0;

	return OK;
}


int dev_tokenize( char *s, const char *delims, char **tokens )
{
	char *p, *r;
	int i, ix = 0;
	FN_CALLED;

	r = p = s;
	for( i = 0, p = strsep( &r, delims ); p && ix < EPT_MAX_TOKENS-1; i++, (p = strsep( &r, delims)) )
		if( p[0] )
			tokens[ix++] = p;
	tokens[ix] = NULL;

	return ix;
}

int dev_parse_expression( char *token )
{
	int num = 0, mul = 1;
	char *tail, *s;
	FN_CALLED;

	if( !token )
		return -1;

	if( !isdigit( token[2] ) )
		return -1;

	s = token+2;
	while( s[0] )
	{
		tail = NULL;
		num = num + mul*(int)strtol( s, &tail, 10 );

		s = tail;
		if( s )
		{
			if( s[0] == ')' )
				break;
		}
		else
			break;
		if( s[0] == '+' )
			mul = 1;
		else if( s[0] == '-' )
			mul = -1;
		else if( !isspace(s[0]) )
			return -1;
		s++;
	}

	return num;
}

enum KEYWORDS {
	PK_M_STATUS = 0,
	PK_S_STATUS,
	PK_L_STATUS,
	PK_S_OP_STATUS,
};
static char *parse_system_keywords[] = {
		"STATUS_MASTER",
		"STATUS_ALL_SLAVES",
		"STATUS_LINK",
		"STATUS_SLAVE_OP",

		NULL
};


static struct { char* name; unsigned short dlen; epicsType type;} datatypes[] = {
	{ "short",      2, epicsInt16T   },
	{ "int16",      2, epicsInt16T   },

	{ "int8",       1, epicsInt8T    },

	{ "char",       1, epicsUInt8T   },
	{ "byte",       1, epicsUInt8T   },
	{ "uint8",      1, epicsUInt8T   },
	{ "unsign8",    1, epicsUInt8T   },
	{ "unsigned8",  1, epicsUInt8T   },

	{ "word",       2, epicsUInt16T  },
	{ "uint16",     2, epicsUInt16T  },
	{ "unsign16",   2, epicsUInt16T  },
	{ "unsigned16", 2, epicsUInt16T  },

	{ "long",       4, epicsInt32T   },
	{ "int32",      4, epicsInt32T   },
	{ "uint32",      4, epicsInt32T   },

	{ "dword",      4, epicsUInt32T  },
	{ "uint32",     4, epicsUInt32T  },
	{ "unsign32",   4, epicsUInt32T  },
	{ "unsigned32", 4, epicsUInt32T  },

	{ "double",     8, epicsFloat64T },
	{ "real64",     8, epicsFloat64T },
	{ "float64",    8, epicsFloat64T },

	{ "single",     4, epicsFloat32T },
	{ "real32",     4, epicsFloat32T },
	{ "float32",    4, epicsFloat32T },
	{ "float",      4, epicsFloat32T },

	{ "string",     0, epicsStringT  },

/*
	{ "qword",      8, regDev64T,    },
	{ "int64",      8, regDev64T     },
	{ "uint64",     8, regDev64T     },
	{ "unsign64",   8, regDev64T     },
	{ "unsigned64", 8, regDev64T     },

	{ "bcd8",       1, regDevBCD8T   },
	{ "bcd16",      2, regDevBCD16T  },
	{ "bcd32",      4, regDevBCD32T  },
	{ "bcd",        1, regDevBCD8T   },
	{ "time",       1, regDevBCD8T   },  for backward compatibility
*/

	{ NULL }
};

int parse_datatype_get_len( epicsType etype )
{
	int ix = -1;

	while( datatypes[++ix].name )
		if( datatypes[ix].type == etype )
			return datatypes[ix].dlen;

	return 0;
}



static int parse_datatype_get_type( char *token, char **name )
{
	int ix = -1, len = strlen( token );

	if( !len || len < 3 )
	{
		errlogSevPrintf( errlogFatal, "%s: record type '%s' invalid\n",
												__func__, token );
		return -1;
	}
	strtolower( token );

	while( datatypes[++ix].name )
		if( !strcmp( token+1, datatypes[ix].name ) ||
				!strcmp( token+2, datatypes[ix].name ) ) /* to allow both Ttype and T=type for regDev compatibility */
		{
			if( name )
				*name = datatypes[ix].name;
			return (int)datatypes[ix].type;
		}

	return -1;
}



int parse_str( char *s, ethcat **e, ecnode **pe, int *dreg_nr, domain_register *dreg, system_rec_data *srdata  )
{
	char *iotext = strdupa(s), *tail, *tokens[EPT_MAX_TOKENS] = { NULL }, *delims = " .\t@";
	int i, ix, found = 0, ntokens, retv = NOTOK, num, token_num[EPT_MAX_TOKENS];


	FN_CALLED;

	if( !(ntokens = dev_tokenize( iotext, delims, tokens )) )
	{
		errlogSevPrintf( errlogFatal, "%s: INP/OUT string invalid\n", __func__ );
		return S_dev_badArgument;
	}

	/* Possible INP/OUT links: */
	/* */
	/* dDaa.Rbb[.Bcc]              : domain register nn, optional bit cc */
	/* Saa.LRbb                    : slave aa, local register bb */
	/* Saa.SMbb.Pcc.Edd            : pdo entry a.b.c.d */
	/* Saa.SMbb.Pcc.Edd.Bee        : pdo entry a.b.c.d, optional bit ee */
	/* Saa.SMbb.Pcc.Edd.Lnn        : pdo entry a.b.c.d, optional string len nn */
	/* Saa.SMbb.Pcc.Edd.Oee[.Lnn]  : pdo entry a.b.c.d, optional rel. offset ee, optional string len nn */
	/* */
	/* Extensions: */
	/* */
	/* T<type>                     : */
	/*                        8-bit: int8, uint8, char, uchar, byte ubyte */
	/*                       16-bit: int16, uint16, word, uword, short, ushort, i16, ui16 */
	/*                       32-bit: int, uint, int32, uint32, i32, ui32, dword, udword */
	/*                       64-bit: long, ulong, int64, uint64, qword, uqword */
	/*               (float) 32-bit: float, single, real32, float32 */
	/*               (float) 64-bit: double, real64, float64 */
	/*                  (bcd) 8-bit: bcd8, bcd */
	/*                 (bcd) 16-bit: bcd16 */
	/*                 (bcd) 32-bit: bcd32 */
	/*                 (time) 8-bit: time (regDev backward compatibility) */
	/* */
	/* System: */
	/* */
	/* [Mnn] M_STATUS  			: Master nn health */
	/* [Mnn] S_STATUS  			: Aggregation of health indicators of all slaves */
	/* [Mnn] L_STATUS  			: Master nn link-up status */
	/* [Mnn] S_OP_STATUS Smm 	: OP status for a slave */
	/* */
	for( i = 0; i < EPT_MAX_TOKENS; i++ )
		token_num[i] = -1;
	for( i = 0; i < ntokens; i++ )
	{
		if( !tokens[i] )
			continue;

		strtoupper( tokens[i] );

		num = -1;
		if( isdigit(tokens[i][1]) )
			num = (int)strtol( tokens[i]+1, &tail, 10 );
		else if( strlen(tokens[i]) > 3 && tokens[i][1] == '(' && tokens[i][strlen(tokens[i])-1] == ')')
			num = dev_parse_expression( tokens[i] );

		ix = -1;
		found = 0;
		while( parse_system_keywords[++ix] )
			if( !strcmp( parse_system_keywords[ix], tokens[i] ) )
			{
				found = 1;
				break;
			}

		if( found )
		{
			if( !srdata )
			{
				errlogSevPrintf( errlogFatal, "%s: System keywords are usable only in record INP/OUT strings\n",
														__func__ );
				return ERR_BAD_ARGUMENT;
			}
			srdata->sysrectype = SRT_ERROR;
			switch( ix )
			{
				case PK_M_STATUS:		srdata->sysrectype = SRT_M_STATUS;			break;
				case PK_S_STATUS:		srdata->sysrectype = SRT_S_STATUS;			break;
				case PK_L_STATUS:		srdata->sysrectype = SRT_L_STATUS;		break;
				case PK_S_OP_STATUS:	srdata->sysrectype = SRT_S_OP_STATUS;		break;
				default:	return ERR_BAD_ARGUMENT;
			}
		}
		else
			switch( toupper(tokens[i][0]) )
			{
				case 'D':	token_num[D_NUM] = num; break;
				case 'R':	token_num[R_NUM] = num; break;
				case 'S':	if( toupper(tokens[i][1]) == 'M' )
							{
								token_num[SM_NUM] = (int)strtol( tokens[i]+2, &tail, 10 );
								break;
							}
							token_num[S_NUM] = num; break;
							break;
				case 'P':	token_num[P_NUM] = num; break;
				case 'E':	token_num[E_NUM] = num; break;
				case 'B':	token_num[B_NUM] = num; break;
				case 'L':   if( toupper(tokens[i][1]) == 'R' )
							{
								token_num[LR_NUM] = (int)strtol( tokens[i]+2, &tail, 10 );
								break;
							}
							token_num[L_NUM] = num;
							break;
				case 'O':   token_num[O_NUM] = num; break;
				case 'M':   token_num[M_NUM] = num; break;

				case 'T':   token_num[T_NUM] = parse_datatype_get_type( tokens[i], &dreg->typename );
							break;

				default:	return ERR_BAD_ARGUMENT;

			}


	}

	if( token_num[M_NUM] < 0 )
		token_num[M_NUM] = 0;

	if( token_num[D_NUM] < 0 )
		token_num[D_NUM] = 0;

	/* type has priority over length */
	if( token_num[T_NUM] > -1 )
		token_num[L_NUM] = -1;

    *e = ethercatOpen( token_num[D_NUM] );
    if( *e == NULL )
    {
        errlogSevPrintf( errlogFatal, "%s: cannot open domain %d\n", __func__, token_num[D_NUM] );
        return ERR_BAD_ARGUMENT;
    }

	/* check to see what kind of INP/OUT info was discovered */

	if( (srdata->system = (srdata->sysrectype != SRT_ERROR)) )
	{
		srdata->master_nr = token_num[M_NUM];
		srdata->nr = token_num[S_NUM];
		retv = OK;
	}
    else if( token_num[S_NUM] >= 0 && token_num[LR_NUM] >= 0 )
    {
		if( drvGetLocalRegisterDesc( *e, dreg, dreg_nr, pe, token_num) != OK )
		{
			errlogSevPrintf( errlogFatal, "%s: INP/OUT string error (%s), slave %d, local register %d not found\n",
													__func__, iotext, token_num[S_NUM], token_num[LR_NUM] );
			return ERR_BAD_ARGUMENT;
		}
    	retv = OK;
	}
    else if( token_num[R_NUM] >= 0 )
    {
    	if( drvGetRegisterDesc( *e, dreg, *dreg_nr = token_num[R_NUM], pe, token_num) != OK )
    	{
    		errlogSevPrintf( errlogFatal, "%s: INP/OUT string error (%s), domain %d, register %d not found\n",
    												__func__, iotext, token_num[D_NUM], token_num[R_NUM] );
    		return ERR_BAD_ARGUMENT;
    	}

    	retv = OK;
    }
    else if( token_num[S_NUM] >= 0 && token_num[SM_NUM] >= 0 && token_num[P_NUM] >= 0 && token_num[E_NUM] >=0 )
    {
    	if( drvGetEntryDesc( *e, dreg, dreg_nr, pe, token_num ) != OK )
    	{

			errlogSevPrintf( errlogFatal, "%s: INP/OUT string error (%s) in domain %d\n", __func__, iotext, token_num[D_NUM] );
			return ERR_BAD_ARGUMENT;
	    }
    	retv = OK;
    }
    else
	{
		errlogSevPrintf( errlogFatal, "%s: INP/OUT string error (%s), domain %d, entry link not valid or incomplete\n",
												__func__, iotext, token_num[D_NUM] );
		return ERR_BAD_ARGUMENT;
	}

    return retv;
}


int dev_parse_io_string( devethercat_private *priv, dbCommon *record, RECTYPE rectype, DBLINK *reclink )
{
	FN_CALLED;
	int ix;

	FN_CALLED;
	dev_get_record_type( record, &ix );

	if( !record || !reclink || rectype == REC_ERROR )
	{
		errlogSevPrintf( errlogFatal, "%s: record, reclink or rectype invalid\n", __func__ );
		return S_dev_badArgument;
	}

	if( !reclink->text )
	{
		errlogSevPrintf( errlogFatal, "%s: INP/OUT string NULL\n", __func__ );
		return S_dev_badArgument;
	}

	if( !strlen(reclink->text) )
	{
		errlogSevPrintf( errlogFatal, "%s: INP/OUT string empty\n", __func__ );
		return S_dev_badArgument;
	}


	return parse_str( reclink->text, &priv->e, &priv->pe, &priv->dreg_nr, &priv->dreg_info,
						&priv->sysrecdata
						);
}

#define MAX_INIT_FAILED_MSG	5
#define CHECK_RECINIT \
static int __init1_failed = 0; \
static int __init2_failed = 0; \
if( !priv )                                                                                                \
{                                                                                                          \
    recGblSetSevr( record, UDF_ALARM, INVALID_ALARM );                                                     \
    if( __init1_failed++ < MAX_INIT_FAILED_MSG ) \
    	errlogSevPrintf(errlogFatal, "%s %s: record not initialized correctly (1), message repeated: %d of %d times\n", \
    			__func__, record->name, __init1_failed, MAX_INIT_FAILED_MSG );    \
    return -1;                                                                                             \
}\
if( !priv->e )                                                                                                \
{                                                                                                          \
    recGblSetSevr( record, UDF_ALARM, INVALID_ALARM );                                                     \
    if( __init2_failed++ < MAX_INIT_FAILED_MSG ) \
    	errlogSevPrintf(errlogFatal, "%s %s: record not initialized correctly (2), message repeated: %d of %d times\n", \
    		 	__func__, record->name, __init2_failed, MAX_INIT_FAILED_MSG );    \
    return -1;                                                                                             \
}

#define CHECK_STATUS                                                                                                 \
if( status )                                                                                                         \
{                                                                                                                    \
    if( __init1_failed++ < MAX_INIT_FAILED_MSG ) \
	    errlogSevPrintf(errlogFatal, "%s failed for record %s: error code 0x%x\n", __func__, record->name, status );     \
    recGblSetSevr(record, READ_ALARM, INVALID_ALARM );                                                               \
}       \
else    \
	record->udf = 0; \
	\

#define NO_SYSTEM_RECORD																						\
if( priv->sysrecdata.system )																					\
{                                                                                                               \
    if( __init1_failed++ < MAX_INIT_FAILED_MSG ) \
		errlogSevPrintf( errlogFatal, "%s: record %s cannot be used as system record\n", __func__, record->name );  \
    recGblSetSevr( record, UDF_ALARM, INVALID_ALARM );                                                          \
	return -1;                                                                                                  \
}



/*---------------------------------------------------------- */
/*                                                           */
/* ai                                                        */
/*                                                           */
/*---------------------------------------------------------- */

long dev_rw_ai( aiRecord *record )
{
   	int status = 0;
   	devethercat_private  *priv = (devethercat_private *)record->dpvt;
	FN_CALLED;

   	CHECK_RECINIT;
   	if( priv->dreg_info.typespec == epicsFloat32T ||
   			priv->dreg_info.typespec == epicsFloat64T )
   	{
   	    status = drvGetValueFloat( priv->e, priv->dreg_info.offs, priv->dreg_info.bit, (epicsUInt32 *)&(record->rval),
									priv->dreg_info.bitlen, priv->dreg_info.bitspec,
									priv->dreg_info.byteoffs, priv->dreg_info.bytelen,
									priv->dreg_info.typespec, (double*)(&record->val) );
   	    if( !status )
   	    {
   	    	record->udf = 0;
   	    	return 2; /* Jedi mind trick: tell EPICS not to perform conversion */
   	    }
   	}
   	else
	    status = drvGetValue( priv->e, priv->dreg_info.offs,
	    							priv->dreg_info.bit, (epicsUInt32 *)&(record->rval),
    								priv->dreg_info.bitlen, priv->dreg_info.bitspec,
    								priv->dreg_info.byteoffs, priv->dreg_info.bytelen );
    CHECK_STATUS;

    return status;
}


long dev_special_linconv_ai( aiRecord *record, int after )
{
	FN_CALLED;

	if( after )
    {
        record->eslo = (record->eguf - record->egul)/(epicsFloat64)0xffffff;
        record->eoff = record->egul;
    }

#if 0
    printf( "---------------------------------------\n");
    printf( "eslo=%.10f, eoff=%.10f, eguf=%.10f, egul=%16.10f\n",
                record->eslo, record->eoff, record->eguf, record->egul );
    printf( "---------------------------------------\n");
#endif
    return 0;
}




/*---------------------------------------------------------- */
/*                                                           */
/* ao                                                        */
/*                                                           */
/*---------------------------------------------------------- */

long dev_rw_ao( aoRecord *record )
{
   	int status = 0;
   	devethercat_private  *priv = (devethercat_private *)record->dpvt;
	FN_CALLED;

  	CHECK_RECINIT;
  	NO_SYSTEM_RECORD;

  	if( priv->dreg_info.typespec == epicsFloat32T ||
   			priv->dreg_info.typespec == epicsFloat64T )
  	    status = drvSetValueFloat( priv->e, priv->dreg_info.offs, priv->dreg_info.bit,
  	    							(epicsUInt32 *)(&record->rval), priv->dreg_info.bitlen, priv->dreg_info.bitspec, priv->dreg_info.byteoffs, priv->dreg_info.bytelen,
  	    							priv->dreg_info.typespec, &record->val);
  	else
    	status = drvSetValue( priv->e, priv->dreg_info.offs, priv->dreg_info.bit,
    							(epicsUInt32 *)(&record->rval), priv->dreg_info.bitlen, priv->dreg_info.bitspec, priv->dreg_info.byteoffs, priv->dreg_info.bytelen );
    CHECK_STATUS;

    return status;
}


long dev_special_linconv_ao( aoRecord* record, int after )
{
	FN_CALLED;

	if( after )
    {
        record->eslo = (record->eguf - record->egul)/(epicsFloat64)0xffffff;
        record->eoff = record->egul;
    }

#if 0
    printf( "---------------------------------------\n");
    printf( "eslo=%.10f, eoff=%.10f, eguf=%.10f, egul=%16.10f\n",
                record->eslo, record->eoff, record->eguf, record->egul );
    printf( "---------------------------------------\n");
#endif

    return 0;
}



/*---------------------------------------------------------- */
/*                                                           */
/* bi                                                        */
/*                                                           */
/*---------------------------------------------------------- */

long dev_rw_bi( biRecord *record )
{
   	int status = 0;
   	devethercat_private  *priv = (devethercat_private *)record->dpvt;
	FN_CALLED;
   	CHECK_RECINIT;

	if( priv->sysrecdata.system )
   		status = drvGetSysRecData( priv->e, &priv->sysrecdata, (dbCommon *)record, &record->rval );
	else
    	status = drvGetValue( priv->e, priv->dreg_info.offs, priv->dreg_info.bit, &record->rval,
    													priv->dreg_info.bitlen, priv->dreg_info.bitspec, priv->dreg_info.byteoffs, priv->dreg_info.bytelen );
    CHECK_STATUS;

    return status;
}



/*---------------------------------------------------------- */
/*                                                           */
/* bo                                                        */
/*                                                           */
/*---------------------------------------------------------- */

long dev_rw_bo( boRecord *record )
{
   	int status = 0;
   	devethercat_private  *priv = (devethercat_private *)record->dpvt;
	FN_CALLED;

   	CHECK_RECINIT;
  	NO_SYSTEM_RECORD;

    status = drvSetValue( priv->e, priv->dreg_info.offs, priv->dreg_info.bit, &record->rval,
    													priv->dreg_info.bitlen, priv->dreg_info.bitspec, priv->dreg_info.byteoffs, priv->dreg_info.bytelen );
    CHECK_STATUS;

    return status;
}


/*---------------------------------------------------------- */
/*                                                           */
/* mbbi                                                      */
/*                                                           */
/*---------------------------------------------------------- */

long dev_rw_mbbi( mbbiRecord *record )
{
   	int status = 0;
   	devethercat_private  *priv = (devethercat_private *)record->dpvt;
	FN_CALLED;

   	CHECK_RECINIT;
  	NO_SYSTEM_RECORD;

    status = drvGetValueMasked( priv->e, priv->dreg_info.offs, priv->dreg_info.bit, &record->rval, priv->dreg_info.bitlen,
    							priv->dreg_info.nobt, priv->dreg_info.shft, priv->dreg_info.mask, priv->dreg_info.byteoffs, priv->dreg_info.bytelen );
    CHECK_STATUS;

    return status;
}


/*---------------------------------------------------------- */
/*                                                           */
/* mbbo                                                      */
/*                                                           */
/*---------------------------------------------------------- */

long dev_rw_mbbo( mbboRecord *record )
{
   	int status = 0;
   	devethercat_private  *priv = (devethercat_private *)record->dpvt;
	FN_CALLED;

   	CHECK_RECINIT;
  	NO_SYSTEM_RECORD;

    status = drvSetValueMasked( priv->e, priv->dreg_info.offs, priv->dreg_info.bit, &record->rval, priv->dreg_info.bitlen,
    							priv->dreg_info.nobt, priv->dreg_info.shft, priv->dreg_info.mask, priv->dreg_info.byteoffs, priv->dreg_info.bytelen );
    CHECK_STATUS;

    return status;
}



/*---------------------------------------------------------- */
/*                                                           */
/* longin                                                    */
/*                                                           */
/*---------------------------------------------------------- */

long dev_rw_longin( longinRecord *record )
{
   	int status = 0;
   	devethercat_private  *priv = (devethercat_private *)record->dpvt;
	FN_CALLED;

   	CHECK_RECINIT;
  	NO_SYSTEM_RECORD;

    status = drvGetValue( priv->e, priv->dreg_info.offs, priv->dreg_info.bit,
    								(epicsUInt32 *)&(record->val), priv->dreg_info.bitlen, priv->dreg_info.bitspec, priv->dreg_info.byteoffs, priv->dreg_info.bytelen );

    /* apply typespec conversion, if any */
    switch( priv->dreg_info.typespec )
    {
		case epicsInt8T:		record->val = (epicsInt8)record->val; break;
		case epicsUInt8T:       record->val = (epicsUInt8)record->val; break;
		case epicsInt16T:       record->val = (epicsInt16)record->val; break;
		case epicsUInt16T:      record->val = (epicsUInt16)record->val; break;
		case epicsEnum16T:      record->val = (epicsEnum16)record->val; break;
		case epicsInt32T:       record->val = (epicsInt32)record->val; break;
		default:
			break;
    }
    CHECK_STATUS;

    return status;
}



/*---------------------------------------------------------- */
/*                                                           */
/* longout                                                   */
/*                                                           */
/*---------------------------------------------------------- */

long dev_rw_longout( longoutRecord *record )
{
   	int status = 0;
   	devethercat_private  *priv = (devethercat_private *)record->dpvt;
	FN_CALLED;

   	CHECK_RECINIT;
  	NO_SYSTEM_RECORD;

    /* apply typespec conversion, if any */
    switch( priv->dreg_info.typespec )
    {
		case epicsInt8T:		record->val = (epicsInt8)record->val; break;
		case epicsUInt8T:       record->val = (epicsUInt8)record->val; break;
		case epicsInt16T:       record->val = (epicsInt16)record->val; break;
		case epicsUInt16T:      record->val = (epicsUInt16)record->val; break;
		case epicsEnum16T:      record->val = (epicsEnum16)record->val; break;
		case epicsInt32T:       record->val = (epicsInt32)record->val; break;
		default:
			break;
    }

    status = drvSetValue( priv->e, priv->dreg_info.offs, priv->dreg_info.bit,
    								(epicsUInt32 *)&(record->val), priv->dreg_info.bitlen, priv->dreg_info.bitspec, priv->dreg_info.byteoffs, priv->dreg_info.bytelen );
    CHECK_STATUS;

    return status;
}




/*---------------------------------------------------------- */
/*                                                           */
/* stringin                                                  */
/*                                                           */
/*---------------------------------------------------------- */

long dev_rw_stringin( stringinRecord *record )
{
   	int status = 0;
   	devethercat_private  *priv = (devethercat_private *)record->dpvt;
	FN_CALLED;

   	CHECK_RECINIT;
  	NO_SYSTEM_RECORD;

    status = drvGetValueString( priv->e, priv->dreg_info.offs, priv->dreg_info.bitlen,
										record->val, record->oval, priv->dreg_info.byteoffs, priv->dreg_info.bytelen );
    CHECK_STATUS;

    return status;
}



/*---------------------------------------------------------- */
/*                                                           */
/* stringout                                                 */
/*                                                           */
/*---------------------------------------------------------- */

long dev_rw_stringout( stringoutRecord *record )
{
   	int status = 0;
   	devethercat_private  *priv = (devethercat_private *)record->dpvt;
	FN_CALLED;

   	CHECK_RECINIT;
  	NO_SYSTEM_RECORD;

    status = drvSetValueString( priv->e, priv->dreg_info.offs, priv->dreg_info.bitlen,
    									record->val, record->oval, priv->dreg_info.byteoffs, priv->dreg_info.bytelen );
    CHECK_STATUS;

    return status;
}


/*---------------------------------------------------------- */
/*                                                           */
/* aai                                                       */
/*                                                           */
/*---------------------------------------------------------- */



long dev_rw_aai( aaiRecord *record )
{
   	int offs, len;
   	devethercat_private  *priv = (devethercat_private *)record->dpvt;

    FN_CALLED;

   	CHECK_RECINIT;
  	NO_SYSTEM_RECORD;

	if( !record->bptr )
		return ERR_PREREQ_FAIL;
  	offs = priv->dreg_info.offs + (priv->dreg_info.byteoffs >= 0 ? priv->dreg_info.byteoffs : 0);
  	len = priv->ftvl_len * record->nelm;

  	if( offs + len > priv->e->d->ddata.dsize )
  		return ERR_OUT_OF_RANGE;

  	drvGetBlock( priv->e, record->bptr, offs, len );

	record->nord = record->nelm;

	record->udf = 0;

    return 0;
}



/*---------------------------------------------------------- */
/*                                                           */
/* aao                                                       */
/*                                                           */
/*---------------------------------------------------------- */

long dev_rw_aao( aaoRecord *record )
{
   	int len, offs;
   	devethercat_private  *priv = (devethercat_private *)record->dpvt;

    FN_CALLED;

   	CHECK_RECINIT;
  	NO_SYSTEM_RECORD;

  	if( !record->bptr )
		return ERR_PREREQ_FAIL;

  	offs = priv->dreg_info.offs + (priv->dreg_info.byteoffs >= 0 ? priv->dreg_info.byteoffs : 0);
  	len = priv->ftvl_len * record->nelm;

  	if( offs + len > priv->e->d->ddata.dsize )
  		return ERR_OUT_OF_RANGE;

  	drvSetBlock( priv->e, record->bptr, offs, len );

	record->nord = record->nelm;

	record->udf = 0;

    return 0;
}



/*---------------------------------------------------------- */
/*                                                           */
/* INIT                                                      */
/*                                                           */
/*---------------------------------------------------------- */

static void add_record( int ix, devethercat_private *priv, dbCommon *record )
{
	conn_rec **cr;
	FN_CALLED;

	if( !priv )
	{
		errlogSevPrintf( errlogFatal, "%s %s: Cannot add record, not initialized correctly\n", __func__, record->name );
		return;
	}


	if( priv->sysrecdata.system )
		return;

	if( priv->pe == NULL )
	{
		errlogSevPrintf( errlogFatal, "%s: Cannot add record, PDO entry undefined.\n", __func__ );
		return;
	}


    /* is this rec already registered? */
    for( cr = &priv->pe->cr; *cr; cr = &(*cr)->next )
    	if( (*cr)->rec == record )
    	{
    		errlogSevPrintf( errlogFatal, "%s: Somehow, this same record has already been registered\n", __func__ );
    		return;
    	}


    *cr = calloc( 1, sizeof(conn_rec) );
	if( *cr == NULL )
	{
		errlogSevPrintf( errlogFatal, "%s: Memory allocation failed.\n", __func__ );
		return;
	}

	(*cr)->ix = ix;
    (*cr)->rectype = rectypes[ix].rtype;
    (*cr)->rec = record;
	(*cr)->dreg_info = &priv->dreg_info;

	(*cr)->ftvl_len = priv->ftvl_len;
	(*cr)->ftvl_type = priv->ftvl_type;
}


static int get_ftvl_len( dbCommon* record, int ftvl )
{
  	devethercat_private *priv = record->dpvt;

  	if( !priv )
		return NOTOK;
  	switch( ftvl )
    {
        case DBF_CHAR:	  priv->ftvl_type = epicsInt8T;	    priv->ftvl_len = sizeof(epicsInt8);     return OK;
        case DBF_UCHAR:	  priv->ftvl_type = epicsUInt8T;    priv->ftvl_len = sizeof(epicsUInt8);    return OK;
        case DBF_SHORT:   priv->ftvl_type = epicsInt16T;    priv->ftvl_len = sizeof(epicsInt16);    return OK;
        case DBF_USHORT:  priv->ftvl_type = epicsUInt16T;   priv->ftvl_len = sizeof(epicsUInt16);   return OK;
        case DBF_LONG:    priv->ftvl_type = epicsInt32T;    priv->ftvl_len = sizeof(epicsInt32);    return OK;
        case DBF_ULONG:   priv->ftvl_type = epicsUInt32T;   priv->ftvl_len = sizeof(epicsUInt32);   return OK;
        case DBF_FLOAT:   priv->ftvl_type = epicsFloat32T;  priv->ftvl_len = sizeof(epicsFloat32);  return OK;
        case DBF_DOUBLE:  priv->ftvl_type = epicsFloat64T;  priv->ftvl_len = sizeof(epicsFloat64);  return OK;
    }
    free( record->dpvt );
    record->dpvt = NULL;
	errlogSevPrintf( errlogFatal, "%s %s: FTVL value %d is invalid\n", __func__, record->name, ftvl );

    return NOTOK;
}




static int init_aa( devethercat_private *priv, aaiRecord *record, RECTYPE rectype, DBLINK *reclink )
{
	record->bptr = NULL;
	/* sets dtype and dlen */
	if( get_ftvl_len( (dbCommon*)record, record->ftvl ) != OK )
		return NOTOK;
	if( (record->nord = record->nelm) < 1 )
	{
		errlogSevPrintf( errlogFatal, "%s %s: Zero or negative number of elements is invalid\n", __func__, record->name );
	    return NOTOK;
	}

	if( priv->dreg_info.offs + (priv->dreg_info.byteoffs >= 0 ? priv->dreg_info.byteoffs : 0) + priv->ftvl_len*record->nelm >
								priv->e->d->ddata.dsize )

	{
		errlogSevPrintf( errlogFatal, "%s %s: Number of elements %d is invalid (%d + %d > %d)\n", __func__,
					record->name, record->nelm,
					priv->dreg_info.offs + (priv->dreg_info.byteoffs >= 0 ? priv->dreg_info.byteoffs : 0),
					priv->ftvl_len*record->nelm,
					priv->e->d->ddata.dsize
					);
	    return NOTOK;
	}

	record->bptr = calloc( record->nelm, priv->ftvl_len );
	if( !record->bptr )
	{
		errlogSevPrintf( errlogFatal, "%s %s: Out of memory\n", __func__, record->name );
	    return NOTOK;
	}

	return OK;
}




#define PARSE_IO_STR( rtype, inout )                                                                         \
reclink = &((rtype##Record *)record)->inout;                                                               \
if( dev_parse_io_string( priv, record, rectype, reclink ) != OK )                                            \
{                                                                                                            \
	errlogSevPrintf( errlogFatal, "%s: Parsing INP/OUT link '%s' failed (record %s, type %s)\n", __func__,   \
			reclink->text, record->name, rectypes[ix].recname );                                             \
	free( priv );                                                                                            \
	record->dpvt = NULL;                                                                                     \
	return ERR_BAD_ARGUMENT;																				\
}

long dev_init_record(
    dbCommon *record
)
{
  	devethercat_private *priv;
  	DBLINK *reclink;
  	int ix = 0, retv = 0;
  	char bitsp[20] = { 0 }, bitsp2[20] = { 0 }, bitsp3[128] = { 0 },
  			boffs[32] = { 0 }, blen[32] = { 0 }, *msg;
  	RECTYPE rectype = dev_get_record_type( record, &ix );
	FN_CALLED;

  	if( rectype == REC_ERROR )
    {
        errlogSevPrintf( errlogFatal, "%s: record %s of type '%s' is not supported \n", __func__, record->name, rectypes[ix].recname );
        return ERR_BAD_REQUEST;
    }
    priv = calloc( sizeof(devethercat_private), 1 );
    if( priv == NULL )
    {
        errlogSevPrintf( errlogFatal, "%s: %s %s: out of memory\n", __func__, rectypes[ix].recname, record->name );
        return ERR_OUT_OF_MEMORY;
    }
    record->dpvt = priv;

    switch( rectype )
    {
    	case REC_AI:
    					PARSE_IO_STR( ai, inp );
    					dev_special_linconv_ai( (aiRecord *)record, 1 );
						break;

    	case REC_AO:
    					PARSE_IO_STR( ao, out );
    					dev_special_linconv_ao( (aoRecord *)record, 1 );
						break;

    	case REC_BI:
    					PARSE_IO_STR( bi, inp );
    		    		break;

    	case REC_BO:
						PARSE_IO_STR( bo, out );
						retv = 2; // init output rec without RVAL (for ASR)
    					break;

    	case REC_MBBI:
						PARSE_IO_STR( mbbi, inp );
						priv->dreg_info.nobt = ((mbbiRecord *)record)->nobt;
						priv->dreg_info.shft = ((mbbiRecord *)record)->shft;
						priv->dreg_info.mask = ((mbbiRecord *)record)->mask << ((mbbiRecord *)record)->shft;
    					break;

    	case REC_MBBO:
						PARSE_IO_STR( mbbo, out );
						priv->dreg_info.nobt = ((mbboRecord *)record)->nobt;
						priv->dreg_info.shft = ((mbboRecord *)record)->shft;
						priv->dreg_info.mask = ((mbboRecord *)record)->mask << ((mbboRecord *)record)->shft;
						retv = 2; // init output rec without RVAL (for ASR)
    					break;

    	case REC_LONGIN:
						PARSE_IO_STR( longin, inp );
    					break;

    	case REC_LONGOUT:
						PARSE_IO_STR( longout, out );
						break;

    	case REC_STRINGIN:
						PARSE_IO_STR( stringin, inp );
						break;

    	case REC_STRINGOUT:
						PARSE_IO_STR( stringout, out );
						break;

    	case REC_AAI:
    					PARSE_IO_STR( aai, inp );
    					if( init_aa( priv, (aaiRecord *)record, rectype, reclink ) != OK )
    						return ERR_OPERATION_FAILED;

    					break;

    	case REC_AAO:
    					PARSE_IO_STR( aao, out );
    					if( init_aa( priv, (aaiRecord *)record, rectype, reclink ) != OK )
    						return ERR_OPERATION_FAILED;
    					break;

    	default:
						errlogSevPrintf( errlogFatal, "%s: record %s currently not supported\n", __func__, record->name );
						return ERR_BAD_ARGUMENT;
    }

	add_record( ix, priv, record );

	if( !priv->sysrecdata.system )
	{
		// normal, non-system records
		bitsp[0] = bitsp2[0] = bitsp3[0] = blen[0] = boffs[0] = 0;
		/* make a somewhat prettier printout */
		if( priv->dreg_info.bitspec >= 0 )
		{
			sprintf( bitsp, ".b%d", priv->dreg_info.bitspec );
			sprintf( bitsp2, "(+%d)", priv->dreg_info.bitspec );
		}


		if( priv->dreg_info.byteoffs > 0 )
			sprintf( boffs, "shifted to %d.%d, (+%d byte%s), ",
						priv->dreg_info.offs + priv->dreg_info.byteoffs,
						priv->dreg_info.bit,
						priv->dreg_info.byteoffs,
						priv->dreg_info.byteoffs == 1 ? "" : "s"
					);

		if( priv->dreg_info.bytelen > 0 )
			sprintf( blen, ", %sblock length %d byte%s",
									boffs,
									priv->dreg_info.bytelen,
									priv->dreg_info.bytelen ==1 ? "" : "s"
					);
		if( priv->dreg_info.typename )
			sprintf( bitsp3, "(override type: %s)", priv->dreg_info.typename );
		else
			sprintf( bitsp3,"(%d bit%s)", priv->dreg_info.bitlen,
									priv->dreg_info.bitlen == 1 ? "" : "s" );

		printf( PPREFIX "configured record %s (%s) as d%d.r%d (s%d.sm%d.p%d.e%d%s), offset %d.%d%s %s %s\n",
					record->name,
					record->rdes->name,
					priv->e->dnr,
					priv->dreg_nr,
					priv->pe->parent->parent->parent->nr,
					priv->pe->parent->parent->nr,
					priv->pe->parent->nr,
					priv->pe->nr,
					bitsp,
					priv->dreg_info.offs,
					priv->dreg_info.bit,
					bitsp2,
					bitsp3,
					blen

				);
	}
	else
	{
		// system health records
		msg = parse_system_keywords[(int)priv->sysrecdata.sysrectype-1] ? parse_system_keywords[(int)priv->sysrecdata.sysrectype-1] : "unknown type";
		if( priv->sysrecdata.sysrectype == SRT_S_OP_STATUS )
			sprintf( blen, " (m%d.s%d)", priv->sysrecdata.master_nr, priv->sysrecdata.nr );
		else
			sprintf( blen, " (m%d)", priv->sysrecdata.master_nr );

		printf( PPREFIX "configured record %s (%s) as %s%s system record\n",
				record->name,
				record->rdes->name,
				msg,
				blen
				);
	}

    return retv;

}

#undef PARSE_IO_STR


devsupport_linconv( ai );
devsupport_linconv( ao );
devsupport( bi );
devsupport( bo );
devsupport( mbbi );
devsupport( mbbo );
devsupport( longin );
devsupport( longout );
devsupport( stringin );
devsupport( stringout );
devsupport( aai );
devsupport( aao );



