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

/*------------------------------------------------------------------- */


#define ECAT_TNAME_D	"ecat_dom"
#define ECAT_TNAME_IRQ	"ecat_irq"
#define ECAT_TNAME_SC	"ecat_sc"

int drvethercatDebug = 0;
static ethcat *ecatList = NULL;

/*
static void dmmdebug( const char *format, ... )
{
    va_list args;
    va_start( args, format );
    vfprintf( stderr, format, args );
    va_end( args );
    fflush( stderr );
}
*/


static inline void *_zalloc( int size )
{
	void *p = calloc( 1, size );
	if( !p )
		errlogSevPrintf( errlogFatal, "%s: Memory allocation failed.\n", __func__ );
	return p;
}
#define zalloc(size) ({ void *__p = _zalloc(size); if(!__p) exit(S_dev_noMemory); __p; })



/*------------------------------------------------------------ */
/*                                                             */
/* Hooks                                                       */
/*                                                             */
/*------------------------------------------------------------ */

void process_hooks( initHookState state )
{
	ethcat **ec;
	static int workthreadsrunning = 0;
	FN_CALLED;

	switch( state )
	{
		case initHookAfterDatabaseRunning:

				if( workthreadsrunning )
					return;
				workthreadsrunning = 1;

				/* unfortunately, this below does not help, */
				/* it only delays the inevitable callback buffer overrun */
				/*callbackSetQueueSize( 1000000 ); */

				/* start various threads */
				for( ec = &ecatList; *ec; ec = &(*ec)->next )
				{
					(*ec)->dthread = epicsThreadMustCreate( ECAT_TNAME_D, 80, /* epicsThreadPriorityLow, */
										epicsThreadGetStackSize(epicsThreadStackSmall), &ec_worker_thread, *ec );
					(*ec)->irqthread = epicsThreadMustCreate( ECAT_TNAME_IRQ, epicsThreadPriorityLow,
										epicsThreadGetStackSize(epicsThreadStackSmall), &ec_irq_thread, *ec );
					(*ec)->scthread = epicsThreadMustCreate( ECAT_TNAME_SC, epicsThreadPriorityLow,
										epicsThreadGetStackSize(epicsThreadStackSmall), &ec_shc_thread, *ec );
					printf( PPREFIX "worker, irq and slave-check threads started\n" );
				}
				break;

		default:
				break;
	}


}




/*-------------------------------------------------------------------- */
ethcat *ethercatOpen( int domain_nr )
{
	ethcat *e;
	FN_CALLED;

	for( e = ecatList; e; e = e->next )
	{
		if( e->dnr == domain_nr )
			return e;
	}
	return NULL;
}


void drvethercatAtExit( void *arg )
{
	ethcat *e = (ethcat *)arg;

	FN_CALLED;

	pinfo( "%s: Deactivating master, domain %d\n", __func__, e->dnr );
	ecrt_master_deactivate( ecroot->child->mdata.master );

	pinfo( "%s: Releasing master\n", __func__ );
	ecrt_release_master( ecroot->child->mdata.master );

}

ethcat *drvFindDomain( int dnr )
{
	ethcat **ec;
	for( ec = &ecatList; *ec; ec = &(*ec)->next )
		if( (*ec)->dnr == dnr )
			return *ec;

	return NULL;
}

