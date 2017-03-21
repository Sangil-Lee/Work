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


ecnode *ecroot = NULL;
/*------------------------------------------------------------------- */
#define PRINT_PHYS_CONFIG 0

int master_create_physical_config( ecnode *m )
{
    int i, j, k, l, sync_count, pdo_count, entry_count;
    ecnode *slave, *sm, *pdo, *pdoe;
    ec_pdo_entry_info_t pdo_entry;
    ec_master_t *ecm = m->mdata.master;

    if( ecrt_master( ecm, &m->mdata.master_info ) != 0)
        perrret( "%s: Getting master info failed\n", __func__ );

    memcpy( &m->mdata.master_info_at_start, &m->mdata.master_info, sizeof(ec_master_info_t) );

#if PRINT_PHYS_CONFIG
    pinfo( "-------------------------\n" );
    pinfo( "EtherCAT physical config:\n" );
    pinfo( "-------------------------\n" );
#endif
    for( i = 0; i < m->mdata.master_info.slave_count; i++ )
    {


    	if( slave_has_static_config( i ) )
        {
        	pinfo( PPREFIX "Slave %d: Static config found, skipping autoconfig\n", i );
        	continue;
        }

        if( !(slave = ecn_add_child_type( m, ECNT_SLAVE )) )
            perrret( "%s: adding slave %d failed\n", __func__, i );
    	slave->nr = i;

    	if( ecrt_master_get_slave( ecm, i, &slave->slave_t ) )
            perrret( "%s: cannot get slave %d info\n", __func__, i );
    	memcpy( &slave->sdata.config_slave_info, &slave->slave_t, sizeof(ec_slave_info_t) );
    	slave->sdata.check = 1;


		sync_count = slave->slave_t.sync_count;

#if PRINT_PHYS_CONFIG
        pinfo( "   Slave %d: SMs %d, alias %d, vendor id 0x%08x, revision nr 0x%08x, product code 0x%08x, sernr 0x%08x\n",
                i,
                sync_count,
                slave->slave_t.alias,
                slave->slave_t.vendor_id,
                slave->slave_t.revision_number,
                slave->slave_t.product_code,
                slave->slave_t.serial_number
             );
#endif



		if( slave_is_6692( i ) >= 0 )
    	{
    		/* special case - EL6692 */
			printf( PPREFIX "EL6692 at pos %d:\n", i );

	        for( j = 0; j < sync_count; j++ )
	        {
	        	if( !(sm = ecn_add_child_type( slave, ECNT_SYNC )) )
	        		return 0;
	        	sm->nr = j;
	        	if( ecrt_master_get_sync_manager( ecm, i, j, &sm->sync_t ) )
	            	perrret( "%s: (EL6692) cannot get slave %d, sync mgr %d info\n", __func__, i, j );

	        	pdo_count = get_no_pdos_6692( j );

	        	for( k = 0; k < pdo_count; k++ )
	            {
	            	if( !(pdo = ecn_add_child_type( sm, ECNT_PDO )) )
	            			return 0;
	            	pdo->nr = k;

	                if( !get_pdo_info_6692( ecm, i, j, k, &pdo->pdo_t ) )
	                {
	                	printf( "%s: (EL6692) cannot get slave %d, sync mgr %d, pdo %d info\n", __func__, i, j, k );
	                	continue;
	                }

	                entry_count = pdo->pdo_t.n_entries;

	#if PRINT_PHYS_CONFIG
	                pinfo( "         PDO %d: Entries %d, index 0x%04x\n",
	                        k,
	                        entry_count,
	                        pdo->pdo_t.index
	                     );
	#endif

	                for( l = 0; l < entry_count; l++ )
	                {
	                    if( !get_pdo_entry_info_6692( ecm, i, j, k, l, &pdo_entry )  )
	                    	perrret( "%s: (EL6692) cannot get slave %d, sync mgr %d, pdo %d, pdoe_entry %d info\n", __func__, i, j, k, l );

	                    if( !pdo_entry.index )
							continue;

	                    if( !(pdoe = ecn_add_child_type( pdo, ECNT_PDO_ENTRY )) )
	                    	return 0;;
	                	pdoe->nr = l;
	                	memcpy( &pdoe->pdo_entry_t, &pdo_entry, sizeof(ec_pdo_entry_info_t) );

	#if PRINT_PHYS_CONFIG
	                    pinfo( "            Entry %d: index 0x%04x, subindex %d, bit length %d\n",
	                            l,
	                            pdo_entry.index,
	                            pdo_entry.subindex,
	                            pdo_entry.bit_length
	                         );
	#endif
	                }

	            }

	        }

	        continue;
    	}



        for( j = 0; j < sync_count; j++ )
        {
        	if( !(sm = ecn_add_child_type( slave, ECNT_SYNC )) )
        		return 0;
        	sm->nr = j;
        	if( ecrt_master_get_sync_manager( ecm, i, j, &sm->sync_t ) )
            	perrret( "%s: cannot get slave %d, sync mgr %d info\n", __func__, i, j );

            pdo_count = sm->sync_t.n_pdos;

#if PRINT_PHYS_CONFIG
            pinfo( "      SM %d: PDOs %d, index 0x%04x, direction: %s, WD mode: %s\n",
                    j,
                    pdo_count,
                    sm->sync_t.index,
                    sm->sync_t.dir == 1 ? "Output" : "Input",
                    sm->sync_t.watchdog_mode == 0 ? "Default" : (sm->sync_t.watchdog_mode == 1 ? "Enabled" : "Disabled")
                 );
#endif


            for( k = 0; k < pdo_count; k++ )
            {
            	if( !(pdo = ecn_add_child_type( sm, ECNT_PDO )) )
            			return 0;
            	pdo->nr = k;
                if( ecrt_master_get_pdo( ecm, i, j, k, &pdo->pdo_t ) )
                	perrret( "%s: cannot get slave %d, sync mgr %d, pdo %d info\n", __func__, i, j, k );

                entry_count = pdo->pdo_t.n_entries;

#if PRINT_PHYS_CONFIG
                pinfo( "         PDO %d: Entries %d, index 0x%04x\n",
                        k,
                        entry_count,
                        pdo->pdo_t.index
                     );
#endif

                for( l = 0; l < entry_count; l++ )
                {
                    if( ecrt_master_get_pdo_entry( ecm, i, j, k, l, &pdo_entry )  )
                    	perrret( "%s: cannot get slave %d, sync mgr %d, pdo %d, pdoe_entry %d info\n", __func__, i, j, k, l );

                    if( !pdo_entry.index )
						continue;

                    if( !(pdoe = ecn_add_child_type( pdo, ECNT_PDO_ENTRY )) )
                    	return 0;;
                	pdoe->nr = l;
                	memcpy( &pdoe->pdo_entry_t, &pdo_entry, sizeof(ec_pdo_entry_info_t) );

#if PRINT_PHYS_CONFIG
                    pinfo( "            Entry %d: index 0x%04x, subindex %d, bit length %d\n",
                            l,
                            pdo_entry.index,
                            pdo_entry.subindex,
                            pdo_entry.bit_length
                         );
#endif
                }


            }
        }

    }


    pinfo( PPREFIX "Slaves added: %d\n", m->mdata.master_info.slave_count );


    /*print_pe(); */
    return 1;
}



