/*
 * devSnmpSoft.c (snmpget only)
 * v0.1 Monday, December 22 14:41:34 KST 2014
 * Mi Jeong Park, mijoy0909@ibs.re.kr
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "alarm.h"
#include "cvtTable.h"
#include "dbDefs.h"
#include "dbAccess.h"
#include "recGbl.h"
#include "recSup.h"
#include "devSup.h"
#include "link.h"
#include "snmpRecord.h"
#include "epicsExport.h"

#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>

#define DEMO_USE_SNMP_VERSION_3
#ifdef DEMO_USE_SNMP_VERSION_3
const char *our_v3_passphrase = "qwertyuiop12345";
const char *priv_passphrase = "qwertyuiop12345"; 
#endif

int snmp_get();
static long snmp_init_record();
static long read_snmp();

struct {
  long number;
  DEVSUPFUN report;
  DEVSUPFUN init;
  DEVSUPFUN init_record;
  DEVSUPFUN get_ioint_info;
  DEVSUPFUN read_snmp;
}devSNMPSoft={
  5,
  NULL,
  NULL,
  snmp_init_record,
  NULL,
  read_snmp,
};
epicsExportAddress(dset,devSNMPSoft);

static long snmp_init_record(struct snmpRecord *psnmp)
{
  if(recGblInitConstantLink(&psnmp->inp,DBF_DOUBLE,&psnmp->val))
    psnmp->udf = FALSE;
  return(0);
}

static long read_snmp(struct snmpRecord *psnmp)
{
  long status;

  snmp_get(psnmp);

  status = dbGetLink(&(psnmp->inp),DBF_DOUBLE, &(psnmp->val),0,0);
  /*If return was succesful then set undefined false*/
  if(!status) psnmp->udf = FALSE;
  return(0);
}

int snmp_get(struct snmpRecord *psnmp)
{
  netsnmp_session session, *ss;
  netsnmp_pdu *pdu;
  netsnmp_pdu *response;
  oid anOID[MAX_OID_LEN];
  size_t anOID_len= MAX_OID_LEN;

  netsnmp_variable_list *vars;
  int status;
  int a;
  int val;

  init_snmp("snmpMon");                                                    /* Initialize the SNMP library */
  snmp_sess_init( &session );                                              /* set up defaults */  
  session.peername = strdup("10.1.5.142");
  session.version=SNMP_VERSION_3;                                          /* set the SNMP version number */
  session.securityName = strdup("mijoy");                                  /* set the SNMPv3 user name */
  session.securityNameLen = strlen(session.securityName);
  session.securityLevel = SNMP_SEC_LEVEL_AUTHPRIV;                         /* set the security level to authenticated, but not encrypted */


  /* Authentication Protocol */
  /* MD5 */
  session.securityAuthProto = usmHMACMD5AuthProtocol;
  session.securityAuthProtoLen = sizeof(usmHMACMD5AuthProtocol)/sizeof(oid);
  session.securityAuthKeyLen = USM_AUTH_KU_LEN;

  /* Authentication keys generation */
  if (generate_Ku(session.securityAuthProto,
		  session.securityAuthProtoLen,
		  (u_char *) our_v3_passphrase, strlen(our_v3_passphrase),
		  session.securityAuthKey,
		  &session.securityAuthKeyLen) != SNMPERR_SUCCESS) {
    snmp_perror("Error!!!!!!!!!!!!");
    snmp_log(LOG_ERR,
	     "Error generating Ku from authentication pass phrase. \n");
    exit(1);
  }

  /* Privacy protocol */
  /* DES */
  session.securityPrivProto = usmDESPrivProtocol;
  session.securityPrivProtoLen = USM_PRIV_PROTO_DES_LEN;
  session.securityPrivKeyLen = USM_PRIV_KU_LEN;

  /* Privacy keys generation */
  if (generate_Ku(session.securityAuthProto,
		  session.securityAuthProtoLen,
		  (u_char *) priv_passphrase, strlen(priv_passphrase),
		  session.securityPrivKey,
		  &session.securityPrivKeyLen) != SNMPERR_SUCCESS) {
    printf ("Error generating Ku from privacity pass phrase. \n");
    exit(1);
  }

  /* VERSION 2c 
   * session.version = SNMP_VERSION_2c; 
   * session.community = "public"; 
   * session.community_len = strlen(session.community); */

  SOCK_STARTUP;                                                            /* Open the session */
  ss = snmp_open(&session);                                                /* establish the session */

  if (!ss) {
    snmp_sess_perror("Error!!!!!!!", &session);                            /* print a error stored in a session pointer */
    SOCK_CLEANUP;
    exit(1);
  }

  pdu = snmp_pdu_create(SNMP_MSG_GET);                                     /* Create the PDU for the data for our request */

  if (!snmp_parse_oid("sPDUOutletCtl.8", anOID, &anOID_len)) {
    snmp_perror("read_objid failed\n");  
    SOCK_CLEANUP;
    exit(1);
  }

  snmp_add_null_var(pdu, anOID, anOID_len);
  status = snmp_synch_response(ss, pdu, &response);                        /* Send the Request out */

  for(vars = response->variables; vars; vars = vars->next_variable)        /* Process the response */
    {
      char *sp = (char *)malloc(1 + vars->val_len);
      memcpy(sp, vars->val.string, vars->val_len);                         /* string */
      sp[vars->val_len] = '\0';           
      memcpy((int *)&a, sp, 4);                                            /* integer */ 
    }

  psnmp->val = a;
  snmp_free_pdu(response);                                                 /* free the response */
  snmp_close(ss);                                                          /* close the session */
        
  return (0);
}


