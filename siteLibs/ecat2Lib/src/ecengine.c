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

long long wt_counter[EC_MAX_DOMAINS] = { 0 },
			delayed[EC_MAX_DOMAINS] = { 0 },
			recd[EC_MAX_DOMAINS] = { 0 },
			forwarded[EC_MAX_DOMAINS] = { 0 },
			irqs_executed[EC_MAX_DOMAINS] = { 0 },
			dropped[EC_MAX_DOMAINS] = { 0 },
			delayctr_cumulative[EC_MAX_DOMAINS] = { 0.0 };


static epicsUInt16 endian_uint16( epicsUInt16 val )
{
	epicsUInt16 retv = 0;
	char *p = (char *)(&retv);

	*p = val & 0xff;
	*(p+1) = (val & 0xff00) >> 8;

	return retv;
}

static epicsUInt32 endian_uint32( epicsUInt32 val )
{
	epicsUInt32 retv = 0;
	char *p = (char *)(&retv);

	*p 		= val & 0x000000ff;
	*(p+1) 	= (val & 0x0000ff00) >> 8;
	*(p+2) 	= (val & 0x00ff0000) >> 16;
	*(p+3) 	= (val & 0xff000000) >> 24;

	return retv;
}

EC_ERR drvGetSysRecData( ethcat *e, system_rec_data *sysrecdata, dbCommon *record, epicsUInt32 *val )
{
	ecnode *s;
	if( !e )
		return ERR_OPERATION_FAILED;
	if( !e->m )
		return ERR_OPERATION_FAILED_1;

	switch( sysrecdata->sysrectype )
	{
		case SRT_M_STATUS:		*val = get_master_health( e ); break;
		case SRT_S_STATUS:  	*val = get_slave_aggregate_health( e ); break;
		case SRT_L_STATUS:		*val = get_master_link_up( e ); break;
		case SRT_S_OP_STATUS:
								s = ecn_get_child_nr_type( e->m, sysrecdata->nr, ECNT_SLAVE );
								if( !s )
									return ERR_OPERATION_FAILED_2;
								*val = get_slave_health( e, s );
								break;

		default:				return ERR_OPERATION_FAILED_3;
	}

	return 0;
}

int drvGetValue( ethcat *e, int offs, int bit, epicsUInt32 *val, int bitlen, int bitspec, int byteoffs, int bytelen )
{
	static long get_speclen_counter = 0;
	char *rw = e->r_data;

	FN_CALLED3;
	if( !e || !val )
		return ERR_BAD_ARGUMENT;

	if( !e->r_data || !e->w_data )
		return ERR_BAD_REQUEST;

	if( byteoffs > 0 )
		offs += byteoffs;
	if( bytelen > 0 )
	{
		if( bytelen <= sizeof(epicsUInt32) )
			bitlen = 8 * bytelen;
		else
		{
			errlogSevPrintf( errlogFatal, "%s: for this record type, length (.Lnn) has to be 0 < length < 5 (currently set to %d)\n",
					__func__, bytelen );
			return S_dev_badArgument;
		}
	}

	epicsMutexMustLock( e->rw_lock );
	switch( bitlen )
	{
		case 1:
				*val = (*(rw + offs) >> bit) & 0x01;
				break;
		case 8:
				if( bitspec >= 0 )
					*val = (*(rw + offs) >> bitspec) & 0x01;
				else
					*val = *(rw + offs);

				break;
		case 16:
				if( bitspec >= 0 )
					*val = (endian_uint16( *(epicsUInt16 *)(rw + offs) ) >> bitspec) & 0x0001;
				else
					*val = endian_uint16( *(epicsUInt16 *)(rw + offs) );
				break;
		case 32:
				if( bitspec >= 0 )
					*val = (endian_uint32( *(epicsUInt32 *)(rw + offs) ) >> bitspec) & 0x00000001;
				else
					*val = endian_uint32( *(epicsUInt32 *)(rw + offs) );
				break;

		default:
				get_speclen_counter++;
				break;
	}
	epicsMutexUnlock( e->rw_lock );


    return 0;
}