void ecn_count_pdo_entries(
	ecnode *node,
	int *count
)
{
	if( !node || !count )
		return;

	do
	{
		if( node->child )
			ecn_count_pdo_entries( node->child, count );

		if( node->type == ECNT_PDO_ENTRY )
			if( node->pdo_entry_t.index && node->pdo_entry_t.bit_length )
				(*count)++;
	} while( (node = node->next) );

}



int domain_create_autoconfig( ecnode *d )
{
    int ix = 0, ndc = 0;
    ecnode *m = d->ddata.nmaster;
    ecnode *slave, *sync, *pdo, *pdo_entry, *dom_entry;
	ec_pdo_entry_reg_t *dc;

	d->ddata.dcfgtype = DCT_NOT_CONFIGURED;
	pinfo( PPREFIX  "%s: Autoconfiguring domain...\n", __func__ );

	ecn_count_pdo_entries( ecn_get_child_nr_type( ecroot, 0, ECNT_MASTER ), &ndc );
	if( !ndc )
		perrret( "%s: No PDO entries found, cancelling autoconfig domain\n", __func__ );

    if( !(d->ddata.regs = (ec_pdo_entry_reg_t *)calloc( ndc+1, sizeof(ec_pdo_entry_reg_t) )) )
        perrret( "%s: Memory allocation for domain config failed\n", __func__ );
    if( !(d->ddata.reginfos = (domain_reg_info *)calloc( ndc+1, sizeof(domain_reg_info) )) )
        perrret( "%s: Memory allocation for domain config failed\n", __func__ );
	d->ddata.num_of_regs = ndc;

	slave = m->child;

	walk( slave, m )
    {

        if( !slave->child )
        	continue;

        walk( sync, slave )
        {
            if( !sync->child )
            	continue;

        	walk( pdo, sync )
            {
            	if( !pdo->child )
            		continue;

                walk( pdo_entry, pdo )
                {
                	if( !pdo_entry->pdo_entry_t.index )
                		continue;


                	dc = &d->ddata.regs[ix];
					dc->alias 			= slave->slave_t.alias;
					dc->position 		= slave->slave_t.position;
					dc->vendor_id		= slave->slave_t.vendor_id;
					dc->product_code 	= slave->slave_t.product_code;
					dc->index 			= pdo_entry->pdo_entry_t.index;
					dc->subindex 		= pdo_entry->pdo_entry_t.subindex;
					dc->offset 			= &d->ddata.reginfos[ix].byte;
					dc->bit_position 	= &d->ddata.reginfos[ix].bit;

			    	dom_entry = ecn_add_child_type( d, ECNT_DOMAIN_ENTRY );
			    	dom_entry->nr = ix;

					dom_entry->de_master 	 = d->ddata.reginfos[ix].master    = m;
					dom_entry->de_domain 	 = d->ddata.reginfos[ix].domain    = d;
					dom_entry->de_slave 	 = d->ddata.reginfos[ix].slave     = slave;
					dom_entry->de_sync 	     = d->ddata.reginfos[ix].sync      = sync;
					dom_entry->de_pdo 		 = d->ddata.reginfos[ix].pdo       = pdo;
					dom_entry->de_pdo_entry  = d->ddata.reginfos[ix].pdo_entry = pdo_entry;
					d->ddata.reginfos[ix].bit_length = pdo_entry->pdo_entry_t.bit_length;

			    	pdo_entry->domain_entry =
			    	d->ddata.reginfos[ix].domain_entry = dom_entry;

					ix++;
                }
            }
        }

    }
	d->ddata.dcfgtype = DCT_CONFIGURED | DCT_AUTOCONFIG;


    return ix;
}





