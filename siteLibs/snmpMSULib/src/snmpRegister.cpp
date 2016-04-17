/*snmpRegister.cpp */

#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <epicsStdioRedirect.h>

#include "devSnmp.h"
#include "iocsh.h"

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

#ifdef __cplusplus
extern "C" {
#endif	/* __cplusplus */

/*--------------------------------------------------------------------*/
static void iocsh_epicsSnmpInit(const iocshArgBuf * args)
{
  epicsSnmpInit(args[0].ival);
};

static const iocshArg iocsh_epicsSnmpInit_Arg0 = {"value", iocshArgInt};
static const iocshArg *const iocsh_epicsSnmpInit_Args[1] =
{ &iocsh_epicsSnmpInit_Arg0 };
static const iocshFuncDef iocsh_epicsSnmpInit_FuncDef = {"epicsSnmpInit", 1, iocsh_epicsSnmpInit_Args};

/*--------------------------------------------------------------------*/

#if	0
static const iocshFuncDef epicsSnmpInitDef =
    {"epicsSnmpInit", 0, 0};
static void epicsSnmpInitCall(const iocshArgBuf * args) {
	epicsSnmpInit();
}
#endif

/*--------------------------------------------------------------------*/
static void iocsh_devSnmpSetSnmpVersion(const iocshArgBuf *args)
{
  char *host = args[0].sval;
  char *vers = args[1].sval;

  devSnmpSetSnmpVersion(host,vers);
}

static const iocshArg iocsh_devSnmpSetSnmpVersion_Arg0 = { "host", iocshArgString };
static const iocshArg iocsh_devSnmpSetSnmpVersion_Arg1 = { "version", iocshArgString };
static const iocshArg * const iocsh_devSnmpSetSnmpVersion_Args[2] =
  {&iocsh_devSnmpSetSnmpVersion_Arg0,
   &iocsh_devSnmpSetSnmpVersion_Arg1};
static const iocshFuncDef iocsh_devSnmpSetSnmpVersion_FuncDef = {"devSnmpSetSnmpVersion",2,iocsh_devSnmpSetSnmpVersion_Args};
// older function name:
static const iocshFuncDef iocsh_epicsSnmpSetSnmpVersion_FuncDef = {"epicsSnmpSetSnmpVersion",2,iocsh_devSnmpSetSnmpVersion_Args};

/*--------------------------------------------------------------------*/
static void iocsh_devSnmpSetMaxOidsPerReq(const iocshArgBuf *args)
{
  char *host = args[0].sval;
  char max   = args[1].ival;

  devSnmpSetMaxOidsPerReq(host,max);
}

static const iocshArg iocsh_devSnmpSetMaxOidsPerReq_Arg0 = { "host", iocshArgString };
static const iocshArg iocsh_devSnmpSetMaxOidsPerReq_Arg1 = { "maxoids", iocshArgInt };
static const iocshArg * const iocsh_devSnmpSetMaxOidsPerReq_Args[2] =
  {&iocsh_devSnmpSetMaxOidsPerReq_Arg0,
   &iocsh_devSnmpSetMaxOidsPerReq_Arg1};
static const iocshFuncDef iocsh_devSnmpSetMaxOidsPerReq_FuncDef = {"devSnmpSetMaxOidsPerReq",2,iocsh_devSnmpSetMaxOidsPerReq_Args};
// older function name:
static const iocshFuncDef iocsh_epicsSnmpSetMaxOidsPerReq_FuncDef = {"epicsSnmpSetMaxOidsPerReq",2,iocsh_devSnmpSetMaxOidsPerReq_Args};

/*--------------------------------------------------------------------*/
static void iocsh_devSnmpSetDebug(const iocshArgBuf *args)
{
  int level = args[0].ival;
  devSnmpSetDebug(level);
}

static const iocshArg iocsh_devSnmpSetDebug_Arg0 = { "state", iocshArgInt };
static const iocshArg * const iocsh_devSnmpSetDebug_Args[1] =
  {&iocsh_devSnmpSetDebug_Arg0};
static const iocshFuncDef iocsh_devSnmpSetDebug_FuncDef = {"devSnmpSetDebug",1,iocsh_devSnmpSetDebug_Args};

/*--------------------------------------------------------------------*/
static void iocsh_devSnmpSetParam(const iocshArgBuf *args)
{
  char *param = args[0].sval;
  int value   = args[1].ival;

  devSnmpSetParam(param,value);
}

static const iocshArg iocsh_devSnmpSetParam_Arg0 = { "param", iocshArgString };
static const iocshArg iocsh_devSnmpSetParam_Arg1 = { "value", iocshArgInt };
static const iocshArg * const iocsh_devSnmpSetParam_Args[2] =
{&iocsh_devSnmpSetParam_Arg0, &iocsh_devSnmpSetParam_Arg1};
static const iocshFuncDef iocsh_devSnmpSetParam_FuncDef = {"devSnmpSetParam",2,iocsh_devSnmpSetParam_Args};

/*--------------------------------------------------------------------*/
static void iocsh_snmpr(const iocshArgBuf *args)
{
  int level = args[0].ival;
  char *match = args[1].sval;

  snmpr(level,match);
}

static const iocshArg iocsh_snmpr_Arg0 = { "level", iocshArgInt };
static const iocshArg iocsh_snmpr_Arg1 = { "match", iocshArgString };
static const iocshArg * const iocsh_snmpr_Args[2] =
  {&iocsh_snmpr_Arg0,
   &iocsh_snmpr_Arg1};
static const iocshFuncDef iocsh_snmpr_FuncDef = {"snmpr",2,iocsh_snmpr_Args};

/*--------------------------------------------------------------------*/
static void iocsh_snmpz(const iocshArgBuf *args)
{
  snmpz();
}

static const iocshFuncDef iocsh_snmpz_FuncDef = {"snmpz",0,NULL};

/*--------------------------------------------------------------------*/
static void iocsh_snmpzr(const iocshArgBuf *args)
{
  int level = args[0].ival;
  char *match = args[1].sval;

  snmpzr(level,match);
}

static const iocshArg iocsh_snmpzr_Arg0 = { "level", iocshArgInt };
static const iocshArg iocsh_snmpzr_Arg1 = { "match", iocshArgString };
static const iocshArg * const iocsh_snmpzr_Args[2] =
  {&iocsh_snmpzr_Arg0,
   &iocsh_snmpzr_Arg1};
static const iocshFuncDef iocsh_snmpzr_FuncDef = {"snmpzr",2,iocsh_snmpzr_Args};

/*--------------------------------------------------------------------*/
/*--------------------------------------------------------------------*/
void snmp_Register()
{
  static int firstTime = true;

  if (! firstTime) return;

  firstTime = false;
  iocshRegister(&iocsh_devSnmpSetMaxOidsPerReq_FuncDef,   iocsh_devSnmpSetMaxOidsPerReq);
  iocshRegister(&iocsh_devSnmpSetSnmpVersion_FuncDef,     iocsh_devSnmpSetSnmpVersion);
  iocshRegister(&iocsh_devSnmpSetParam_FuncDef,           iocsh_devSnmpSetParam);
  iocshRegister(&iocsh_snmpr_FuncDef,                     iocsh_snmpr);
  iocshRegister(&iocsh_snmpz_FuncDef,                     iocsh_snmpz);
  iocshRegister(&iocsh_snmpzr_FuncDef,                    iocsh_snmpzr);
  // old function names, still supported for now (mapped to new names)
  iocshRegister(&iocsh_epicsSnmpSetMaxOidsPerReq_FuncDef, iocsh_devSnmpSetMaxOidsPerReq);
  iocshRegister(&iocsh_epicsSnmpSetSnmpVersion_FuncDef,   iocsh_devSnmpSetSnmpVersion);
  // deprecated functions
  iocshRegister(&iocsh_devSnmpSetDebug_FuncDef,           iocsh_devSnmpSetDebug);
  iocshRegister(&iocsh_epicsSnmpInit_FuncDef,             iocsh_epicsSnmpInit);
}
/*--------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif	/* __cplusplus */
class snmp_CommonInit {
    public:
    snmp_CommonInit() {
	snmp_Register();
    }
};
static snmp_CommonInit snmp_CommonInitObj;
