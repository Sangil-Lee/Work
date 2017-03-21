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


#ifndef ECNODE_H
#define ECNODE_H

ecnode *ecn_add_child( ecnode *parent );
ecnode *ecn_add_child_type( ecnode *parent, ECN_TYPE type );
void ecn_remove_node( ecnode *node );
void ecn_delete_branch( ecnode *n );
void ecn_delete_children( ecnode *node );

ecnode *ecn_get_child_nr( ecnode *parent, int nr );
ecnode *ecn_get_next_child_nr( ecnode *parent, int *last_nr );
ecnode *ecn_get_child_nr_type( ecnode *parent, int nr, ECN_TYPE type );
int ecn_get_first_free_child_nr_type( ecnode *parent, ECN_TYPE type );



ecnode *ecn_get_domain_nr( int m_nr, int d_nr );
ecnode *ecn_get_pdo_entry_nr( int m_nr, int s_nr, int sm_nr, int p_nr, int pe_nr );

#endif


















