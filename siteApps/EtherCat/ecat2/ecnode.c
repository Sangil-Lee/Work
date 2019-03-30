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

ecnode *ecn_add_child( ecnode *parent )
{
	ecnode *node = NULL /* to satisfy -Wall */, *nnode;

	if( parent )
		 node = parent->child;

	if( !(nnode = calloc( 1, sizeof(ecnode)) ))
		perrret( "%s: memory alloc failed", __func__ );

	nnode->nr = -1;
	nnode->parent = parent;

	if( !parent )
		return nnode; /* root */

	if( node )
	{
		while( node->next )
			node = node->next;
		node->next = nnode;
	}
	else
		parent->child = nnode;

	return nnode;
}


ecnode *ecn_add_child_type( ecnode *parent, ECN_TYPE type )
{
	ecnode *n = ecn_add_child( parent );
	if( !n )
		return n;
	n->type = type;
	return n;
}


int ecn_get_next_free_child_nr_type( ecnode *parent, ECN_TYPE type )
{
	ecnode *n = parent->child;
	int max = -1;

	if( !n )
		return 0;

	do {
		if( n->type != type )
			continue;
		if( n->nr > max )
			max = n->nr;
	}
	while( (n = n->next) );

	return max+1;
}

int ecn_get_first_free_child_nr_type( ecnode *parent, ECN_TYPE type )
{
	ecnode *n = parent->child;
	int trynr = 0, found;

	if( !n )
		return 0;

	do {
		found = 0;
		do {
			if( n->type != type )
				continue;
			if( n->nr == trynr )
			{
				found = 1;
				break;
			}
		} while( (n = n->next) );
		if( !found )
			return trynr;
		trynr++;
	} while( trynr < 10000 ); /* just in case */
	perr( "%s: cannot find free node nr (type %d)\n", __func__, type );
	return -1;
}

ecnode *ecn_get_child_nr( ecnode *parent, int nr )
{
	ecnode *n;
	int sl = 0;

	if( !parent )
		perrret( "%s: parent NULL\n", __func__ );
	if( !parent->child )
		perrret( "%s: parent->child NULL\n", __func__ );

	n = parent->child;

	do {
		sl++;
		if( n->nr == nr )
			return n;
	}
	while( (n = n->next) );

	pinfo( "%s: child nr. %d not found (%d slaves visited)\n", __func__, nr, sl );
	return NULL;
}

ecnode *ecn_get_next_child_nr( ecnode *parent, int *last_nr )
{
	ecnode *n = parent->child;
	int sl = 0, found_last = 0;


	if( !parent || !n )
		perrret( "%s: parent or parent->child NULL\n", __func__ );

	do {
		sl++;

		/* get the very first one */
		if( *last_nr < 0 )
		{
			*last_nr = n->nr;
			return n;
		}

		if( n->nr == *last_nr )
		{
			found_last = 1;
			continue;
		}

		if( !found_last )
			continue;

		*last_nr = n->nr;
		return n;
	}
	while( (n = n->next) );

	return NULL;
}



ecnode *ecn_get_child_nr_type( ecnode *parent, int nr, ECN_TYPE type )
{
	ecnode *n;
	if( !parent )
		return NULL;
	n = parent->child;
	if( !n )
		return NULL;

	do {
		if( n->nr == nr && n->type == type )
			return n;
	}
	while( (n = n->next) );

	return NULL;
}





ecnode *ecn_get_master_nr( int m_nr )
{
    return ecn_get_child_nr_type( ecroot, m_nr, ECNT_MASTER );
}

ecnode *ecn_get_slave_nr( int m_nr, int s_nr )
{
    return ecn_get_child_nr_type( ecn_get_master_nr( m_nr ), s_nr, ECNT_SLAVE);
}

ecnode *ecn_get_sync_nr( int m_nr, int s_nr, int sm_nr )
{
    return ecn_get_child_nr_type( ecn_get_slave_nr( m_nr, s_nr), sm_nr, ECNT_SYNC) ;
}

ecnode *ecn_get_pdo_nr( int m_nr, int s_nr, int sm_nr, int p_nr )
{
    return ecn_get_child_nr_type( ecn_get_sync_nr( m_nr, s_nr, sm_nr), p_nr, ECNT_PDO) ;
}

