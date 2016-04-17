/*
 * NET-SNMP demo
 *
 * This program demonstrates different ways to query a list of hosts
 * for a list of variables.
 *
 * It would of course be faster just to send one query for all variables,
 * but the intention is to demonstrate the difference between synchronous
 * and asynchronous operation.
 *
 * Niels Baggesen (Niels.Baggesen@uni-c.dk), 1999.
 */

#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>

#ifdef HAVE_WINSOCK_H
#include <winsock.h>
#endif

#define DEMO_USE_SNMP_VERSION_3


#ifdef DEMO_USE_SNMP_VERSION_3
const char *our_v3_passphrase = "MySecret";
const char *our_v3_privphrase = "MySecret";
#endif



/*
 * a list of hosts to query
 */
struct host {
  const char *name;
  const char *community;
} hosts[] = {
  { "10.1.5.123",		"public" },
  { NULL }
};

/*
 * a list of variables to query for
 */
struct oid {
  const char *Name;
  oid Oid[MAX_OID_LEN];
  int OidLen;
} oids[] = {
  { "WIENER-CRATE-MIB::outputSupervisionMinSenseVoltage.u0" },
  { "WIENER-CRATE-MIB::outputCurrent.u0" },
  { "WIENER-CRATE-MIB::outputMeasurementTemperature.u0" },
  { "WIENER-CRATE-MIB::outputName.u0" },
  { "WIENER-CRATE-MIB::sysMainSwitch.0" },
  { "WIENER-CRATE-MIB::sysStatus.0" },
  { NULL }
};

/*
 * initialize
 */
void initialize (void)
{
  struct oid *op = oids;
  
  /* Win32: init winsock */
  SOCK_STARTUP;

  /* initialize library */
  init_snmp("asynchapp");

  /* parse the oids */
  while (op->Name) {
    op->OidLen = sizeof(op->Oid)/sizeof(op->Oid[0]);
    if (!read_objid(op->Name, op->Oid, &op->OidLen)) {
      snmp_perror("read_objid");
      exit(1);
    }
    op++;
  }
}

/*
 * simple printing of returned data
 */
int print_result (int status, struct snmp_session *sp, struct snmp_pdu *pdu)
{
  char buf[1024];
  struct variable_list *vp;
   int ix; 
    char *pValStr;

  /* struct timeval now; */
  /* struct timezone tz; */
  /* struct tm *tm; */

  /* gettimeofday(&now, &tz); */
  /* tm = localtime(&now.tv_sec); */
  /* fprintf(stdout, "%.2d:%.2d:%.2d.%.6d ", tm->tm_hour, tm->tm_min, tm->tm_sec, */
  /*         now.tv_usec); */
  switch (status) {
  case STAT_SUCCESS:
    vp = pdu->variables;
    if (pdu->errstat == SNMP_ERR_NOERROR) {
      while (vp) {
        snprint_variable(buf, sizeof(buf), vp->name, vp->name_length, vp);
        /* fprintf(stdout, "%s\n",  buf); */
	    printf("%s\n", buf);
            pValStr = strrchr(buf, ':');
	    /* pValStr = strpbrk(buf,':'); */
	    printf("%s\n", pValStr+2);
	vp = vp->next_variable;
      }
    }
    else {
      for (ix = 1; vp && ix != pdu->errindex; vp = vp->next_variable, ix++)
        ;
      if (vp) snprint_objid(buf, sizeof(buf), vp->name, vp->name_length);
      else strcpy(buf, "(none)");
      fprintf(stdout, "%s: %s: %s\n",
      	sp->peername, buf, snmp_errstring(pdu->errstat));
    }
    return 1;
  case STAT_TIMEOUT:
    fprintf(stdout, "%s: Timeout\n", sp->peername);
    return 0;
  case STAT_ERROR:
    snmp_perror(sp->peername);
    return 0;
  }
  return 0;
}


/*****************************************************************************/

/*
 * poll all hosts in parallel
 */
struct session {
  struct snmp_session *sess;		/* SNMP session data */
  struct oid *current_oid;		/* How far in our poll are we */
} sessions[sizeof(hosts)/sizeof(hosts[0])];

int active_hosts;			/* hosts that we have not completed */

/*
 * response handler
 */
