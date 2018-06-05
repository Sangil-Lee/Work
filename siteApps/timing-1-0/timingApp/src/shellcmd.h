#if !defined(__SHELLCMD_H__)
#define __SHELLCMD_H__
/******************************************************************************
 *  Copyright (c) 2008 ~ by OLZETEK. All Rights Reserved.                     *
 *                                                                            *
 ******************************************************************************/

/*==============================================================================
                        EDIT HISTORY FOR MODULE

when       who     what, where, why
YY/MM/DD
--------   ------- ------------------------------------------------------------
2014-11-10 lay_kim modify for linux
08/12/23   dhpark  Created.
==============================================================================*/

#ifdef __cplusplus
extern "C" {
#endif

void get_shell_cmd_list(void *cmd_list);
void set_shell_mem_offset(int offset);
void version_print (void);
unsigned int str_to_uint( const char *p, char **out_p, int base );


#ifdef __cplusplus
}
#endif



#endif /* __SHELLCMD_H__ */
