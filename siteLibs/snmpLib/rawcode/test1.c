#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <string.h>

/* change the word "define" to "undef" to try the (insecure) SNMPv1 version */
#define DEMO_USE_SNMP_VERSION_3

#ifdef DEMO_USE_SNMP_VERSION_3
const char *our_v3_passphrase = "qwertyuiop12345";
#endif

int main(int argc, char **argv)
{
    struct snmp_session session, *ss;
    struct snmp_pdu *pdu;
    struct snmp_pdu *response;

    oid anOID[MAX_OID_LEN];
    size_t anOID_len = MAX_OID_LEN;

    struct variable_list *vars;
    int status;

 /*
   * Initialize the SNMP library
   */
  init_snmp("test");

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

   /*
     * 위의 초기화 정보를 토대로 
     * 세션을 연다 
     */
    SOCK_STARTUP;
    ss = snmp_open(&session);
    if (!ss)
    {
        snmp_perror("ack");
        exit(0);
    }
    /* 요청을 위한 PDU를 만든다. 
     * PDU에 대한 내용은 이미 앞전의 기사에서 
     * 설명했음으로 참고하기 바란다.  
     */
    pdu = snmp_pdu_create(SNMP_MSG_GET);
    read_objid("sPDUOutletCtl.8", anOID, &anOID_len);
    snmp_add_null_var(pdu, anOID, anOID_len);

    /*
     * 만들어진 요청을 보낸다. 
     * 함수이름에서 알수 있듯이 동기요청이다. 
     * 즉 응답이 올때까지 기다린다. 
     * 물론 비동기 요청도 있다. 
     * 해당 세션으로 pdu를 보내고 response로 응답을 
     * 받아온다. 
     */
    status = snmp_synch_response(ss, pdu, &response);


    /*
     * 리턴값이 넘어왔다면..  
     * 값을 검사한다.  
     */
    if (status == STAT_SUCCESS && response->errstat == SNMP_ERR_NOERROR)
    {
        int count = 1;
        /*
         *  읽어온 정보를 분석해서 출력한다.  
         */
        for (vars = response->variables; vars; vars = vars->next_variable)
        {
            char *sp = (char *)malloc(1 + vars->val_len);
            memcpy(sp, vars->val.string, vars->val_len);
            sp[vars->val_len] = ' ';
            /*
             * 따로 설명할건데, 
             * 응답은 자신의 고유타입을 가지고 있다. 
             * 즉 일반 스트링인지, 숫자인지, 시간관련된 
             * 값인지에 대한 정보를 가지고 있다. 
             * 그럼으로 타입에 따라서 처리방법을 달리해야 한다. 
             */
            switch(vars->type)
            {
                case ASN_OCTET_STR:
                {
                    printf("value #%d is a string: %s\", count++, sp);
                    break;
                }
                case ASN_GAUGE:
                case ASN_COUNTER:
                case ASN_TIMETICKS:
                case ASN_INTEGER:
                {
                    int a;
                    memcpy((void *)&a, sp, 4);
                    printf("value #%d is a INTEGER: %d\", count++, a);
                    break;
                }
                case ASN_IPADDRESS:
                {
                    int a;
                    struct in_addr st_addr;
    
                    memcpy((void *)&a, sp, 4);
                    st_addr.s_addr = a;
                    printf("value #%d is a IPADDR: %s\", count++, inet_ntoa(st_addr));
                    break;
                }   
            }
            free(sp);
        }
    }
    else
    {
        if (status == STAT_SUCCESS)
        {
            fprintf(stderr, "Err : %s\", snmp_errstring(response->errstat));
        }
        else
        {
            snmp_sess_perror("snmpget", ss);
        }
    }

    if (response)
    {
        snmp_free_pdu(response);
    }
    snmp_close(ss);

}
