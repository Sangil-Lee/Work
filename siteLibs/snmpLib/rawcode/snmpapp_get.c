/*
 * SNMPGET
 * Tuesday, December  9 15:06:40 KST 2014
 * Mi Jeong Park, mijoy0909@ibs.re.kr
 */

#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <string.h>

/* change the word "define" to "undef" to try the (insecure) SNMPv1 version */
#define DEMO_USE_SNMP_VERSION_3

/* #ifdef DEMO_USE_SNMP_VERSION_3 */
/* const char *our_v3_passphrase = "qwertyuiop12345"; */
/* const char *our_v3_privphrase = "qwertyuiop12345"; */
/* #endif */

#ifdef DEMO_USE_SNMP_VERSION_3
const char *our_v3_passphrase = "MySecret";
const char *our_v3_privphrase = "MySecret";
#endif



int main(int argc, char ** argv)
{
  netsnmp_session session, *ss;
  netsnmp_pdu *pdu;
  netsnmp_pdu *response;

  oid anOID[MAX_OID_LEN];
  size_t anOID_len;

  netsnmp_variable_list *vars;
  int status;
  int count=1;
  int a;

  /*
   * Initialize the SNMP library
   */
  init_snmp("snmpapp_get");

  /*
   * Initialize a "session" that defines who we're going to talk to
   */
  snmp_sess_init( &session );                   /* set up defaults */
  session.peername = strdup("10.1.5.123");

  /* set up the authentication parameters for talking to the server */
  /*-------------------------------------------------------------------------------------------- */
#ifdef DEMO_USE_SNMP_VERSION_3
  /* Use SNMPv3 to talk to the experimental server */

  /* set the SNMP version number */
  session.version=SNMP_VERSION_3;
        
  /* set the SNMPv3 user name */
  session.securityName = strdup("admin");
  session.securityNameLen = strlen(session.securityName);

  /* set the security level to authenticated, but not encrypted */
  session.securityLevel = SNMP_SEC_LEVEL_AUTHPRIV;

  /* set the authentication method to MD5 */
  session.securityAuthProto = usmHMACMD5AuthProtocol;
  session.securityAuthProtoLen = sizeof(usmHMACMD5AuthProtocol)/sizeof(oid);
  session.securityAuthKeyLen = USM_AUTH_KU_LEN;

  /* set the authentication key to a MD5 hashed version of our
     passphrase "The Net-SNMP Demo Password" (which must be at least 8
     characters long) */
  if (generate_Ku(session.securityAuthProto,
		  session.securityAuthProtoLen,
		  (u_char *) our_v3_passphrase, strlen(our_v3_passphrase),
		  session.securityAuthKey,
		  &session.securityAuthKeyLen) != SNMPERR_SUCCESS) {
    snmp_perror(argv[0]);
    snmp_log(LOG_ERR,
	     "Error generating Ku from authentication pass phrase. \n");
    exit(1);
  }

    session.securityPrivProto = usmDESPrivProtocol;
    session.securityPrivProtoLen = USM_PRIV_PROTO_DES_LEN;
    session.securityPrivKeyLen = USM_PRIV_KU_LEN;

  if (generate_Ku(session.securityAuthProto,
		  session.securityAuthProtoLen,
		  (u_char *) our_v3_privphrase, strlen(our_v3_privphrase),
		  session.securityPrivKey,
		  &session.securityPrivKeyLen) != SNMPERR_SUCCESS) {
    snmp_perror(argv[0]);
    snmp_log(LOG_ERR,
	     "Error generating Ku from authentication pass phrase. \n");
    exit(1);
  }
 
#endif
  /* /\*-------------------------------------------------------------------------------------------- *\/ */
  /*   /\* set the SNMP version number *\/ */
  /*   session.version = SNMP_VERSION_2c; */
  /*   /\* set the SNMPv1 community name used for authentication *\/ */
  /*   session.community = "public"; */
  /*   session.community_len = strlen(session.community); */
  /* /\*-------------------------------------------------------------------------------------------- *\/ */


  /*
   * Open the session
   */
  SOCK_STARTUP;
  ss = snmp_open(&session);                     /* establish the session */

  if (!ss) {
    snmp_sess_perror("ack", &session);
    SOCK_CLEANUP;
    exit(1);
  }
    
  /*
   * Create the PDU for the data for our request.
   *   1) We're going to GET the system.sysDescr.0 node.
   */

  pdu = snmp_pdu_create(SNMP_MSG_GET);
  anOID_len = MAX_OID_LEN;

  /*-------------------------------------
   * char oid[40];
   * printf("Enter OID Name : ");
   * scanf("%s", &oid);
   ---------------------------------------- */
    
  if (!snmp_parse_oid("WIENER-CRATE-MIB::outputSupervisionMinSenseVoltage.u0", anOID, &anOID_len)) {
    snmp_perror("read_objid failed\n");  
    SOCK_CLEANUP;
    exit(1);
  }

  snmp_add_null_var(pdu, anOID, anOID_len);


  /*
   * Send the Request out.
   */
  status = snmp_synch_response(ss, pdu, &response);
  /*
   * Process the response.
   */
  if (status == STAT_SUCCESS && response->errstat == SNMP_ERR_NOERROR) 
{    /*
     * SUCCESS: Print the result variables
     */

    for(vars = response->variables; vars; vars = vars->next_variable)
      {
	printf("%s\n", vars);
	char *sp = NULL;
	sp = (char *)malloc(1+vars->val_len);

	/* char *sp = (char *)malloc(1 + vars->val_len); */
	memcpy(sp, vars->val.string, vars->val_len);
	sp[vars->val_len] = '\0';

	int a;
	memcpy((void *)&a, sp, 4);

	double f;
	memcpy((void *)&f, sp, 4);


	switch(vars->type)
	  {
	  case ASN_OCTET_STR:
	    printf("%s\n", sp);
	    break;
	  case ASN_GAUGE:
	  case ASN_OPAQUE:
	    printf("%lf\n", f);
	    break;
	  case ASN_COUNTER:
	  case ASN_TIMETICKS:
	  case ASN_INTEGER:
	    {
	      printf("%d\n", a);
	      break;
	    }
	  case ASN_IPADDRESS:
	    {
	    }   
	  }
	free(sp);
      }
 } else {
    /*
     * FAILURE: print what went wrong!
     */

    if (status == STAT_SUCCESS)
      fprintf(stderr, "Error in packet\nReason: %s\n",
	      snmp_errstring(response->errstat));
    else if (status == STAT_TIMEOUT)
      fprintf(stderr, "Timeout: No response from %s.\n",
	      session.peername);
    else
      snmp_sess_perror("snmppduapp", ss);
  }


  /*
   * Clean up:
   *  1) free the response.
   *  2) close the session.
   */
  if (response)
    snmp_free_pdu(response);
  snmp_close(ss);

  SOCK_CLEANUP;
  return (0);
} /* main() */