int drvSetValue( ethcat *e, int offs, int bit, epicsUInt32 *val, int bitlen, int bitspec, int byteoffs, int bytelen )
{
	int retv = 0;

	FN_CALLED;

	if( !e || !val )
		return ERR_BAD_ARGUMENT;

	if( !e->w_data )
		return ERR_BAD_REQUEST;

	if( byteoffs > 0 )
		offs += byteoffs;
	if( bytelen > 0 )
	{
		if( bytelen == 3 || bytelen > sizeof(epicsUInt32) )
		{
			errlogSevPrintf( errlogFatal, "%s: for this record type, length (.Lnn) has to be 1, 2 or 4 (currently set to %d)\n",
					__func__, bytelen );
			return S_dev_badArgument;
		}
		else
			bitlen = 8 * bytelen;
	}

	epicsMutexMustLock( e->rw_lock );
	switch( bitlen )
	{
		case 1:
				if( *val & 0x01 )
					*(e->w_data + offs) |= (0x01 << bit);
				else
					*(e->w_data + offs) &= ~(0x01 << bit);

				*(e->w_mask + offs) |= (0x01 << bit);
				break;
		case 8:
				if( bitspec >= 0 )
				{
					*(e->w_data + offs) &= ~(1 << bitspec);
					*(e->w_data + offs) |= ((*val & 0x01) << bitspec);
					*(e->w_mask + offs) |= (0x01 << bitspec);
				}
				else
				{
					*(e->w_data + offs) = *val;
					*(e->w_mask + offs) = 0xff;
				}

				break;
		case 16:
				if( bitspec >= 0 )
				{
					if( *val & 0x0001 )
						*(epicsUInt16 *)(e->w_data + offs) |= endian_uint16((*val & 0x0001) << bitspec);
					else
						*(epicsUInt16 *)(e->w_data + offs) &= ~endian_uint16(1 << bitspec);
					*(epicsUInt16 *)(e->w_mask + offs) |= endian_uint16(1 << bitspec);
				}
				else
				{
					*(epicsUInt16 *)(e->w_data + offs) 	= endian_uint16(*val);
					*(epicsUInt16 *)(e->w_mask + offs) 	= 0xffff;
				}
				break;
		case 32:
				if( bitspec >= 0 )
				{
					if( *val & 0x00000001 )
						*(epicsUInt32 *)(e->w_data + offs) |= endian_uint32((*val & 0x00000001) << bitspec);
					else
						*(epicsUInt32 *)(e->w_data + offs) &= ~endian_uint32(1 << bitspec);
					*(epicsUInt32 *)(e->w_mask + offs) |= endian_uint32(1 << bitspec);
				}
				else
				{
					*(epicsUInt32 *)(e->w_data + offs) 	= endian_uint32(*val);
					*(epicsUInt32 *)(e->w_mask + offs) 	= 0xffffffff;
				}
				break;
		default:
				if( bitlen < 1 || bitlen > 8 )
				{
					errlogSevPrintf( errlogFatal, "%s: bitlen %d not allowed\n", __func__, bitlen );
					retv = 1; /* error, unaligned field */
					break;
				}
				break;
	}
	epicsMutexUnlock( e->rw_lock );

    return retv;
}