int drvGetRegisterDesc( ethcat *e, domain_register *dreg, int regnr, ecnode **pentry, int *token_num )
{
	ecnode *d;

	if( !e )
	{
		errlogSevPrintf( errlogFatal, "%s: ethcat/e NULL - init failed\n", __func__ );
		return FAIL;
	}

	if( !e->d )
	{
		errlogSevPrintf( errlogFatal, "%s: ethcat domain NULL - domain init failed\n", __func__ );
		return FAIL;
	}

	d = e->d;

	FN_CALLED;
	if( regnr < 0 || regnr > d->ddata.num_of_regs )
	{
		errlogSevPrintf( errlogFatal, "%s: Domain register %d is out of range for this domain\n", __func__, regnr );
		return FAIL;
	}


	dreg->offs = d->ddata.reginfos[regnr].byte;
	dreg->bit = d->ddata.reginfos[regnr].bit;
	dreg->bitlen = (token_num[T_NUM] < 0 ? d->ddata.reginfos[regnr].bit_length : 8*parse_datatype_get_len( token_num[T_NUM] ) );
	dreg->bitspec = token_num[B_NUM];
	dreg->byteoffs = token_num[O_NUM];
	dreg->bytelen = token_num[L_NUM];
	dreg->typespec = token_num[T_NUM];

	*pentry = d->ddata.reginfos[regnr].pdo_entry;
	if( !*pentry )
	{
		errlogSevPrintf( errlogFatal, "%s: Domain register %d appears to have no pdo entry associated\n", __func__, regnr );
		return FAIL;
	}

	return OK;
}


int drvGetLocalRegisterDesc( ethcat *e, domain_register *dreg, int *dreg_nr, ecnode **pentry, int *token_num )
{
	ecnode *d, *s;
	int i, found = 0, lr = token_num[LR_NUM], sm = token_num[SM_NUM], pdo = token_num[P_NUM];
	FN_CALLED;

	if( !e )
	{
		errlogSevPrintf( errlogFatal, "%s: ethcat/e NULL - init failed\n", __func__ );
		return FAIL;
	}

	if( !e->m )
	{
		errlogSevPrintf( errlogFatal, "%s: ethcat master NULL - domain init failed\n", __func__ );
		return FAIL;
	}

	if( !e->d )
	{
		errlogSevPrintf( errlogFatal, "%s: ethcat domain NULL - domain init failed\n", __func__ );
		return FAIL;
	}

	d = e->d;

	s = ecn_get_child_nr_type( e->m, token_num[S_NUM], ECNT_SLAVE );
	if( !s )
	{
		errlogSevPrintf( errlogFatal, "%s: cannot find slave %d\n", __func__, token_num[S_NUM] );
		return FAIL;
	}

	for( i = 0; !found && i < d->ddata.num_of_regs; i++ )
		if( d->ddata.reginfos[i].slave == s &&
				(sm < 0 || d->ddata.reginfos[i].sync->nr == sm ) &&
				(pdo < 0 || d->ddata.reginfos[i].pdo->nr == pdo )
			)
		{
				if( --lr < 0 )
				{
					found = 1;
					break;
				}
		}

	if( !found )
	{
		errlogSevPrintf( errlogFatal, "%s: cannot find local register %d in domain %d (base: slave %d, sync %d, pdo %d)\n", __func__,
														token_num[LR_NUM], d->nr, s->nr, sm, pdo );
		return FAIL;
	}

	*dreg_nr = i;
	dreg->offs = d->ddata.reginfos[i].byte;
	dreg->bit = d->ddata.reginfos[i].bit;
	dreg->bitlen = (token_num[T_NUM] < 0 ? d->ddata.reginfos[i].bit_length : 8*parse_datatype_get_len( token_num[T_NUM] ) );
	dreg->bitspec = token_num[B_NUM];
	dreg->byteoffs = token_num[O_NUM];
	dreg->bytelen = token_num[L_NUM];
	dreg->typespec = token_num[T_NUM];

	*pentry = d->ddata.reginfos[i].pdo_entry;
	if( !*pentry )
	{
		errlogSevPrintf( errlogFatal, "%s: Domain register %d appears to have no pdo entry associated\n", __func__, i );
		return FAIL;
	}

	return OK;
}



