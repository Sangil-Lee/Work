/*
 * SNMPGET
 * Tuesday, December  9 15:06:40 KST 2014
 * Mi Jeong Park, mijoy0909@ibs.re.kr
 */

#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <string.h>

#define DEMO_USE_SNMP_VERSION_3


#ifdef DEMO_USE_SNMP_VERSION_3
const char *our_v3_passphrase = "qwertyuiop12345";
#endif


void snmpget(netsnmp_session *session);
void snmpset(netsnmp_session *session);
int getVariables(netsnmp_session *session, netsnmp_pdu *pdu,netsnmp_pdu **response);
void printVariables(netsnmp_pdu *response);


int main(int argc, char ** argv)
{
  netsnmp_session session, *ss;

  /*
   * Initialize the SNMP library
   */
  init_snmp("snmpapp");

  /*
   * Initialize a "session" that defines who we're going to talk to
   */
  snmp_sess_init( &session );                   /* set up defaults */
  session.peername = strdup("10.1.5.142");

  /* set up the authentication parameters for talking to the server */

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

  /*
   * Open the session
   */
  SOCK_STARTUP;
  ss = snmp_open(&session);                     /* establish the session */

  if (!ss) {
    snmp_sess_perror("snmpappget", &session);
    SOCK_CLEANUP;
    exit(1);
  }
    
  snmpget(ss);
  snmpset(ss);

  snmp_close(ss);

  SOCK_CLEANUP;
  return (0);
} /* main() */


void snmpget(netsnmp_session *session)
{
  netsnmp_pdu *pdu;
  netsnmp_pdu *response;

  oid anOID[MAX_OID_LEN];
  size_t anOID_len;

  netsnmp_variable_list *vars;
  int status;
  int count=1;

  /*
   * Create the PDU for the data for our request.
   *   1) We're going to GET the system.sysDescr.0 node.
   */

  pdu = snmp_pdu_create(SNMP_MSG_GET);
  anOID_len = MAX_OID_LEN;
  if (!snmp_parse_oid("sPDUOutletCtl.8", anOID, &anOID_len)) {
    snmp_perror("read_objid failed\n");  
    SOCK_CLEANUP;
    exit(1);
  }

  snmp_add_null_var(pdu, anOID, anOID_len);
  
  /*
   * Send the Request out.
   */
  
  status=getVariables(session,pdu,&response);

  /*
   * Process the response.
   */
  if (status == STAT_SUCCESS && response->errstat == SNMP_ERR_NOERROR) {
    /*
     * SUCCESS: Print the result variables
     */
    printVariables(response);

  } else {
    /*
     * FAILURE: print what went wrong!
     */

    if (status == STAT_SUCCESS)
      fprintf(stderr, "Error in packet\nReason: %s\n",
	      snmp_errstring(response->errstat));
    else if (status == STAT_TIMEOUT)
      fprintf(stderr, "Timeout: No response.\n");
    else
          fprintf(stderr,"error occrued!\n");
  }

  /*
   * Clean up:
   *  1) free the response.
   *  2) close the session.
   */
  if (response)
    snmp_free_pdu(response);
}




void snmpset(netsnmp_session *session)
{
  netsnmp_pdu *pdu;
  netsnmp_pdu *response;

  oid anOID[MAX_OID_LEN];
  size_t anOID_len;

  netsnmp_variable_list *vars;
  int status;
  int count=1;
  char type='i';

  /*
   * Create set request and add object names and values.
   */

  pdu = snmp_pdu_create(SNMP_MSG_SET);
  anOID_len = MAX_OID_LEN;

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
  status=getVariables(session,pdu,&response);

  /*
   * Process the response.
   */
  if (status == STAT_SUCCESS && response->errstat == SNMP_ERR_NOERROR) {
    /*
     * SUCCESS: Print the result variables
     */
    printVariables(response);


  } else {
    /*
     * FAILURE: print what went wrong!
     */

    if (status == STAT_SUCCESS)
      fprintf(stderr, "Error in packet\nReason: %s\n",
	      snmp_errstring(response->errstat));
    else if (status == STAT_TIMEOUT)
      fprintf(stderr, "Timeout: No response\n");
    else
          fprintf(stderr,"error occrued!\n");
  }

  /*
   * Clean up:
   *  1) free the response.
   *  2) close the session.
   */
  if (response)
    snmp_free_pdu(response);

}

int getVariables(netsnmp_session *session, netsnmp_pdu *pdu, netsnmp_pdu **response){
  //send request.
  int status;
  return status = snmp_synch_response(session, pdu, response);
}

void printVariables(netsnmp_pdu *response){
  netsnmp_variable_list *vars;
  for(vars = response->variables; vars; vars = vars->next_variable){
    print_variable(vars->name, vars->name_length, vars);
    printf("\n");
  }
}
