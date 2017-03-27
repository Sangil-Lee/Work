/*
 * Everything after this is for snmpSessionsShow()
*/

#define epicsOk    (0)
#define epicsError (-1)

#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>

#include "requireNetSNMPversion.h"

typedef struct request_list {
  struct request_list *next_request;
  long            request_id;     /* request id */
  long            message_id;     /* message id */
  netsnmp_callback callback;      /* user callback per request (NULL if unused
) */
  void           *cb_data;        /* user callback data per request (NULL if u
nused) */
  int             retries;        /* Number of retries */
  u_long          timeout;        /* length to wait for timeout */
  struct timeval  time;   /* Time this request was made */
  struct timeval  expire; /* time this request is due to expire */
  struct snmp_session *session;
  netsnmp_pdu    *pdu;    /* The pdu for this request
                           * (saved so it can be retransmitted */
} netsnmp_request_list;

struct snmp_internal_session {
  netsnmp_request_list *requests;     /* Info about outstanding requests */
  netsnmp_request_list *requestsEnd;  /* ptr to end of list */
  int             (*hook_pre) (netsnmp_session *, netsnmp_transport *,
                               void *, int);
  int             (*hook_parse) (netsnmp_session *, netsnmp_pdu *,
                                 u_char *, size_t);
  int             (*hook_post) (netsnmp_session *, netsnmp_pdu *, int);
  int             (*hook_build) (netsnmp_session *, netsnmp_pdu *,
                                 u_char *, size_t *);
  int             (*hook_realloc_build) (netsnmp_session *,
                                         netsnmp_pdu *, u_char **,
                                         size_t *, size_t *);
  int             (*check_packet) (u_char *, size_t);
  netsnmp_pdu    *(*hook_create_pdu) (netsnmp_transport *,
                                      void *, size_t);

  u_char         *packet;
  size_t          packet_len, packet_size;
};

#if devSnmp_NETSNMP_VERSION < 50700
  /*
     In net-snmp-5.7.1 (and possibly earlier) this is defined for us in snmp_api.h
     We define it here if this is an older net-snmp version
  */
  struct session_list {
    struct session_list *next;
    netsnmp_session *session;
    netsnmp_transport *transport;
    struct snmp_internal_session *internal;
  };
#endif

extern struct session_list *Sessions;

int snmpHostsShow()
{
  return(epicsOk);
}

int snmpSessionsShow()
{
  struct session_list *sessions = Sessions;
  struct snmp_internal_session *isp;
  int i = 0, s;

  sessions = Sessions;

  for (s = 0; sessions != NULL; sessions = sessions->next, s++) {
    isp = sessions->internal;

    for (i = 0; isp->requestsEnd != NULL;
        isp->requestsEnd = isp->requestsEnd->next_request, i++);

    printf("Sessions %d internal %d\n", s, i);
  }
  return(i);
}