#define SHOW_GET_DEBUG 0
#define SHOW_SET_DEBUG 0
int drvGetValueMasked( ethcat *e, int offs, int bit, epicsUInt32 *rval, int bitlen, epicsInt16 nobt, epicsUInt16 shift, epicsUInt32 mask, int byteoffs, int bytelen )
{

	FN_CALLED3;

	if( !e || !rval )
		return ERR_BAD_ARGUMENT;

	if( !e->r_data )
		return ERR_OPERATION_FAILED;

	if( byteoffs > 0 )
		offs += byteoffs;
	if( bytelen > 0 )
	{
		if( bytelen == 3 || bytelen > sizeof(epicsUInt32) )
		{
			errlogSevPrintf( errlogFatal, "%s: for this record type, length (.Lnn) has to be 1, 2 or 4 (currently set to %d)\n",
					__func__, bytelen );
			return S_dev_badArgument;
		}
		else
			bitlen = 8 * bytelen;
	}


#if SHOW_SET_DEBUG
	printf( "\n%s:get before: *val=0x%08x, ec*val= 0x%08x, wdata=0x%08x ecwdata=0x%08x (offs.bit:%d.%d, bitlen %d), mask=0x%08x\n", __func__,
			*rval, endian_uint32( *rval ), *(epicsUInt32 *)(e->w_data + offs), endian_uint32(*(epicsUInt32 *)(e->w_data + offs)), offs, bit, bitlen, mask );
#endif
	*rval = (endian_uint32(*(epicsUInt32 *)(e->r_data + offs)) & mask);

#if SHOW_SET_DEBUG
	printf( "%s:get  after: *val=0x%08x, ec*val= 0x%08x, wdata=0x%08x ecwdata=0x%08x (offs.bit:%d.%d, bitlen %d), mask=0x%08x\n", __func__,
			*rval, endian_uint32( *rval ), *(epicsUInt32 *)(e->w_data + offs), endian_uint32(*(epicsUInt32 *)(e->w_data + offs)), offs, bit, bitlen, mask );
#endif

    return 0;
}


int drvSetValueMasked( ethcat *e, int offs, int bit, epicsUInt32 *val, int bitlen, epicsInt16 nobt, epicsUInt16 shift, epicsUInt32 mask, int byteoffs, int bytelen )
{
	FN_CALLED;

	if( !e || !val )
		return ERR_OPERATION_FAILED;

	if( !e->w_data || !e->w_mask )
		return ERR_OPERATION_FAILED_1;

	if( byteoffs > 0 )
		offs += byteoffs;
	if( bytelen > 0 )
	{
		if( bytelen == 3 || bytelen > sizeof(epicsUInt32) )
		{
			errlogSevPrintf( errlogFatal, "%s: for this record type, length (.Lnn) has to be 1, 2 or 4 (currently set to %d)\n",
					__func__, bytelen );
			return S_dev_badArgument;
		}
		else
			bitlen = 8 * bytelen;
	}

	epicsMutexMustLock( e->rw_lock );

#if SHOW_GET_DEBUG
	printf( "\n%s: set before: *val=0x%08x, ec*val= 0x%08x, wdata=0x%08x ecwdata=0x%08x (offs.bit:%d.%d, bitlen %d), mask=0x%08x\n", __func__,
			*val, endian_uint32( *val ), *(epicsUInt32 *)(e->w_data + offs), endian_uint32(*(epicsUInt32 *)(e->w_data + offs)), offs, bit, bitlen, mask );
#endif

	*(epicsUInt32 *)(e->w_data + offs) &= endian_uint32( ~mask );
	*(epicsUInt32 *)(e->w_data + offs) |= endian_uint32( *val );
	*(epicsUInt32 *)(e->w_mask + offs) |= endian_uint32( mask );

#if SHOW_GET_DEBUG
	printf( "%s:  set after: *val=0x%08x, ec*val= 0x%08x, wdata=0x%08x ecwdata=0x%08x (offs.bit:%d.%d, bitlen %d), mask=0x%08x\n", __func__,
			*val, endian_uint32( *val ), *(epicsUInt32 *)(e->w_data + offs), endian_uint32(*(epicsUInt32 *)(e->w_data + offs)), offs, bit, bitlen, mask );
#endif
	epicsMutexUnlock( e->rw_lock );


	return 0;
}