ecnode *add_domain( ecnode *m, int rate )
{
	int retv, nr = ecn_get_first_free_child_nr_type( m, ECNT_DOMAIN );
	int size, nregs;
	ecnode *d = ecn_add_child_type( m, ECNT_DOMAIN );
	if( !d )
		perrret( "%s: failed", __func__ );

	d->nr = nr;
	d->ddata.nmaster = m;
	d->ddata.dmem = d->ddata.rmem = d->ddata.wmem = NULL;

	if( !(d->domain_t = ecrt_master_create_domain( m->mdata.master )) )
        perrret( "%s: Domain creation failed!\n", __func__ );

    pinfo( PPREFIX "%s: Domain %d creation succeeded\n", __func__, d->nr );

    if( !(nregs = domain_create_autoconfig( d )) )
        return 0;
	pinfo( PPREFIX "Domain %d: Autoconfig found %d entries\n", d->nr, nregs );

#if 0
	int i;
	for( i = 0; i < nregs; i++ )
	{
		printf( "%d.  alias %d, pos %d, vendor 0x%x, pcode 0x%x, 0x%04x:%02x, blen=%d\n",
				i,
				d->ddata.regs[i].alias,
				d->ddata.regs[i].position,
				d->ddata.regs[i].vendor_id,
				d->ddata.regs[i].product_code,
				d->ddata.regs[i].index,
				d->ddata.regs[i].subindex,
				d->ddata.reginfos[i].bit_length
				);
	}
#endif
	retv = ecrt_domain_reg_pdo_entry_list( d->domain_t, d->ddata.regs );
	if( retv)
		perrret( "%s: ecrt_domain_reg_pdo_entry_list failed!\n", __func__ );

	size = ecrt_domain_size( d->domain_t );
	if( size <= 0 )
		perrret( "%s: ecrt_domain_size() returned %s value %d\n", __func__, !size ? "zero" : "negative", size );

	d->ddata.dsize = size;
	if( size % EC_PAGE_SIZE )
		size += (EC_PAGE_SIZE - size % EC_PAGE_SIZE);
	d->ddata.dallocated = size;


#ifdef DOMAIN_EXT_MEM
	if( !(d->ddata.dmem = (char *)calloc( 1, size )) )
        perrret( "%s: memory allocation for domain config failed\n", __func__ );
#endif
	if( !(d->ddata.rmem = (char *)calloc( 1, size )) )
        perrret( "%s: memory allocation for domain config failed\n", __func__ );
    if( !(d->ddata.wmem = (char *)calloc( 1, size )) )
        perrret( "%s: memory allocation for domain config failed\n", __func__ );
/*	pinfo( "%s: domain size %d bytes (allocated %d pages, %d bytes)\n", __func__, d->ddata.dsize, size/EC_PAGE_SIZE, size ); */


#ifdef DOMAIN_EXT_MEM
	ecrt_domain_external_memory( d->domain_t, d->ddata.dmem );
#endif

	/*domain_print_autoconfig( d ); */
	d->ddata.rate = rate;

#if 1
    if( ecrt_master_activate( m->mdata.master ) )
		perrret( "Master %d activation failed\n", m->nr );

/*    st_start( 0 ); */

#ifndef DOMAIN_EXT_MEM
	d->ddata.dmem = (char *)ecrt_domain_data( d->domain_t );
#endif

#endif

	if( !d->ddata.dmem )
		perrret( "%s: cannot get or allocate domain memory\n", __func__ );


    return d;
}