int drvGetEntryDesc( ethcat *e, domain_register *dreg, int *dreg_nr, ecnode **pentry, int *token_num )
{
	ecnode *d, *pe;
	int i;
	FN_CALLED;

	if( !e )
	{
		errlogSevPrintf( errlogFatal, "%s: ethcat/e NULL - init failed\n", __func__ );
		return FAIL;
	}

	if( !e->d )
	{
		errlogSevPrintf( errlogFatal, "%s: ethcat domain NULL - domain init failed\n", __func__ );
		return FAIL;
	}

	d = e->d;


	pe = ecn_get_pdo_entry_nr( 0, token_num[S_NUM], token_num[SM_NUM], token_num[P_NUM], token_num[E_NUM] );
	if( !pe )
	{
		errlogSevPrintf( errlogFatal, "%s: PDO entry s%d.sm%d.p%d.e%d not found\n", __func__,
				token_num[S_NUM], token_num[SM_NUM], token_num[P_NUM], token_num[E_NUM] );
		return FAIL;
	}

	if( !pe->domain_entry )
	{
		errlogSevPrintf( errlogFatal, "%s: PDO entry s%d.sm%d.p%d.e%d not registered in any domain\n",
													__func__, token_num[S_NUM], token_num[SM_NUM], token_num[P_NUM], token_num[E_NUM] );
		return FAIL;
	}

	if( pe->domain_entry->de_domain->nr != d->nr )
	{
		errlogSevPrintf( errlogFatal, "%s: PDO entry s%d.sm%d.p%d.e%d registered in other domain (%d)\n",
						__func__, token_num[S_NUM], token_num[SM_NUM], token_num[P_NUM], token_num[E_NUM], pe->domain_entry->de_domain->nr );
		return FAIL;
	}

	*pentry = pe;
	for( i = 0; i < d->ddata.num_of_regs; i++ )
		if( d->ddata.reginfos[i].pdo_entry == pe )
		{
			*dreg_nr = i;
			dreg->offs = d->ddata.reginfos[i].byte;
			dreg->bit = d->ddata.reginfos[i].bit;
			dreg->bitlen = (token_num[T_NUM] < 0 ? d->ddata.reginfos[i].bit_length : 8*parse_datatype_get_len( token_num[T_NUM] ) );
			dreg->bitspec = token_num[B_NUM];
			dreg->byteoffs = token_num[O_NUM];
			dreg->bytelen = token_num[L_NUM];
			dreg->typespec = token_num[T_NUM];

			return OK;
		}


	errlogSevPrintf( errlogFatal, "%s: Internal error - PDO entry s%d.sm%d.p%d.e%d registered in domain %d, but not in reg list\n",
												__func__, token_num[S_NUM], token_num[SM_NUM], token_num[P_NUM], token_num[E_NUM], pe->domain_entry->de_domain->nr );

	return FAIL;
}


int drvDomainExists( int mnr, int dnr )
{
	ecnode *m, *d;

	FN_CALLED;

	m = ecn_get_child_nr_type( ecroot, mnr, ECNT_MASTER );
	if( !m )
		return FAIL;

	d = ecn_get_child_nr_type( m, dnr, ECNT_DOMAIN );
	if( !d )
		return FAIL;

	return OK;
}

/*-------------------------------------------------------------------*/
/*                                                                   */
/* Startup configuration                                             */
/*                                                                   */
/*-------------------------------------------------------------------*/

long drvethercatConfigure(
    int domain_nr,
    double freq,
    int autoconfig,
    int autostart
)