int drvGetValueString( ethcat *e, int offs, int bitlen, char *val, char *oval, int byteoffs, int bytelen )
{
	char *rw = e->r_data;

	FN_CALLED3;
	if( !e || !val )
		return ERR_OPERATION_FAILED;

	if( !e->r_data || !e->w_data )
		return ERR_OPERATION_FAILED_1;

	if( bytelen <= 0 || bytelen > 40 )
	{
		errlogSevPrintf( errlogFatal, "%s: string of length %d not supported\n",
													__func__, bytelen );
		return ERR_BAD_ARGUMENT;
	}

	if( byteoffs < 0 )
		byteoffs = 0;
	if( bytelen > e->d->ddata.dsize ||
			(byteoffs + bytelen) > e->d->ddata.dallocated )
	{
		errlogSevPrintf( errlogFatal, "%s: additional offset (.O%d) and/or length (.L%d) exceed domain size/allocated memory limits\n",
													__func__, byteoffs, bytelen );
		return S_dev_badArgument;
	}

	epicsMutexMustLock( e->rw_lock );

	memcpy( oval, val, 40 );
	memset( val, 0, 40 );
	memcpy( val, rw + offs + byteoffs, bytelen );

	epicsMutexUnlock( e->rw_lock );

    return 0;
}

int drvSetValueString( ethcat *e, int offs, int bitlen, char *val, char *oval, int byteoffs, int bytelen )
{
	int retv = 0;

	FN_CALLED;

	if( !e || !val )
		return ERR_OPERATION_FAILED;

	if( !e->w_data )
		return ERR_OPERATION_FAILED_1;

	if( bytelen <= 0 || bytelen > 40 )
	{
		errlogSevPrintf( errlogFatal, "%s: string of length %d not supported\n",
													__func__, bytelen );
		return ERR_BAD_ARGUMENT;
	}

	if( byteoffs < 0 )
		byteoffs = 0;
	if( bytelen > e->d->ddata.dsize ||
			(byteoffs + bytelen) > e->d->ddata.dallocated )
	{
		errlogSevPrintf( errlogFatal, "%s: additional offset (.O%d) and/or length (.L%d) exceed domain size/allocated memory limits\n",
													__func__, byteoffs, bytelen );
		return S_dev_badArgument;
	}

	if( bytelen < 40 )
		*(val + bytelen) = 0;

	epicsMutexMustLock( e->rw_lock );

	memcpy( e->w_data + offs + byteoffs, val, bytelen );
	memset( e->w_mask + offs + byteoffs, 0xff, bytelen );

	epicsMutexUnlock( e->rw_lock );

    return retv;
}



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
)
{
	float f1, f2;
	double d1;
	char *p, *d;
	int i;
	FN_CALLED3;
	if( !e )
		return ERR_BAD_ARGUMENT;

	if( !val )
		return ERR_BAD_ARGUMENT_1;

	if( !e->r_data || !e->w_data )
		return ERR_PREREQ_FAIL;

	if( byteoffs > 0 )
		offs += byteoffs;

	epicsMutexMustLock( e->rw_lock );

	if( etype == epicsFloat32T )
	{
		p = (char *)&f1;
		d = (char *)&f2;
		f1 = *(float *)(e->r_data + offs );
		*(d+0) = *(p+3); *(d+1) = *(p+2); *(d+2) = *(p+1); *(d+3) = *(p+0);
		 *fval = (double)f2;
 	}
	else
	{
		d1 = *(double *)(e->r_data + offs );
		for( i = 0; i < sizeof(double); i++ )
			*((char *)fval + i) = *((char *)&d1 + sizeof(double) - i);
	}

	epicsMutexUnlock( e->rw_lock );

    return 0;
}

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
 )
{
	int i, retv = 0;
	float f1;
	char *p, *d;

	FN_CALLED;

	if( !e )
		return ERR_BAD_ARGUMENT;

	if( !val )
		return ERR_BAD_ARGUMENT_1;

	if( !e->w_data )
		return ERR_PREREQ_FAIL;

	if( byteoffs > 0 )
		offs += byteoffs;

	epicsMutexMustLock( e->rw_lock );


	if( etype == epicsFloat32T )
	{
		p = (char *)&f1;
		d = e->w_data + offs;
		f1 = (float)*fval;
		*(d+0) = *(p+3); *(d+1) = *(p+2); *(d+2) = *(p+1); *(d+3) = *(p+0);
 	}
	else
	{
		p = e->w_data + offs;
		for( i = 0; i < sizeof(double); i++ )
			*(p + i) = *((char *)fval + sizeof(double) - i);
	}


	memset( e->w_mask + offs, 0xff, etype == epicsFloat32T ? sizeof(float) : sizeof(double) );

	epicsMutexUnlock( e->rw_lock );

    return retv;
}




