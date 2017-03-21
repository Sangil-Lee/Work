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

#ifndef ECTOOLS_H
#define ECTOOLS_H



#define VENDOR_BECKHOFF		0x00000002
#define VENDOR_PSI			0x00505349


#define ESC_BOLD 		"\033[1m"
#define ESC_FG_RED		"\033[31m"
#define ESC_REVERSE		"\033[7m"
#define ESC_RESET 		"\033[0m"



char *strtoupper( char *s );
char *strtolower( char *s );

/*---------------------------------*/
long ConfigEL6692( int slave_pos, char *io, int bitlen );
void configure_el6692_entries( ec_master_t *master );
int slave_is_6692( int slave_pos );
int get_no_pdos_6692( int sm_num );
int get_pdo_info_6692( ec_master_t *ecm, int i, int j, int k, ec_pdo_info_t *pdo_t );
int get_pdo_entry_info_6692( ec_master_t *ecm, int i, int j, int k, int l, ec_pdo_entry_info_t *pdo_entry_t );

long dmap( char *cmd );
long stat( int dnr );

long sts( char *from, char *to );

long cfgslave( char *cmd, int slave_nr, int sm_nr, int pdo_ix_dir, int entry_ix_wd_mode, int entry_sub_ix, int entry_bitlen );
EC_ERR execute_configuration_prg( void );

long si( int level );


/*---------------------------------*/

#endif


