{
    ethcat **ec;
    ecnode *m;
    EC_ERR retv;

	FN_CALLED;

    /* check arguments */
    if( domain_nr < 0 ||
    	freq < 0.001 || freq > 15000.0 ||
    	autoconfig < 0 || autoconfig > 1 ||
    	autostart < 0 || autostart > 1
   	)
    {
        printf( "----------------------------------------------------------------------------------\n" );
        printf( "Usage: drvethercatConfigure( domain_nr )\n\n");
        printf( " Argument        Desc\n");
        printf( " domain_nr       Number of the corresponding EtherCAT domain\n");
        printf( " frequency       EtherCAT frequency in Hz\n");
        printf( " autoconfig      Should driver autoconfigure the domain?\n");
        printf( " autostart       Should domain packet exchange be automatically started?\n");
        printf( " \nExample:\n");
        printf( " drvethercatConfigure 0, 1000, 1, 1\n");
        printf( "----------------------------------------------------------------------------------\n" );

        printf( "\n***** ecat2 driver called with drvethercatConfigure %d, %f, %d, %d\n",
        		domain_nr, freq, autoconfig, autostart );
        printf( "\n***** ecat2 EPICS driver not loaded!\n\n");
        return 0;
    }


    if( !ecroot )
    {
		/*--------------------------------- */
		/* create root and master           */
		ecroot = ecn_add_child_type( NULL, ECNT_ROOT );
		if( !ecroot )
		{
			errlogSevPrintf( errlogFatal, "%s: creating root node failed\n", __func__ );
			return ERR_OUT_OF_MEMORY;
		}

		ecroot->nr = 0;
    }

    if( !ecroot->child )
    {
		/* currently fixed to a single master */
		m = ecn_add_child_type( ecroot, ECNT_MASTER );
		if( !m )
		{
			errlogSevPrintf( errlogFatal, "%s: creating master node failed\n", __func__ );
			return ERR_OUT_OF_MEMORY;
		}

		m->nr = 0;

		/* allocate a master */
	    pinfo( PPREFIX "Requesting master..." );
	    if( !(m->mdata.master = ecrt_request_master( 0 )) )
	        perrret( "\nRequesting master 0 failed. EtherCAT Master not running or not responding.\n" );
	    pinfo( "succeeded\n" );
    }
    else
    	m = ecroot->child;

    /*-------------------------- */
    /*                           */
    /* slave config              */
    /*                           */
    /*-------------------------- */
    printf( PPREFIX "Configuring EL6692 entries start...\n" );
    configure_el6692_entries( m->mdata.master );
    printf( PPREFIX "Configuring EL6692 entries end.\n" );


    /* query master about the current config */
    if( !master_create_physical_config( m ) )
    {
		errlogSevPrintf( errlogFatal, "%s: creating master config failed\n", __func__ );
		return ERR_BAD_REQUEST;
    }

    printf( PPREFIX "Configuring slave(s) start...\n" );
    retv = execute_configuration_prg();
	if( retv != ERR_NO_ERROR )
	{
		errlogSevPrintf( errlogFatal, "%s: executing slave configuration program failed (error %d)\n", __func__, retv );
		return retv;
	}
    printf( PPREFIX "Configuring slave(s) end.\n" );


	/*--------------------------------- */
    /* create and init domain           */

    /* is this domain already registered? */
    for( ec = &ecatList; *ec; ec = &(*ec)->next )
    	if( (*ec)->dnr == domain_nr )
    	{
    		errlogSevPrintf( errlogFatal, "%s: EtherCAT domain %d has already been registered\n", __func__, domain_nr );
    		return ERR_ALREADY_EXISTS;
    	}


    *ec = malloc( sizeof(ethcat) );
	if( *ec == NULL )
	{
		errlogSevPrintf( errlogFatal, "%s: Memory allocation failed.\n", __func__ );
		return ERR_OUT_OF_MEMORY;
	}

    (*ec)->next = NULL;
    (*ec)->dnr = domain_nr;
	(*ec)->rw_lock = epicsMutexMustCreate();
	(*ec)->health_lock = epicsMutexMustCreate();
	(*ec)->irq_lock = epicsMutexMustCreate();
	(*ec)->rate = (long)((double)1000000000.0/(double)freq);
	(*ec)->m = m;
	(*ec)->irq = epicsEventMustCreate( epicsEventEmpty );


	/*---------------------------------- */
	if( !((*ec)->d = add_domain( m, (*ec)->rate )) )
	{
		errlogSevPrintf( errlogFatal, "%s: Domain init and autoconfig failed.\n", __func__ );
		return ERR_OUT_OF_MEMORY;
	}
	(*ec)->d->nr = domain_nr;
	(*ec)->d->ddata.sts_lock = epicsMutexMustCreate();
	(*ec)->r_data = (*ec)->d->ddata.rmem;
	(*ec)->w_data = (*ec)->d->ddata.wmem;
    (*ec)->w_mask = calloc( 1, (*ec)->d->ddata.dsize );
	if( !(*ec)->w_mask )
	{
		errlogSevPrintf( errlogFatal, "%s: allocating memory for domain wmask failed\n", __func__ );
		return ERR_OUT_OF_MEMORY;
	}
    (*ec)->irq_r_mask = calloc( 1, (*ec)->d->ddata.dsize );
	if( !(*ec)->irq_r_mask )
	{
		errlogSevPrintf( errlogFatal, "%s: allocating memory for domain irq rmask failed\n", __func__ );
		return ERR_OUT_OF_MEMORY;
	}

	/*---------------------------------- */
	/* init irq scanning */
    scanIoInit( &((*ec)->r_scan) );
    scanIoInit( &((*ec)->w_scan) );

    /* register atexit callback */
    /*epicsAtExit( drvethercatAtExit, *ec ); */

	/*---------------------------------- */
	printf( PPREFIX "\n" );
	printf( PPREFIX "Domain %d\n", domain_nr );
	printf( PPREFIX "Domain frequency %.1f Hz (1 network packet sent every %f ms)\n", freq, (double)((*ec)->rate)/(double)1000000 );
	printf( PPREFIX "Domain registers %d\n", (*ec)->d->ddata.num_of_regs );
	printf( PPREFIX "Domain size %d bytes (allocated %d bytes)\n", (*ec)->d->ddata.dsize, (*ec)->d->ddata.dallocated );
	printf( PPREFIX "Slave-to-slave entries %d\n", (*ec)->d->ddata.num_of_sts_entries );



	printf( PPREFIX "Domain %d configure process completed\n", domain_nr );
    return 0;
}