int drvGetBlock(
		ethcat *e,
		char *buf,
		int offs,
		int len
)
{
	FN_CALLED3;

	if( !e )
		return ERR_BAD_ARGUMENT;

	if( !buf )
		return ERR_BAD_ARGUMENT_1;

	if( len < 1 )
		return ERR_BAD_ARGUMENT_2;

	if( offs + len > e->d->ddata.dsize )
		return ERR_BAD_ARGUMENT_3;

	if( !e->r_data )
		return ERR_PREREQ_FAIL;


	epicsMutexMustLock( e->rw_lock );

	memcpy( buf, e->r_data + offs, len );

	epicsMutexUnlock( e->rw_lock );

	return ERR_NO_ERROR;
}



int drvSetBlock(
		ethcat *e,
		char *buf,
		int offs,
		int len
)
{
	FN_CALLED3;

	if( !e )
		return ERR_BAD_ARGUMENT;

	if( !buf )
		return ERR_BAD_ARGUMENT_1;

	if( len < 1 )
		return ERR_BAD_ARGUMENT_2;

	if( offs + len > e->d->ddata.dsize )
		return ERR_BAD_ARGUMENT_3;

	if( !e->w_data )
		return ERR_PREREQ_FAIL;

	epicsMutexMustLock( e->rw_lock );

	memcpy( e->w_data + offs, buf, len );
	memset( e->w_mask + offs, 0xff, len );

	epicsMutexUnlock( e->rw_lock );

	return ERR_NO_ERROR;
}




/*-------------------------------------------------------------------*/
/*                                                                   */
/* System health check thread                                                        */
/*                                                                   */
/*-------------------------------------------------------------------*/

void set_master_health( ethcat *ec, int health )
{
	epicsMutexMustLock( ec->health_lock );
	ec->m->mdata.health = health;
	epicsMutexUnlock( ec->health_lock );
}

int get_master_health( ethcat *ec )
{
	int retv;
	epicsMutexMustLock( ec->health_lock );
	retv = ec->m->mdata.health;
	epicsMutexUnlock( ec->health_lock );

	return retv;
}

void set_master_link_up( ethcat *ec, int link_up )
{
	epicsMutexMustLock( ec->health_lock );
	ec->m->mdata.link_up = link_up;
	epicsMutexUnlock( ec->health_lock );
}

int get_master_link_up( ethcat *ec )
{
	int retv;
	epicsMutexMustLock( ec->health_lock );
	retv = ec->m->mdata.link_up;
	epicsMutexUnlock( ec->health_lock );

	return retv;
}

void set_slave_health( ethcat *ec, ecnode *s, int health )
{
	epicsMutexMustLock( ec->health_lock );
	s->sdata.health = health;
	epicsMutexUnlock( ec->health_lock );
}

int get_slave_health( ethcat *ec, ecnode *s )
{
	int retv;
	if( !s )
		return 0;
	epicsMutexMustLock( ec->health_lock );
	retv = s->sdata.health;
	epicsMutexUnlock( ec->health_lock );

	return retv;
}

