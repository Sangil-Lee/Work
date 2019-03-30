/*******************************************************************************
 *                                                                             *
 *  Copyright (c) 2014 ~ by DURUTRONIX. All Rights Reserved.                   *
 *                                                                             *
 ******************************************************************************/

/*==============================================================================
                        EDIT HISTORY FOR MODULE

when                    who         what, where, why
----------------------  ----------  --------------------------------------------
2018-05-07 15:19:42     laykim     Modify
2015-05-29 15:19:42     laykim     Modify
==============================================================================*/
#if !defined( __COM_DEF_VER_H__ )
#define __COM_DEF_VER_H__


#ifdef __cplusplus
extern "C" {
#endif

//==============================================================================
//$$ OS_TYPE define
//==============================================================================
// WINDOWS : Windows Family
// LINUX   : Linux Series
// VXWORKS : VxWorks

#define WINDOWS     0x8080
#define LINUX       0x8081
#ifdef	UNIX
#undef  UNIX
#endif
#define UNIX        0x8082
#define VXWORKS     0x8083
#define QNX         0x8084
#define XILINX      0x8085
#define VXWORKS_RTP 0x8086

#define OS_TYPE   LINUX




//==============================================================================
//$$ includes
//==============================================================================
#include <stdio.h>

#if OS_TYPE == VXWORKS
  #include <vxWorks.h>
#endif

//==============================================================================
//$$ Defines
//==============================================================================
//#define __DEBUG_0__ // Enable all
//#define __DEBUG_1__ // Enable only logmsg level 1, prnErr, prnMeta, prnRLS
#define __DEBUG_2__ // Enable prnErr, prnMeta, prnRLS
//#define __DEBUG_3__ // Enable prnErr, prnMeta, prnRLS
//#define __DEBUG_9__ // Enable only prnRLS
//#define __RELEASE__ // Remove all logmsg.



#ifndef STATUS  //2014-11-11 ���� 5:46:23 update by laykim
  typedef unsigned int STATUS;
#endif


//==============================================================================
//$$ Type Defines
//==============================================================================
#if OS_TYPE == XILINX
  typedef signed   char      INT8;
  typedef signed   short int INT16;
  typedef signed   int       INT32;
  typedef signed   long long INT64;
  typedef unsigned char      UINT8;
  typedef unsigned short int UINT16;
  typedef unsigned int       UINT32;
  typedef unsigned long long UINT64;
#endif


#if OS_TYPE == LINUX 
  typedef signed   char      INT8;
  typedef signed   short int INT16;
  typedef signed   int       INT32;
  typedef signed   long long INT64;
  typedef unsigned char      UINT8;
  typedef unsigned short int UINT16;
  typedef unsigned int       UINT32;
  typedef unsigned long long UINT64;
 
  typedef unsigned char      u8;
  typedef unsigned short int u16;
  typedef unsigned int       u32;
  typedef unsigned long long u64;
#endif

#if OS_TYPE == VXWORKS_RTP 
  typedef signed   char      INT8;
  typedef signed   short int INT16;
  typedef signed   int       INT32;
  typedef signed   long long INT64;
  typedef unsigned char      UINT8;
  typedef unsigned short int UINT16;
  typedef unsigned int       UINT32;
  typedef unsigned long long UINT64;
 
  typedef unsigned char      u8;
  typedef unsigned short int u16;
  typedef unsigned int       u32;
  typedef unsigned long long u64;
#endif



//==============================================================================
//$$ Basic Memory Access Macro
//==============================================================================
#if (OS_TYPE == XILINX) || (OS_TYPE == VXWORKS_RTP) || (OS_TYPE == VXWORKS)
#define READ8(port)   (*((volatile unsigned char*)(port)))
#define READ16(port)  (*((volatile unsigned short int*)(port)))
#define READ32(port)  (*((volatile unsigned int*)(port)))

#define WRITE8( port, val )   (*((volatile unsigned char*)(port))=((unsigned char)(val)))
#define WRITE16( port, val )  (*((volatile unsigned short int*)(port))=((unsigned short int)(val)))
#define WRITE32( port, val )  (*((volatile unsigned int*)(port))=((unsigned int)(val)))

#define SWAP_READ16(port)  E_SWAP_16((*((volatile unsigned short *)(port))))
#define SWAP_READ32(port)  E_SWAP_32((*((volatile unsigned int *)(port))))
#define SWAP_WRITE16( port, val )  (*((volatile unsigned short *)(port))=((unsigned short)(E_SWAP_16(val))))
#define SWAP_WRITE32( port, val )  (*((volatile unsigned int *)(port))=((unsigned int)(E_SWAP_32(val))))
#endif

#if 0 //2014-11-07 pm 4:18:52 by lay_kim
  // hslee, optimize the swap operation, this macro uses port reading 4 times
  #define E_SWAP_16(A)    ((((UINT16)(A)&0xff00)>>8)|(((UINT16)(A)&0x00ff)<<8))
  #define E_SWAP_32(A)    ((((UINT32)(A)&0xff000000)>>24)|\
                           (((UINT32)(A)&0x00ff0000)>>8)|\
                           (((UINT32)(A)&0x0000ff00)<<8)|\
                           (((UINT32)(A)&0x000000ff)<<24))