/*-------------------------------------------------------------------*/
/*                                                                   */
/* Boilerplate                                                       */
/*                                                                   */
/*-------------------------------------------------------------------*/

/*---------------------- */
/*                       */
/* Configure             */
/*                       */
static const iocshArg drvethercatConfigureArg0 = { "domainnr", 		iocshArgInt };
static const iocshArg drvethercatConfigureArg1 = { "freq", 			iocshArgDouble };
static const iocshArg drvethercatConfigureArg2 = { "autoconfig",	iocshArgInt };
static const iocshArg drvethercatConfigureArg3 = { "autostart", 	iocshArgInt };
static const iocshArg * const drvethercatConfigureArgs[] = {
    &drvethercatConfigureArg0,
    &drvethercatConfigureArg1,
    &drvethercatConfigureArg2,
    &drvethercatConfigureArg3
};

static const iocshFuncDef drvethercatConfigureDef =
    { "ecat2configure", 4, drvethercatConfigureArgs };

static void drvethercatConfigureFunc( const iocshArgBuf *args )
{
    drvethercatConfigure(
        args[0].ival,
        args[1].dval,
        args[2].ival,
        args[3].ival
    );
}

/*---------------------- */
/*                       */
/* dmap                  */
/*                       */
/*---------------------- */
static const iocshArg drvethercatDMapArg0 = { "cmd", 		iocshArgString };
static const iocshArg * const drvethercatDMapArgs[] = {
    &drvethercatDMapArg0,
};

static const iocshFuncDef drvethercatDMapDef =
    { "dmap", 1, drvethercatDMapArgs };

static void drvethercatDMapFunc( const iocshArgBuf *args )
{
    dmap(
        args[0].sval
    );
}



/*---------------------- */
/*                       */
/* stat                  */
/*                       */
/*---------------------- */
static const iocshArg drvethercatstatArg0 = { "dnr", 		iocshArgInt };
static const iocshArg * const drvethercatstatArgs[] = {
    &drvethercatstatArg0
};

static const iocshFuncDef drvethercatstatDef =
    { "stat", 1, drvethercatstatArgs };

static void drvethercatStatFunc( const iocshArgBuf *args )
{
    stat(
        args[0].ival
    );
}


/*---------------------- */
/*                       */
/* ecat2cfgEL6692        */
/*                       */
/*---------------------- */
static const iocshArg drvethercatConfigEL6692Arg[] = {
		{ "slave_pos",  	iocshArgInt },
		{ "io",  			iocshArgString },
		{ "bitlen", 		iocshArgInt }
};
static const iocshArg * const drvethercatConfigEL6692Args[] = {
    &drvethercatConfigEL6692Arg[0],
    &drvethercatConfigEL6692Arg[1],
    &drvethercatConfigEL6692Arg[2],
};
#if 1
static const iocshFuncDef drvethercatConfigEL6692Def =
    { "ecat2cfgEL6692", 3, drvethercatConfigEL6692Args };