void set_slave_aggregate_health( ethcat *ec, int health )
{
	epicsMutexMustLock( ec->health_lock );
	ec->m->mdata.slave_aggr_health = health;
	epicsMutexUnlock( ec->health_lock );

}

int get_slave_aggregate_health( ethcat *ec )
{
	int retv;
	epicsMutexMustLock( ec->health_lock );
	retv = ec->m->mdata.slave_aggr_health;
	epicsMutexUnlock( ec->health_lock );

	return retv;

}

void ec_shc_thread( void *data )
{
	int setto, sl_aggr, start_slave_count, last_count;
	ethcat *ec = (ethcat *)data;
	struct timespec tperiod = { .tv_sec = 1, .tv_nsec = 0 };
	ec_master_t *ecm;
	ec_master_info_t *minfo;
	ec_master_state_t state;
	ecnode *n;
	ec_slave_info_t current_sinfo;

	if( !ec )
	{
		errlogSevPrintf( errlogFatal, "%s: data not valid\n", __func__ );
		goto thread_out;
	}

	if( !ec->m )
	{
		errlogSevPrintf( errlogFatal, "%s: master not valid\n", __func__ );
		goto thread_out;
	}
	if( !ec->m->child )
	{
		errlogSevPrintf( errlogFatal, "%s: no slaves found\n", __func__ );
		goto thread_out;
	}
	ecm = ec->m->mdata.master;
	if( !ecm )
	{
		errlogSevPrintf( errlogFatal, "%s: master not valid (2)\n", __func__ );
thread_out:
		errlogSevPrintf( errlogFatal, "%s: System health check thread NOT started!\n", __func__ );
		return;
	}

	minfo = &ec->m->mdata.master_info;
	set_master_health( ec, 1 );
	set_master_link_up( ec, 1 );

	last_count = start_slave_count = ec->m->mdata.master_info_at_start.slave_count;

    while(1)
    {
    	clock_nanosleep( CLOCK_MONOTONIC, 0, &tperiod, NULL );

		if( ecrt_master( ecm, minfo ) )
        {
			set_master_health( ec, 0 );
			set_master_link_up( ec, 0 );
			set_slave_aggregate_health( ec, 0 );
			continue;
        }
		else
			set_master_health( ec, 1 );

		if( !minfo->link_up )
        {
			set_master_link_up( ec, 0 );
			continue;
        }
		else
			set_master_link_up( ec, 1 );


		ecrt_master_state( ecm, &state );


		if( last_count != state.slaves_responding )
			printf( PPREFIX "Number of slaves responding has changed to %d (%d slaves expected)!\n",
								state.slaves_responding, start_slave_count );
		last_count = state.slaves_responding;

		sl_aggr = 1;
		n = ec->m->child;
		do
		{
			if( n->type != ECNT_SLAVE )
				continue;
			if( n->sdata.check )
			{
				setto = 0;
				memset( &current_sinfo, 0, sizeof(ec_slave_info_t) );

				if( !ecrt_master_get_slave( ecm, n->nr, &current_sinfo ) )
				{

					if(
/*
						config_sinfo->vendor_id 		== current_sinfo.vendor_id 		&&
						config_sinfo->product_code 		== current_sinfo.product_code 	&&
						config_sinfo->revision_number 	== current_sinfo.revision_number &&
						config_sinfo->serial_number 	== current_sinfo.serial_number 	&&
*/
						(current_sinfo.al_state & S_OP)
						)
							setto = 1;
				}

				sl_aggr &= setto;
				set_slave_health( ec, n, setto );

			}

		} while( (n = n->next) );

		if( minfo->slave_count != start_slave_count  ||
				state.slaves_responding != start_slave_count )
			sl_aggr = 0;

		set_slave_aggregate_health( ec, sl_aggr );

    }

}



