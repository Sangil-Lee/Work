
#include <stdio.h>
#include <string.h>
#include <ctype.h>


#include "inc/com_def.h"
#include "ntshell/ntshell_main.h"

#include "shellcmd.h"
#include "ts2api.h"

#define CMD_MEM_DUMP_SIZE (256)

static void help(void);

void usr_ntshell_close(void);

#if (OS_TYPE == XILINX) || (OS_TYPE == VXWORKS_RTP) || (OS_TYPE == VXWORKS)
void mem_dump(int argc, char **argv);
void r32( int argc, char **argv );
void r16( int argc, char **argv );
void r8 ( int argc, char **argv );
void w32( int argc, char **argv );
void w16( int argc, char **argv );
void w8 ( int argc, char **argv );
#endif

//==============================================================================

const command_table_t shell_cmdlist[] = {
    {"exit"               ,"exit console"       , (void*)usr_ntshell_close},
    {"quit"               ,"exit console"       , (void*)usr_ntshell_close},
    {"help"               ,"help command"       , (void*)help},
    {"?"                  ,"help command"       , (void*)help},
#if (OS_TYPE == XILINX) || (OS_TYPE == VXWORKS_RTP) || (OS_TYPE == VXWORKS)    
    {"d"                  ,"mem_dump"           , (void*)mem_dump },
    {"r32"                ,"read_32bit"         , (void*)r32 },
    {"r16"                ,"read_16bit"         , (void*)r16 },
    {"r8"                 ,"read_8bit"          , (void*)r8 },
    {"w32"                ,"write_32bit"        , (void*)w32 },
    {"w16"                ,"write_16bit"        , (void*)w16 },
    {"w8"                 ,"write_8bit"         , (void*)w8 },
#endif    
    {"version"            ,"version"            , (void*)version_print },
//==============================================================================    
    {"fled"             ,"ts2api_setLed"                , (void*)ts2api_setLed },
    {"cmd"              ,"ts2api_shellCmd"              , (void*)ts2api_shellCmd },
    {"uTrg"             ,"ts2api_usrTrg"                , (void*)ts2api_usrTrg },
    {"uev"              ,"ts2api_usrEvCode"             , (void*)ts2api_usrEvCode },
    {"cps"              ,"ts2api_cpsCtrl"               , (void*)ts2api_cpsCtrl },
    {"cpsT"              ,"ts2api_cpsTest"               , (void*)ts2api_cpsTest },    
    {"seqR"             ,"ts2api_seqResume"             , (void*)ts2api_seqResume },
    
    
    
    // {"Start"              ,"Start"              , (void*)startMyapp },
    // {"Stop"               ,"Stop"               , (void*)stopMyapp },
    // {"init"               ,"init"               , (void*)initGtpCore },
    // {"prn"                ,"prn"                , (void*)prnStatus },

//==============================================================================
    {NULL       , NULL          , NULL},
};


//void xtu_lcd_mcmd(int argc, char **argv);
//void xtu_lcd_data_mwr(int argc, char **argv);

//STATUS xtu_StartShotNow( void);
//STATUS xtu_StopShotNow( void);


static void help(void)
{
	int i=0;
	for(i=0;shell_cmdlist[i].command != NULL;i++)
	{
		prnM2("%s\t:\t\t%s\r\n",shell_cmdlist[i].command,shell_cmdlist[i].description);
	}
}



void get_shell_cmd_list(void *cmd_list)
{
  memcpy(cmd_list,shell_cmdlist,sizeof(shell_cmdlist));
}



void version_print (void)
{
  char pdate[50];
  char ptime[50];
  char version[100];

  sprintf(pdate, __DATE__);
  sprintf(ptime, __TIME__);

  prnM2 ("------------------------------------------------------\r\n");
  prnM2 ("FW Build Date[M/D/Y]	: ");  prnM2(pdate); prnM2 ("\r\n");
  prnM2 ("FW Build Time[H/M/S]	: ");  prnM2(ptime); prnM2 ("\r\n");
  prnM2 ("------------------------------------------------------\r\n");
  prnM2 ("DURUTRONIX Inc.\r\n");
  prnM2 ("\r\n\r\n");

  sprintf(version,"[M/D/Y]: %s, [H/M/S]: %s", __DATE__, __TIME__);
}


void usr_ntshell_close(void)
{
  //do not edit below ---------------------
  ntshell_close();
}