int asynch_response(int operation, struct snmp_session *sp, int reqid, struct snmp_pdu *pdu, void *magic)
{
  struct session *host = (struct session *)magic;
  struct snmp_pdu *req;

  if (operation == NETSNMP_CALLBACK_OP_RECEIVED_MESSAGE) {
    if (print_result(STAT_SUCCESS, host->sess, pdu)) {
      host->current_oid++;			/* send next GET (if any) */
      if (host->current_oid->Name) {
	req = snmp_pdu_create(SNMP_MSG_GET);
	snmp_add_null_var(req, host->current_oid->Oid, host->current_oid->OidLen);
	if (snmp_send(host->sess, req))
	  return 1;
	else {
	  snmp_perror("snmp_send");
	  snmp_free_pdu(req);
	}
      }
    }
  }
  else
    print_result(STAT_TIMEOUT, host->sess, pdu);

  /* something went wrong (or end of variables) 
   * this host not active any more
   */
  active_hosts--;
  return 1;
}

void asynchronous(void)
{
  struct session *hs;
  struct host *hp;

  /* startup all hosts */

  for (hs = sessions, hp = hosts; hp->name; hs++, hp++) {
    struct snmp_pdu *req;
    struct snmp_session sess;

#ifdef DEMO_USE_SNMP_VERSION_3
    snmp_sess_init(&sess);			/* initialize session */
    sess.version = SNMP_VERSION_3;
    sess.peername = strdup(hp->name);
    sess.securityName = strdup(hp->community);
    sess.securityNameLen = strlen(sess.securityName);
    sess.securityLevel = SNMP_SEC_LEVEL_AUTHPRIV;

    /* Authentication Protocol */
    /* MD5 */
    sess.securityAuthProto = usmHMACMD5AuthProtocol;
    sess.securityAuthProtoLen = sizeof(usmHMACMD5AuthProtocol)/sizeof(oid);
    sess.securityAuthKeyLen = USM_AUTH_KU_LEN;

    /* Authentication keys generation */
    if (generate_Ku(sess.securityAuthProto,
		    sess.securityAuthProtoLen,
		    (u_char *) our_v3_passphrase, strlen(our_v3_passphrase),
		    sess.securityAuthKey,
		    &sess.securityAuthKeyLen) != SNMPERR_SUCCESS) {
      snmp_perror("Error!!!!!!!!!!!!");
      snmp_log(LOG_ERR,
	       "Error generating Ku from authentication pass phrase. \n");
      exit(1);
    }

    /* Privacy protocol */
    /* DES */
    sess.securityPrivProto = usmDESPrivProtocol;
    sess.securityPrivProtoLen = USM_PRIV_PROTO_DES_LEN;
    sess.securityPrivKeyLen = USM_PRIV_KU_LEN;

    /* Privacy keys generation */
    if (generate_Ku(sess.securityAuthProto,
		    sess.securityAuthProtoLen,
		    (u_char *) our_v3_privphrase, strlen(our_v3_privphrase),
		    sess.securityPrivKey,
		    &sess.securityPrivKeyLen) != SNMPERR_SUCCESS) {
      printf ("Error generating Ku from privacity pass phrase. \n");
      exit(1);
    }

#endif

    sess.callback = asynch_response;		/* default callback */
    sess.callback_magic = hs;
    if (!(hs->sess = snmp_open(&sess))) {
      snmp_perror("snmp_open");
      continue;
    }
    hs->current_oid = oids;
    req = snmp_pdu_create(SNMP_MSG_GET);	/* send the first GET */
    snmp_add_null_var(req, hs->current_oid->Oid, hs->current_oid->OidLen);

    if (snmp_send(hs->sess, req))
      active_hosts++;
    else {
      snmp_perror("snmp_send");
      snmp_free_pdu(req);
    }
  }

  /* loop while any active hosts */

  while (active_hosts) {
    int fds = 0, block = 1;
    fd_set fdset;
    struct timeval timeout;

    FD_ZERO(&fdset);
    snmp_select_info(&fds, &fdset, &timeout, &block);
    fds = select(fds, &fdset, NULL, NULL, block ? NULL : &timeout);
    if (fds < 0) {
        perror("select failed");
        exit(1);
    }
    if (fds)
        snmp_read(&fdset);
    else
        snmp_timeout();
  }

  /* cleanup */

  for (hp = hosts, hs = sessions; hp->name; hs++, hp++) {
    if (hs->sess) snmp_close(hs->sess);
  }
}

/*****************************************************************************/

int main (int argc, char **argv)
{
  initialize();

  asynchronous();

  return 0;
}