/*-------------------------------------------------------------------*/
/*                                                                   */
/* IRQ thread                                                        */
/*                                                                   */
/*-------------------------------------------------------------------*/
void ec_irq_thread( void *data )
{
	ethcat *ec = (ethcat *)data;

    while(1)
    {
        epicsEventMustWait( ec->irq );

        scanIoRequest( ec->r_scan );
		scanIoRequest( ec->w_scan );

    }

}

/*-------------------------------------------------------------------*/
/*                                                                   */
/* Worker thread                                                     */
/*                                                                   */
/*-------------------------------------------------------------------*/

void copy_1bit( char *source, int sbyte_offs, int sbit_offs, char *dest, int dbyte_offs, int dbit_offs )
{
	if( sbit_offs > 7 )
	{
		sbyte_offs += sbit_offs / 8;
		sbit_offs %= 8;
	}
	if( dbit_offs > 7 )
	{
		dbyte_offs += dbit_offs / 8;
		dbit_offs %= 8;
	}

	*(dest+dbyte_offs) &= ~(0x1 << dbit_offs);
	*(dest+dbyte_offs) |= (((*(source+sbyte_offs) >> sbit_offs) & 1) << dbit_offs);

}


void process_sts_entries( ecnode *d )
{
	register int i;
	domain_register *from, *to;
	sts_entry **se;

	epicsMutexMustLock( d->ddata.sts_lock );
    for( se = &(d->ddata.sts); *se; se = &((*se)->next) )
    {
    	from = &((*se)->from);
    	to = &((*se)->to);

    	if( !(from->bitlen % 8) && !from->bit && (from->bitlen/8 > 0) &&
    			!(to->bitlen % 8) && !to->bit && (to->bitlen/8 > 0) &&
    			from->bitspec < 0 && to->bitspec < 0 )
    		memmove( d->ddata.dmem + to->offs, d->ddata.dmem + from->offs, from->bitlen / 8 );

    	else
    	{

    		if( from->bitspec > -1 || to->bitspec > -1 )
				copy_1bit( d->ddata.dmem, from->offs, from->bitspec > -1 ? from->bitspec : (from->bitlen == 1 ? from->bit : 0),
							d->ddata.dmem, to->offs, to->bitspec > -1 ? to->bitspec : (to->bitlen == 1 ? to->bit : 0) );
    		else
				for( i = 0; i < from->bitlen; i++ )
					copy_1bit( d->ddata.dmem, from->offs, from->bit,
								d->ddata.dmem, to->offs, to->bit );
    	}
    }
    epicsMutexUnlock( d->ddata.sts_lock );

}


inline void process_write_values( char *dmem, ecnode *d, char *wmask )
{
	register int i;

	for( i = 0; i < d->ddata.dsize; i++ )
	{
		d->ddata.dmem[i] &= ~wmask[i];
		d->ddata.dmem[i] |= d->ddata.wmem[i];
	}
	memset( wmask, 0, d->ddata.dsize );

}

inline int irq_values_changed( ethcat *ec )
{
	register int mask;
	register int i, len = ec->d->ddata.dsize / sizeof(int);
	register int *irqmask = (int *)ec->irq_r_mask,  /* using int for faster cmp */
					*rmem = (int *)ec->d->ddata.rmem,
					*dmem = (int *)ec->d->ddata.dmem;

	for( i = 0; i < len; i++ )
		if( (mask = *(irqmask + i)) )
			if( (*(rmem + i) & mask) !=
					(*(dmem + i) & mask) )
				return 1;

	return 0;
}

/*#define PRINT_DEBUG_TIMING */

#ifdef PRINT_DEBUG_TIMING
static int use_dt_1 = 0,
		use_dt_2 = 0;
#endif