//==========================================================================//
// Convert String to unsigned int type.
//==========================================================================//
unsigned int str_to_uint( const char *p, char **out_p, int base )
{
  unsigned int v = 0;

  while( isspace((int)*p))
  {
    p++;
  }

  if(((base == 16) || (base == 0)) && ((*p == '0') && ((p[1] == 'x') || (p[1] =='X'))))
  {
    p +=2;
    base = 16;
  }

  if( base == 0)
  {
    if( *p == '0' )
    {
      base = 8;
    }
    else
    {
      base = 10;
    }
  }

  while(1)
  {
    char c = *p;

    if((c >= '0') && (c <= '9') && ( c - '0' < base ))
    {
      v = ( v * base ) + ( c - '0' );
    }
    else if( ( c >= 'a' ) && ( c <= 'z' ) && ( c- 'a' + 10 < base ))
    {
      v = ( v * base ) + (c - 'a' + 10);
    }
    else if(( c >= 'A') && ( c <= 'Z' ) && ( c - 'A' + 10 < base ))
    {
      v = ( v * base ) + (c - 'A' + 10);
    }
    else
    {
      break;
    }

    p++;
  }

  if( out_p)
  {
    *out_p = (char*)p;
  }

  return v;
}




/*
d 0xe8000000
*/
//==========================================================================//
// Basic Memory operation commnad
//==========================================================================//
static int user_mem_offset = -1;

#if (OS_TYPE == XILINX) || (OS_TYPE == VXWORKS_RTP) || (OS_TYPE == VXWORKS)
void set_shell_mem_offset(int offset)
{
  prnM1("user_mem_offset : 0x%08x\r\n",user_mem_offset);
  user_mem_offset = offset;
  prnM1("user_mem_offset : 0x%08x\r\n",user_mem_offset);
}


void mem_dump(int argc, char **argv)
{
    static unsigned int address = 0;
    unsigned int i,j;
    unsigned char byte;

    if( argc >= 2 )
    {
        address = 0xFFFFFFF0 & str_to_uint(argv[1], NULL, 0);
    }

    if(user_mem_offset == -1)return;

		address = user_mem_offset + address;


    // dump here.
    for( i = 0 ; i < CMD_MEM_DUMP_SIZE; i += 16)
    {
        prnM1("\r\n");
        prnM1("0x%08x :", address );
        for( j = 0; j < 16; j++ )
        {
            byte = READ8( address );
            address++;
            prnM1(" %02x", byte);
        }
    }
    prnM1("\r\n");
}

void r32( int argc, char **argv )
{
    unsigned int add;
    unsigned int data;

    if(argc < 2)
    {
        printf("Argument Count Error!\r\n");
        return;
    }

    if(user_mem_offset == -1)return;
 
    add  = str_to_uint(argv[1], NULL, 0) + user_mem_offset;
    data = READ32(add);
    printf("[RD32] address = 0x%08x, data = 0x%08x \r\n", add, data);
}

void r16( int argc, char **argv )
{
    unsigned int add;
    unsigned int data;

    if(argc < 2)
    {
        prnM1("Argument Count Error!\r\n");
        return;
    }

    if(user_mem_offset == -1)return;
 
    add  = str_to_uint(argv[1], NULL, 0)+ user_mem_offset;
    data = READ16(add);
    prnM1("[RD16] address = 0x%08x, data = 0x%04x \r\n", add, data);
}

void r8( int argc, char **argv )
{
    unsigned int add;
    unsigned int data;

    if(argc < 2)
    {
        prnM1("Argument Count Error!\r\n");
        return;
    }

    if(user_mem_offset == -1)return;
 
    add  = str_to_uint(argv[1], NULL, 0)+ user_mem_offset;
    data = READ8(add);
    prnM1("[RD08] address = 0x%08x, data = 0x%02x \r\n", add, data);
}


void w32( int argc, char **argv )
{
    unsigned int add;
    unsigned int data;

    if(argc < 3)
    {
        prnM1("Argument Count Error!\r\n");
        return;
    }

    if(user_mem_offset == -1)return;
 
    add  = str_to_uint(argv[1], NULL, 0)+ user_mem_offset;
    data = (unsigned int)str_to_uint(argv[2], NULL, 0);
    WRITE32(add, data);
}

void w16( int argc, char **argv )
{
    unsigned int add;
    unsigned short int data;

    if(argc < 3)
    {
        prnM1("Argument Count Error!\r\n");
        return;
    }

    if(user_mem_offset == -1)return;
 
    add  = str_to_uint(argv[1], NULL, 0)+ user_mem_offset;
    data = (unsigned short int)str_to_uint(argv[2], NULL, 0);
    WRITE16(add, data);
}

void w8( int argc, char **argv )
{
    unsigned int add;
    unsigned char data;

    if(argc < 3)
    {
        prnM1("Argument Count Error!\r\n");
        return;
    }

    if(user_mem_offset == -1)return;
 
    add  = str_to_uint(argv[1], NULL, 0)+ user_mem_offset;
    data = (unsigned char)str_to_uint(argv[2], NULL, 0);
    WRITE8(add, data);
}

#endif