#endif



//==============================================================================
//$$ BITx/size/etc define
//==============================================================================
enum 
{
    BIT0  = 0x00000001,
    BIT1  = 0x00000002,
    BIT2  = 0x00000004,
    BIT3  = 0x00000008,
    BIT4  = 0x00000010,
    BIT5  = 0x00000020,
    BIT6  = 0x00000040,
    BIT7  = 0x00000080,
    BIT8  = 0x00000100,
    BIT9  = 0x00000200,
    BIT10 = 0x00000400,
    BIT11 = 0x00000800,
    BIT12 = 0x00001000,
    BIT13 = 0x00002000,
    BIT14 = 0x00004000,
    BIT15 = 0x00008000,
    BIT16 = 0x00010000,
    BIT17 = 0x00020000,
    BIT18 = 0x00040000,
    BIT19 = 0x00080000,
    BIT20 = 0x00100000,
    BIT21 = 0x00200000,
    BIT22 = 0x00400000,
    BIT23 = 0x00800000,
    BIT24 = 0x01000000,
    BIT25 = 0x02000000,
    BIT26 = 0x04000000,
    BIT27 = 0x08000000,
    BIT28 = 0x10000000,
    BIT29 = 0x20000000,
    BIT30 = 0x40000000,
    BIT31 = 0x80000000
};




#ifndef OK
  #define OK 1
#endif

#ifndef ERROR
  #define ERROR -1
#endif

#define RET_OK  1
#define RET_ERR -1

#define _1_KILO_ (0x00000400)
#define _1_MEGA_ (0x00100000)
#define _1_GIGA_ (0x40000000)

#define _1KB_    (0x00000400)
#define _2KB_    (0x00000800)
#define _4KB_    (0x00001000)
#define _8KB_    (0x00002000)
#define _16KB_   (0x00004000)
#define _32KB_   (0x00008000)
#define _64KB_   (0x00010000)
#define _128KB_  (0x00020000)
#define _256KB_  (0x00040000)
#define _512KB_  (0x00080000)
#define _1MB_    (0x00100000)
#define _2MB_    (0x00200000)
#define _4MB_    (0x00400000)
#define _8MB_    (0x00800000)
#define _16MB_   (0x01000000)
#define _32MB_   (0x02000000)
#define _64MB_   (0x04000000)
#define _128MB_  (0x08000000)
#define _256MB_  (0x10000000)
#define _512MB_  (0x20000000)
#define _1GB_    (0x40000000)
#define _2GB_    (0x80000000)

#define _CTYPE_TX_ "Tx"
#define _CTYPE_RX_ "Rx"

#define _NULL_         (-1)
#define _UNLINKED_     (0)
#define _LINKED_       (1)