#define __s(x) \
	if( use_dt_##x ) \
		st_start( x );

#define __e(x) \
	if( use_dt_##x ) \
		st_end( x );

#define __p(x) \
		clock_gettime( CLOCK_MONOTONIC, &pt_now[x] ); \
		if( pt_now[x].tv_sec > pt_last[x].tv_sec ) \
		{ \
			printf( "*** TIMING: " ); \
			st_print( x ); \
			printf( "\n" ); \
		} \
		memcpy( &pt_last[x], &pt_now[x], sizeof(struct timespec) );

void ec_worker_thread( void *data )
{
	int delayctr, dnr, chg = 0;
	struct timespec rec = { .tv_sec = 0, .tv_nsec = 50000 };
	ethcat *ec = (ethcat *)data;
	ec_master_t *ecm;
	ec_domain_t *ecd;

#ifdef PRINT_DEBUG_TIMING
	struct timespec pt_last[10] = { { 0 } }, pt_now[10];
#endif

	FN_CALLED;

	if( !ec->m )
	{
		errlogSevPrintf( errlogFatal, "%s: master not valid\n", __func__ );
		return;
	}
	if( !ec->d )
	{
		errlogSevPrintf( errlogFatal, "%s: domain not valid\n", __func__ );
		return;
	}
	if( !ec->r_data || !ec->w_data )
	{
		errlogSevPrintf( errlogFatal, "%s: Internal error - r_data/w_data not initialized\n", __func__ );
		return;
	}
	if( tmr_init( ec->d->nr, ec->rate ) != OK )
	{
		errlogSevPrintf( errlogFatal, "%s: CPU timer %d failure\n", __func__, ec->d->nr );
		return;
	}

	ecm = ec->m->mdata.master;
	ecd = ec->d->domain_t;

	dnr = ec->d->nr;

	ec->d->ddata.is_running = 1;

	/*---------------------- */
	while( 1 )
	{

		ecrt_master_receive( ecm );
		ecrt_domain_process( ecd );

			st_start( ECT_ECWORK_TOTAL );
		epicsMutexMustLock( ec->rw_lock );
			st_start( ECT_IRQ );
		chg = irq_values_changed( ec );
			st_end( ECT_IRQ );
			st_start( ECT_RW );
		memcpy( ec->d->ddata.rmem, ec->d->ddata.dmem, ec->d->ddata.dsize );
		process_write_values( ec->d->ddata.dmem, ec->d, ec->w_mask );
			st_end( ECT_RW );
			st_start( ECT_STS );
		process_sts_entries( ec->d );
			st_end( ECT_STS );
		epicsMutexUnlock( ec->rw_lock );
			st_end( ECT_ECWORK_TOTAL );

		if( chg )
		{
			epicsEventSignal( ec->irq );
			irqs_executed[dnr]++;
		}

#ifdef PRINT_DEBUG_TIMING
		__s(1);
#endif
		ecrt_domain_queue( ecd );
		ecrt_master_send( ecm );
#ifdef PRINT_DEBUG_TIMING
		__e(1);
#endif

		forwarded[dnr] += (tmr_wait( 0 ) - 1);

		delayctr = 0;
		while( 1 )
		{
#ifdef PRINT_DEBUG_TIMING
			__s(2);
#endif
            ecrt_master_receive( ecm );
#ifdef PRINT_DEBUG_TIMING
    		__e(2);
#endif
    		if( ecrt_domain_received( ecd ) )
    		{
    			recd[dnr]++;
            	break;
    		}

			if( ++delayctr > 50 )
			{
				dropped[dnr]++;
				break;
			}
			clock_nanosleep( CLOCK_MONOTONIC, 0, &rec, NULL );
		}

		if( delayctr )
		{
			delayed[dnr]++;
			delayctr_cumulative[dnr] += (double)delayctr;
		}
		wt_counter[dnr]++;

#ifdef PRINT_DEBUG_TIMING
		__p(1);
		__p(2);
#endif
	}

	ec->d->ddata.is_running = 0;

}


