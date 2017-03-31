/*
 * SNMPSET
 * Tuesday, December  9 15:05:51 KST 2014
 * Mi Jeong Park, mijoy0909@ibs.re.kr
 */

#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <string.h>

/* change the word "define" to "undef" to try the (insecure) SNMPv1 version */
#define DEMO_USE_SNMP_VERSION_3

#ifdef DEMO_USE_SNMP_VERSION_3
const char *our_v3_passphrase = "qwertyuiop12345";
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
  char type='i';

  /*
   * Initialize the SNMP library
   */
  init_snmp("snmpapp_set");

  /*
   * Initialize a "session" that defines who we're going to talk to
   */
  snmp_sess_init( &session );                   /* set up defaults */
  session.peername = strdup("10.1.5.142");

  /* set up the authentication parameters for talking to the server */


  /*-------------------------------------------------------------------------------------------- */
#ifdef DEMO_USE_SNMP_VERSION_3

  /* Use SNMPv3 to talk to the experimental server */

  /* set the SNMP version number */
  session.version=SNMP_VERSION_3;
        
  /* set the SNMPv3 user name */
  session.securityName = strdup("mijoy");
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
#endif
  /*-------------------------------------------------------------------------------------------- */

    
  /* #else /\* we'll use the insecure (but simplier) SNMPv1 *\/ */

  /*     /\* set the SNMP version number *\/ */
  /*     session.version = SNMP_VERSION_1; */

  /*     /\* set the SNMPv1 community name used for authentication *\/ */
  /*     session.community = "admin"; */
  /*     session.community_len = strlen(session.community); */

  /* #endif /\* SNMPv1 *\/ */


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
   * Create set request and add object names and values.
   */

  pdu = snmp_pdu_create(SNMP_MSG_SET);
  anOID_len = MAX_OID_LEN;


  /*-------------------------------------
   * char oid[40];
   * char value[40];
   * printf("Enter OID Name : ");
   * scanf("%s", &oid);
   * printf("Enter Value : ");
   * scanf("%s", &value);
   ---------------------------------------- */
  
  if (!snmp_parse_oid("sPDUOutletCtl.8", anOID, &anOID_len)) {
    snmp_perror("read_objid failed\n");  
    SOCK_CLEANUP;
    exit(1);
  }


  if (snmp_parse_oid("sPDUOutletCtl.8", anOID, &anOID_len) == NULL)
    {
      snmp_perror("read_objid failed\n");  
      SOCK_CLEANUP; 
      exit(1); 
    }
    
  if (snmp_add_var(pdu, anOID, anOID_len, type, "1"))
    {
      snmp_perror("read_objid failed\n");  
    }


  /*
   * Send the Request out.
   */
  status = snmp_synch_response(ss, pdu, &response);

  /*
   * Process the response.
   */
  if (status == STAT_SUCCESS && response->errstat == SNMP_ERR_NOERROR) {
    /*
     * SUCCESS: Print the result variables
     */

    for(vars = response->variables; vars; vars = vars->next_variable)
      print_variable(vars->name, vars->name_length, vars);

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
      snmp_sess_perror("snmpdemoapp", ss);
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