#define slv_reg00 0x000
#define slv_reg01 0x004
#define slv_reg02 0x008
#define slv_reg03 0x00C
#define slv_reg04 0x010
#define slv_reg05 0x014
#define slv_reg06 0x018
#define slv_reg07 0x01C
#define slv_reg08 0x020
#define slv_reg09 0x024
#define slv_reg0a 0x028
#define slv_reg0b 0x02C
#define slv_reg0c 0x030
#define slv_reg0d 0x034
#define slv_reg0e 0x038
#define slv_reg0f 0x03C
#define slv_reg10 0x040
#define slv_reg11 0x044
#define slv_reg12 0x048
#define slv_reg13 0x04C
#define slv_reg14 0x050
#define slv_reg15 0x054
#define slv_reg16 0x058
#define slv_reg17 0x05C
#define slv_reg18 0x060
#define slv_reg19 0x064
#define slv_reg1a 0x068
#define slv_reg1b 0x06C
#define slv_reg1c 0x070
#define slv_reg1d 0x074
#define slv_reg1e 0x078
#define slv_reg1f 0x07C
#define slv_reg20 0x080
#define slv_reg21 0x084
#define slv_reg22 0x088
#define slv_reg23 0x08C
#define slv_reg24 0x090
#define slv_reg25 0x094
#define slv_reg26 0x098
#define slv_reg27 0x09C
#define slv_reg28 0x0A0
#define slv_reg29 0x0A4
#define slv_reg2a 0x0A8
#define slv_reg2b 0x0AC
#define slv_reg2c 0x0B0
#define slv_reg2d 0x0B4
#define slv_reg2e 0x0B8
#define slv_reg2f 0x0BC
#define slv_reg30 0x0C0
#define slv_reg31 0x0C4
#define slv_reg32 0x0C8
#define slv_reg33 0x0CC
#define slv_reg34 0x0D0
#define slv_reg35 0x0D4
#define slv_reg36 0x0D8
#define slv_reg37 0x0DC
#define slv_reg38 0x0E0
#define slv_reg39 0x0E4
#define slv_reg3a 0x0E8
#define slv_reg3b 0x0EC
#define slv_reg3c 0x0F0
#define slv_reg3d 0x0F4
#define slv_reg3e 0x0F8
#define slv_reg3f 0x0FC
#define slv_reg40 0x100
#define slv_reg41 0x104
#define slv_reg42 0x108
#define slv_reg43 0x10C
#define slv_reg44 0x110
#define slv_reg45 0x114
#define slv_reg46 0x118
#define slv_reg47 0x11C
#define slv_reg48 0x120
#define slv_reg49 0x124
#define slv_reg4a 0x128
#define slv_reg4b 0x12C
#define slv_reg4c 0x130
#define slv_reg4d 0x134
#define slv_reg4e 0x138
#define slv_reg4f 0x13C
#define slv_reg50 0x140
#define slv_reg51 0x144
#define slv_reg52 0x148
#define slv_reg53 0x14C
#define slv_reg54 0x150
#define slv_reg55 0x154
#define slv_reg56 0x158
#define slv_reg57 0x15C
#define slv_reg58 0x160
#define slv_reg59 0x164
#define slv_reg5a 0x168
#define slv_reg5b 0x16C
#define slv_reg5c 0x170
#define slv_reg5d 0x174
#define slv_reg5e 0x178
#define slv_reg5f 0x17C

//==============================================================================
//----===@ define user register map Address
//==============================================================================
#define A_vendor                  slv_reg00
#define A_ipInfo                  slv_reg01
#define A_axiClkFreq              slv_reg02
#define A_freeRunCntr             slv_reg03