ecnode *ecn_get_pdo_entry_nr( int m_nr, int s_nr, int sm_nr, int p_nr, int pe_nr )
{
    return ecn_get_child_nr_type( ecn_get_pdo_nr( m_nr, s_nr, sm_nr, p_nr), pe_nr, ECNT_PDO_ENTRY) ;
}


ecnode *ecn_get_domain_nr( int m_nr, int d_nr )
{
    return ecn_get_child_nr_type( ecn_get_master_nr( m_nr ), d_nr, ECNT_DOMAIN );
}

ecnode *ecn_get_domain_entry_nr( int m_nr, int d_nr, int de_nr )
{
    return ecn_get_child_nr_type( ecn_get_domain_nr( m_nr, d_nr ), de_nr, ECNT_DOMAIN_ENTRY );
}



ecnode *ecn_get_first_sync_nr(
        ecnode *m,
        int s_nr,
        int sm_nr

)
{
	ecnode *temp;
	ecnode *slave = ecn_get_child_nr( m, s_nr );
	if( !slave )
		perrret( "%s: get slave nr %d failed\n", __func__, s_nr );
	temp = ecn_get_child_nr( slave, sm_nr );
	if( !temp )
			perrret( "%s: get sync nr %d failed\n", __func__, sm_nr );

    return( temp );
}

ecnode *ecn_get_next_sync_nr(
        ecnode *m,
        int s_nr,
        int *last_sm_nr

)
{
	ecnode *temp;
	ecnode *slave = ecn_get_child_nr( m, s_nr );
	if( !slave )
		perrret( "%s: get slave nr %d failed\n", __func__, s_nr );

	temp = ecn_get_next_child_nr( slave, last_sm_nr );
	if( !temp )
			perrret( "%s: get sync nr %d failed\n", __func__, *last_sm_nr );


    return( temp );
}


void ecn_remove_node( ecnode *node )
{
	ecnode *n;
	if( !node )
		return;

	if( node->path )
		free( node->path );

	/* free domain specific memory */
	if( node->type == ECNT_DOMAIN )
	{
		if( node->ddata.reginfos )
			free( node->ddata.reginfos );
		if( node->ddata.regs )
			free( node->ddata.regs );
		if( node->ddata.dmem && node->ddata.dallocated > 0 )
		{
			free( node->ddata.dmem );
			free( node->ddata.rmem );
			free( node->ddata.wmem );
			node->ddata.dmem = node->ddata.rmem = node->ddata.wmem = NULL;
			node->ddata.dallocated = node->ddata.dsize = 0;
		}
		if( node->ddata.sts && node->ddata.num_of_sts_entries )
			free( node->ddata.sts );
	}


	if( node->de_pdo_entry )
	{
		node->de_pdo_entry->domain_entry = NULL;
		node->de_domain = NULL;
		node->de_master = NULL;
		node->de_slave = NULL;
		node->de_sync = NULL;
		node->de_pdo = NULL;
		node->de_pdo_entry = NULL;
	}

	if( node->parent )
	{
		if( node->parent->child == node )
			node->parent->child = (node->next ? node->next : NULL);
		else
		{
			n = node->parent->child;
			do
			{
				if( node == n->next )
				{
					n->next = node->next;
					break;
				}
			} while( (n = n->next) );
		}
	}
	free( node );

}


void ecn_delete_branch( ecnode *node )
{
	if( !node )
		return;

	while( node->child )
		ecn_delete_branch( node->child );

	ecn_remove_node( node );
}


void ecn_delete_children( ecnode *node )
{
	if( !node )
		return;

	while( node->child )
		ecn_delete_branch( node->child );
}






void ep_print_tree( ecnode *root )
{
	int i;
	static int depth = 0;
	static char *ntypes[] = {
			"root  ",
			"master",
			"slave ",
			"smgr  ",
			"pdo   ",
			"entry ",
			"domain",
			"dentry",
			"error ",
			"error ",
			"error "
	};
	ecnode *next, *node = root;
	if( !node )
		perrret2( "%s: called with NULL\n", __func__ );

	depth++;
	do
	{
		next = node->next;
		pinfo( "\n" );
		for( i = 0; i < depth; i++ )
			pinfo( "         " );
		pinfo( "[%s %d]", ntypes[node->type], node->nr );

		if( node->child )
			ep_print_tree( node->child );

	} while( (node = next) );
	depth--;
}