static void drvethercatConfigEL6692Func( const iocshArgBuf *args )
{
    ConfigEL6692(
        args[0].ival,
        args[1].sval,
        args[2].ival
    );
}

#endif


/*---------------------- */
/*                       */
/* ecat2sts              */
/*                       */
/*---------------------- */
static const iocshArg drvethercatStSArg[] = {
		{ "from",  	iocshArgString },
		{ "to",  	iocshArgString },
};
static const iocshArg *const drvethercatStSArgs[] = {
    &drvethercatStSArg[0],
    &drvethercatStSArg[1],
};

static const iocshFuncDef drvethercatStSDef =
    { "ecat2sts", 2, drvethercatStSArgs };

static void drvethercatStSFunc( const iocshArgBuf *args )
{
    sts(
        args[0].sval,
        args[1].sval
    );
}



/*---------------------- */
/*                       */
/* si                    */
/*                       */
/*---------------------- */
static const iocshArg _si_args[] = {
		{ "level",  	iocshArgInt },
};
static const iocshArg *const si_args[] = {
    &_si_args[0],
    &_si_args[1],
};

static const iocshFuncDef si_fndef =
    { "si", 1, si_args };

static void si_fn( const iocshArgBuf *args )
{
    si(
        args[0].ival
    );
}


/*---------------------- */
/*                       */
/* ecat2cfgslave         */
/*                       */
/*---------------------- */


static const iocshArg drvethercatcfgslaveArg[] = {
		{ "cmd",  				iocshArgString },
		{ "s_nr",  				iocshArgInt },
		{ "sm_nr",  			iocshArgInt },
		{ "pdo_ix_dir",  		iocshArgInt },
		{ "entry_ix_wd_mode",	iocshArgInt },
		{ "entry_sub_ix",		iocshArgInt },
		{ "entry_bitlen",		iocshArgInt },

};
static const iocshArg *const drvethercatcfgslaveArgs[] = {
    &drvethercatcfgslaveArg[0],
    &drvethercatcfgslaveArg[1],
    &drvethercatcfgslaveArg[2],
    &drvethercatcfgslaveArg[3],
    &drvethercatcfgslaveArg[4],
    &drvethercatcfgslaveArg[5],
    &drvethercatcfgslaveArg[6],
};

static const iocshFuncDef drvethercatcfgslaveDef =
    { "ecat2cfgslave", 7, drvethercatcfgslaveArgs };

static void drvethercatcfgslaveFunc( const iocshArgBuf *args )
{
    cfgslave(
        args[0].sval,
        args[1].ival,
        args[2].ival,
        args[3].ival,
        args[4].ival,
        args[5].ival,
        args[6].ival
    );
}







/*===================================================== */
/*                                                      */
/* EPICS support structures                             */
/*                                                      */
/*===================================================== */

static void drvethercat2_registrar()
{
    iocshRegister( &drvethercatConfigureDef, drvethercatConfigureFunc );
    iocshRegister( &drvethercatDMapDef, drvethercatDMapFunc );
    iocshRegister( &drvethercatstatDef, drvethercatStatFunc );
	iocshRegister( &drvethercatConfigEL6692Def, drvethercatConfigEL6692Func );
    iocshRegister( &drvethercatStSDef, drvethercatStSFunc );
    iocshRegister( &drvethercatcfgslaveDef, drvethercatcfgslaveFunc );
    iocshRegister( &si_fndef, si_fn ); /* going for somewhat shorter names from now on :P */
}

epicsExportRegistrar( drvethercat2_registrar );





struct {
    long number;
    long (*report) ();
    long (*init) ();
} drvecat2 = {
    1,
    NULL, /*drvethercat_report,*/
    NULL
};