//==============================================================================
//$$ printf Control Macro /VXWORKS
//==============================================================================
#if OS_TYPE == VXWORKS
  #include "logLib.h"
 
  #define prnRLS(s,p1,p2)  logMsg(s,p1,p2,0,0,0,0)
 
  #ifdef __DEBUG_0__   // Enable all
    #define prnM0(s,p1,p2,p3,p4,p5,p6)  logMsg(s,p1,p2,p3,p4,p5,p6)
    #define prnM1(s,p1,p2,p3,p4,p5,p6)  logMsg(s,p1,p2,p3,p4,p5,p6)
    #define prnErr()                      logMsg("[ERR][Func:%s] [%s_%d]\n",(int)__FUNCTION__ , (int)__FILE__, __LINE__,0,0,0)
    #define prnMeta(args)         if(args)prnErr()
    #define prnMeta_RET(args);    if(args){prnErr();return RET_ERR;}
  #endif

  #ifdef __DEBUG_1__   // Enable only logmsg level 1, prnErr, prnMeta, PRINTFR
    #define prnM0(s,p1,p2,p3,p4,p5,p6)  
    #define prnM1(s,p1,p2,p3,p4,p5,p6)  logMsg(s,p1,p2,p3,p4,p5,p6)
    #define prnErr()                      logMsg("[ERR][Func:%s] [%s_%d]\n",(int)__FUNCTION__ , (int)__FILE__, __LINE__,0,0,0)
    #define prnMeta(args)         if(args)prnErr()
    #define prnMeta_RET(args);    if(args){prnErr();return RET_ERR;}
  #endif

  #ifdef __DEBUG_2__   // Enable prnErr, prnMeta, PRINTFR
    #define prnM0(s,p1,p2,p3,p4,p5,p6)  
    #define prnM1(s,p1,p2,p3,p4,p5,p6)  
    #define prnErr()                      logMsg("[ERR][Func:%s] [%s_%d]\n",(int)__FUNCTION__ , (int)__FILE__, __LINE__,0,0,0)
    #define prnMeta(args)         if(args)prnErr()
    #define prnMeta_RET(args);    if(args){prnErr();return RET_ERR;}
  #endif 

  #ifdef __RELEASE__ // Enable only PRINTFR.
    #define prnM0(s,p1,p2,p3,p4,p5,p6)  
    #define prnM1(s,p1,p2,p3,p4,p5,p6)  
    #define prnErr()
    #define prnMeta(args)         if(args)prnErr()
    #define prnMeta_RET(args);    if(args){prnErr();return RET_ERR;}
  #endif
#endif

//==============================================================================
//printf Control Macro /LINUX
//==============================================================================
#if (OS_TYPE == LINUX) || (OS_TYPE == VXWORKS_RTP)
  
  #define taskDelay(sv)  usleep(sv)

 
  #define prnRLS(fmt,args...)   printf(fmt,##args)        
 
  #ifdef __DEBUG_0__   // Enable all
    #define prnM0(fmt,args...)  printf(fmt,##args)        
    #define prnM1(fmt,args...)  printf(fmt,##args)       
    #define prnM2(fmt,args...)  printf(fmt,##args)       
    #define prnErr()            printf("[ERR][Func:%s] [%s_%d]\n",__FUNCTION__ , __FILE__, __LINE__)
   #endif

  #ifdef __DEBUG_1__   // Enable only printf level 1, prnErr, prnMeta
    #define prnM0(fmt,args...)  
    #define prnM1(fmt,args...)  printf(fmt,##args)       
    #define prnM2(fmt,args...)  printf(fmt,##args)       
    #define prnErr()            printf("[ERR]%-22s [%s_%d]\r\n",__FUNCTION__ , __FILE__, __LINE__)
   #endif

  #ifdef __DEBUG_2__   // Enable prnErr, prnMeta
    #define prnM0(fmt,args...)  
    #define prnM1(fmt,args...)  
    #define prnM2(fmt,args...)  printf(fmt,##args)       
    #define prnErr()            printf("[ERR][Func:%s] [%s_%d]\n",__FUNCTION__ , __FILE__, __LINE__)
  #endif

  #ifdef __DEBUG_3__   // Enable prnErr, prnMeta
    #define prnM0(fmt,args...)  
    #define prnM1(fmt,args...)  
    #define prnM2(fmt,args...)  
    #define prnErr()            printf("[ERR][Func:%s] [%s_%d]\n",__FUNCTION__ , __FILE__, __LINE__)
  #endif

  #ifdef __RELEASE__ // Enable only PRINTFR.
    #define prnM0(fmt,args...)
    #define prnM1(fmt,args...)
    #define prnM2(fmt,args...)  
    #define prnErr()  				  
  #endif

  #define prnM0_fmtHex(s1, v1, s2)  prnM0("%-22s: 0x%08x, %s\r\n" , s1, v1, s2)
  #define prnM0_fmtDec(s1, v1, s2)  prnM0("%-22s: %10d, %s\r\n"   , s1, v1, s2)
  #define prnM1_fmtHex(s1, v1, s2)  prnM1("%-22s: 0x%08x, %s\r\n" , s1, v1, s2)
  #define prnM1_fmtDec(s1, v1, s2)  prnM1("%-22s: %10d, %s\r\n"   , s1, v1, s2)
  #define prnM2_fmtHex(s1, v1, s2)  prnM2("%-22s: 0x%08x, %s\r\n" , s1, v1, s2)
  #define prnM2_fmtDec(s1, v1, s2)  prnM2("%-22s: %10d, %s\r\n"   , s1, v1, s2)

  #define prnErrRet()   {prnErr();return RET_ERR;}
  #define prnMeta(args)  if(args)prnErr()
  #define ifRet(args)    if(args){prnErr();return RET_ERR;}
#endif



//==============================================================================
//$$ Message Logging Control Macro
//==============================================================================
//#define __FUNC_LOG_ON__
#ifdef __FUNC_LOG_ON__
  #define DR_SAVE_LOG(s,p1) sprintf(s,"%s %d %d",__FUNCTION__,__LINE__,p1)
#else
  #define DR_SAVE_LOG(s,p1)
#endif



#define __ERR_LOG_LEVEL0__
//#define __ERR_LOG_LEVEL1__
//#define __ERR_LOG_LEVEL2__

#ifdef __ERR_LOG_LEVEL0__
  #define DR_ERR_LOG(s,p1,p2)
#endif

#ifdef __ERR_LOG_LEVEL1__
  #define DR_ERR_LOG(s,p1,p2)  { s.ErrLog1[p2]++; s.ErrCnt++; }
#endif

#ifdef __ERR_LOG_LEVEL2__
  #define DR_ERR_LOG(s,p1,p2)  { s.ErrLog2[s.ErrCnt].RunCntNum = p1;          \
                                 s.ErrLog2[s.ErrCnt].ErrRtn    = p2;          \
                                 s.ErrCnt++; }
#endif


typedef struct
{
  UINT32 s_time;
  UINT32 e_time;
  UINT32 d_time;
  UINT32 DTP_i;
  UINT32 DTP_f;
  UINT32 DTP_i_Tenth;
  double DTP;
}drTIME_CHECK_T;

#define MAXNUM_ERRLOGCNT (100)
#define MAXNUM_DTP       (1000)

typedef struct
{
  UINT32 RunCntNum;
  int    ErrRtn;  
}drCOMERR_LOG1_T;

typedef struct
{
  UINT32 RunCntNum;
  UINT32 DataSize;
  UINT32 DTP;   
}drCOMDTP_LOG_T;

typedef struct
{
  UINT32          RunCnt;                   
  UINT32          OKCnt;                  
  UINT32          ErrCnt;                  
  int             ErrLog1[MAXNUM_ERRLOGCNT];
  drCOMERR_LOG1_T ErrLog2[MAXNUM_ERRLOGCNT];  
  UINT32          DTP[MAXNUM_DTP];          // 10MB/s
  drCOMDTP_LOG_T* pDTPLog;
}drCOMLOG_T;


//==============================================================================
//$$ Performace Measure Macro
//==============================================================================
#define TIMEING_CHK( eventNum, val ) { int evtBuf; evtBuf = (int)val; wvEvent(eventNum, (char*)&evtBuf, sizeof(evtBuf));}






#ifdef __cplusplus
}
#endif
#endif
/*==============================================================================
                                  END OF FILE
==============================================================================*/
