/* Example showing how to register a new command with iocsh */

#include <bitset>
#include <ext/hash_map>

#include <epicsExport.h>
#include <iocsh.h>

#include "icmpOPingDev.h"
#include "debugPrint.h"

#if 1
namespace __gnu_cxx {
	template <>
		struct hash<std::string> {
			size_t operator() (const std::string& x) const {
				return hash<const char*>()(x.c_str());
			}
		};
};
#endif



////////////////////////////////
//start-oping
//static double  opt_interval   = 1.0;
//static double  opt_timeout    = 2.0;
//static int     opt_addrfamily = PING_DEF_AF;
//static char   *opt_device     = NULL;
//static int     opt_send_ttl   = 64;
//static double  opt_timeout    = PING_DEF_TIMEOUT;
//static char   *opt_srcaddr    = NULL;
//static char   *opt_mark       = NULL;


using namespace std;
__gnu_cxx::hash_map<string, bool> iptable;

static ping_context_t *context_create (void)
{
	ping_context_t *ret;
	if ((ret = (ping_context_t*)malloc (sizeof (ping_context_t))) == NULL)
		return (NULL);

	memset (ret, '\0', sizeof (ping_context_t));
	ret->latency_total = 0.0;
	return (ret);
}

static void context_destroy (ping_context_t *context)
{
	if (context == NULL) return;
	free (context);
}

static int compare_double (void const *arg0, void const *arg1)
{
	double dbl0 = *((double *) arg0);
	double dbl1 = *((double *) arg1);

	if (isnan (dbl0))
	{
		if (isnan (dbl1))
			return 0;
		else
			return 1;
	}
	else if (isnan (dbl1))
		return -1;
	else if (dbl0 < dbl1)
		return -1;
	else if (dbl0 > dbl1)
		return 1;
	else
		return 0;
}

static void clean_history (ping_context_t *ctx)
{
	size_t i;
	if (!ctx->history_dirty)
		return;
	/* Copy all values from by_time to by_value. */
	memcpy (ctx->history_by_value, ctx->history_by_time,
			sizeof (ctx->history_by_time));

	/* Sort all RTTs. */
	qsort (ctx->history_by_value, ctx->history_size, sizeof
			(ctx->history_by_value[0]), compare_double);

	/* Update the number of received RTTs. */
	ctx->history_received = 0;
	for (i = 0; i < ctx->history_size; i++)
		if (!isnan (ctx->history_by_value[i]))
			ctx->history_received++;

	/* Mark as clean. */
	ctx->history_dirty = 0;
}

static double percentile_to_latency (ping_context_t *ctx, double percentile)
{
	size_t index;
	clean_history (ctx);

	/* Not a single packet was received successfully. */
	if (ctx->history_received == 0)
		return NAN;

	if (percentile <= 0.0)
		index = 0;
	else if (percentile >= 100.0)
		index = ctx->history_received - 1;
	else
	{
		index = (size_t) ceil ((percentile / 100.0) * ((double) ctx->history_received));
		assert (index > 0);
		index--;
	}

	return (ctx->history_by_value[index]);
}

static double context_get_packet_loss (const ping_context_t *ctx)
{
	if (ctx == NULL) return (-1.0);

	if (ctx->req_sent < 1) return (0.0);
	return (100.0 * (ctx->req_sent - ctx->req_rcvd)
			/ ((double) ctx->req_sent));
}

static int ping_initialize_contexts (pingobj_t *ping) /* {{{ */
{
	pingobj_iter_t *iter;

	if (ping == NULL)
		return (EINVAL);

	int index = 0;

	for (iter = ping_iterator_get (ping);
			iter != NULL;
			iter = ping_iterator_next (iter))
	{
		ping_context_t *context;
		size_t buffer_size;

		context = context_create ();
		context->index = index;

		buffer_size = sizeof (context->host);
		ping_iterator_get_info (iter, PING_INFO_HOSTNAME, context->host, &buffer_size);

		buffer_size = sizeof (context->addr);
		ping_iterator_get_info (iter, PING_INFO_ADDRESS, context->addr, &buffer_size);

		ping_iterator_set_context (iter, (void *) context);

		index++;
	};

	return (0);
}

static void time_normalize (struct timespec *ts)
{
	while (ts->tv_nsec < 0)
	{
		if (ts->tv_sec == 0) {
			ts->tv_nsec = 0;
			return;
		};

		ts->tv_sec  -= 1;
		ts->tv_nsec += 1000000000;
	};

	while (ts->tv_nsec >= 1000000000)
	{
		ts->tv_sec  += 1;
		ts->tv_nsec -= 1000000000;
	};
}

static void time_calc (struct timespec *ts_dest, const struct timespec *ts_int,
		const struct timeval  *tv_begin, const struct timeval  *tv_end)
{
	ts_dest->tv_sec = tv_begin->tv_sec + ts_int->tv_sec;
	ts_dest->tv_nsec = (tv_begin->tv_usec * 1000) + ts_int->tv_nsec;
	time_normalize (ts_dest);

	/* Assure that `(begin + interval) > end'.
	 * This may seem overly complicated, but `tv_sec' is of type `time_t'
	 * which may be `unsigned. *sigh* */
	if ((tv_end->tv_sec > ts_dest->tv_sec)
			|| ((tv_end->tv_sec == ts_dest->tv_sec)
				&& ((tv_end->tv_usec * 1000) > ts_dest->tv_nsec)))
	{
		ts_dest->tv_sec  = 0;
		ts_dest->tv_nsec = 0;
		return;
	}

	ts_dest->tv_sec = ts_dest->tv_sec - tv_end->tv_sec;
	ts_dest->tv_nsec = ts_dest->tv_nsec - (tv_end->tv_usec * 1000);
	time_normalize (ts_dest);
}

static int pre_loop_hook (pingobj_t *ping)
{
	pingobj_iter_t *iter;

	for (iter = ping_iterator_get (ping);
			iter != NULL;
			iter = ping_iterator_next (iter))
	{
		ping_context_t *ctx;
		size_t buffer_size;

		ctx =(ping_context_t*)ping_iterator_get_context (iter);
		if (ctx == NULL)
			continue;

		buffer_size = 0;
		ping_iterator_get_info (iter, PING_INFO_DATA, NULL, &buffer_size);

		debug(DEBUG_INFO,"PING %s (%s) %zu bytes of data.\n", ctx->host, ctx->addr, buffer_size);
				
	}

	return (0);
}

static int pre_sleep_hook (__attribute__((unused)) pingobj_t *ping)
{
	fflush (stdout);
	return (0);
}

static int post_sleep_hook (__attribute__((unused)) pingobj_t *ping)
{
	return (0);
}

static void update_context (ping_context_t *ctx, double latency)
{
	ctx->req_sent++;

	if (latency > 0.0)
	{
		ctx->req_rcvd++;
		ctx->latency_total += latency;
	}
	else
	{
		latency = NAN;
	}

	ctx->history_by_time[ctx->history_index] = latency;
	ctx->history_dirty = 1;

	/* Update index and size. */
	ctx->history_index = (ctx->history_index + 1) % HISTORY_SIZE_MAX;
	if (ctx->history_size < HISTORY_SIZE_MAX)
		ctx->history_size++;
}

static void update_host_hook (pingobj_iter_t *iter, __attribute__((unused)) int index)
{
	double          latency;
	unsigned int    sequence;
	int             recv_ttl;
	size_t          buffer_len;
	size_t          data_len;
	ping_context_t *context;

	latency = -1.0;
	buffer_len = sizeof (latency);
	ping_iterator_get_info (iter, PING_INFO_LATENCY,
			&latency, &buffer_len);

	sequence = 0;
	buffer_len = sizeof (sequence);
	ping_iterator_get_info (iter, PING_INFO_SEQUENCE,
			&sequence, &buffer_len);

	recv_ttl = -1;
	buffer_len = sizeof (recv_ttl);
	ping_iterator_get_info (iter, PING_INFO_RECV_TTL,
			&recv_ttl, &buffer_len);

	data_len = 0;
	ping_iterator_get_info (iter, PING_INFO_DATA,
			NULL, &data_len);

	context = (ping_context_t *)ping_iterator_get_context (iter);

	update_context (context, latency);

	if (latency > 0.0)
	{
		debug(DEBUG_INFO,"%zu bytes from %s (%s): icmp_seq=%u ttl=%i ",
				data_len, context->host, context->addr, sequence, recv_ttl);
		debug(DEBUG_INFO,"time=%.2f ms,index(%d)\n", latency, index);

		iptable[context->addr] = 0;
	}
	else /* if (!(latency > 0.0)) */
	{
		debug(DEBUG_INFO,"echo reply from %s (%s): icmp_seq=%u timeout\n",
				context->host, context->addr, sequence);
		iptable[context->addr] = 1;
	}

}

//end-oping
////////////////////////////////

//after compile
//sudo install -o root -g root -m u=rxs,g=rx,o=x -t /usr/local/bin icmp

static oping_option oping_options;
//ICMP Result IOC Shell command registration
static const iocshArg resultArg0 = {"result", iocshArgInt};
static const iocshArg *resultArgs[] = {&resultArg0};
static const iocshFuncDef icmpResultCallFuncDef = 
	{ "icmpresult", 1, resultArgs };

static void icmpResultCallFunc(const iocshArgBuf *args)
{
	icmpResult(args[0].ival);
};

epicsShareFunc int icmpResult(int fail)
{
	__gnu_cxx::hash_map<string, bool>::iterator ipiter;
	for(ipiter = iptable.begin(); ipiter != iptable.end(); ++ipiter)
	{
		string ipaddr = ipiter->first;
		bool val = ipiter->second;
		if(fail)
		{
			if(val==true)
				printf("DisConnected  IP(%s)\n", ipaddr.c_str());	
		} 
		else
		{
			if(val==false)
				printf("Connected  IP(%s)\n", ipaddr.c_str());	
		};
	};

	return 0;
};

//icmpOPingInit IOC Shell command registration
static const iocshArg icmpOPingArg0 = {"ip file name", iocshArgString};
static const iocshArg icmpOPingArg1 = {"device", iocshArgString};
static const iocshArg icmpOPingArg2 = {"timeout", iocshArgInt};
static const iocshArg *icmpOPingArgs[] = {&icmpOPingArg0, &icmpOPingArg1, &icmpOPingArg2};
static const iocshFuncDef icmpOPingInitFuncDef = 
	{ "icmpOPingInit", 3, icmpOPingArgs };

static void icmpOPingCallFunc(const iocshArgBuf *args)
{
	icmpOPingInit(args[0].sval,args[1].sval, args[2].ival);
};

static void icmpOPingInitRegisterCommands(void)
{
	static int firstTime = 1;
	if(firstTime)
	{
		iocshRegister(&icmpOPingInitFuncDef, icmpOPingCallFunc);
		iocshRegister(&icmpResultCallFuncDef, icmpResultCallFunc);
		firstTime = 0;
	};
}
epicsExportRegistrar(icmpOPingInitRegisterCommands);

//icmpOPingInit
epicsShareFunc int icmpOPingInit(const char *iplist, const char *device, int timeout)
{
#if 1
	oping_options.interval = 1.0;
	oping_options.timeout = timeout;
	oping_options.ip_family = PING_DEF_AF;
	oping_options.ttl = 64;
	strcpy(oping_options.device, device);
	strcpy(oping_options.filename, iplist);

    epicsThreadCreate("icmpOPingInit", epicsThreadPriorityHigh, epicsThreadGetStackSize(epicsThreadStackSmall),
                     (EPICSTHREADFUNC)devOPingAsyncThread,(void*)&oping_options);
#endif
	return 0;
};

static void devOPingAsyncThread(oping_option *pOpingopt)
{
	//oping main
	pingobj_t      *ping;
	pingobj_iter_t *iter;

	struct timeval  tv_begin;
	struct timeval  tv_end;
	struct timespec ts_wait;
	struct timespec ts_int;

	oping_option*  pOptions = (oping_option*)pOpingopt;

	int status;
	uid_t saved_set_uid;

	/* Save the old effective user id */
	saved_set_uid = geteuid ();
	/* Set the effective user ID to the real user ID without changing the
	 * saved set-user ID */
	status = seteuid (getuid ());
	if (status != 0)
	{
		debug (DEBUG_ERR, "Temporarily dropping privileges "
				"failed: %s\n", strerror (errno));
		exit (EXIT_FAILURE);
	}

	if ((ping = ping_construct ()) == NULL)
	{
		debug (DEBUG_ERR, "ping_construct failed \n");
		exit (EXIT_FAILURE);
	}

	//if (ping_setopt (ping, PING_OPT_TTL, &opt_send_ttl) != 0)
	if (ping_setopt (ping, PING_OPT_TTL, &pOptions->ttl) != 0)
	{
		debug (DEBUG_ERR, "Setting TTL to %i failed: %s\n",
				pOptions->ttl, ping_get_error (ping));
		exit (EXIT_FAILURE);
	}

	{
		double temp_sec;
		double temp_nsec;

		temp_nsec = modf (pOptions->interval, &temp_sec);
		ts_int.tv_sec  = (time_t) temp_sec;
		ts_int.tv_nsec = (long) (temp_nsec * 1000000000L);
		debug(DEBUG_DEBUG,"ts_int = %i.%09li\n", (int) ts_int.tv_sec, ts_int.tv_nsec);
	}

	//if (ping_setopt (ping, PING_OPT_TIMEOUT, (void*)(&opt_timeout)) != 0)
	if (ping_setopt (ping, PING_OPT_TIMEOUT, (void*)(&pOptions->timeout)) != 0)
	{
		debug (DEBUG_ERR, "Setting timeout failed: %s\n",
				ping_get_error (ping));
	}

	ping_setopt (ping, PING_OPT_AF, (void *)&pOptions->ip_family);

#if 0
	if (opt_srcaddr != NULL)
	{
		if (ping_setopt (ping, PING_OPT_SOURCE, (void *) opt_srcaddr) != 0)
		{
			debug (DEBUG_ERR, "Setting source address failed: %s\n",
					ping_get_error (ping));
		}
	}
#endif

	if (pOptions->device != NULL)
	{
		if (ping_setopt (ping, PING_OPT_DEVICE, (void *)pOptions->device) != 0)
		{
			debug (DEBUG_ERR, "Setting device failed: %s\n",
					ping_get_error (ping));
		}
	}

#if 0
	if (opt_mark != NULL)
	{
		char *endp = NULL;
		int mark = (int) strtol (opt_mark, &endp, /* base = */ 0);
		if ((opt_mark[0] != 0) && (endp != NULL) && (*endp == 0))
		{
			if (ping_setopt(ping, PING_OPT_MARK, (void*)(&mark)) != 0)
			{
				debug (DEBUG_ERR, "Setting mark failed: %s\n",
					ping_get_error (ping));
			}
		}
		else
		{
			debug (DEBUG_ERR, "Ignoring invalid mark: %s\n", optarg);
		}
	}
#endif

		/* Drop privileges */
		status = seteuid (getuid ());
		if (status != 0)
		{
			debug (DEBUG_ERR, "Temporarily dropping privileges failed: %s\n", 
					strerror (errno));
			exit (EXIT_FAILURE);
		}

	/* Regain privileges */
	status = seteuid (saved_set_uid);
	if (status != 0)
	{
		debug (DEBUG_ERR, "Temporarily re-gaining privileges failed: %s\n", 
				strerror (errno));
		exit (EXIT_FAILURE);
	}

	string filename = string(pOptions->filename);

	if (filename.c_str()!= NULL)
	{
		FILE *infile;
		char line[256];
		char host[256];

		infile = fopen(filename.c_str(), "r");
		if(infile==NULL) 
		{
			debug (DEBUG_ERR, "File not found(%s)\n", filename.c_str());
			exit (EXIT_FAILURE);
		}

		while (fgets(line, sizeof(line), infile))
		{
			/* Strip whitespace */
			if (sscanf(line, "%s", host) != 1) continue;
			if ((host[0] == 0) || (host[0] == '#')) continue;

			if (ping_host_add(ping, host) < 0)
			{
				const char *errmsg = ping_get_error (ping);
				debug (DEBUG_ERR, "Adding host `%s' failed: %s\n", host, errmsg);
				continue;
			}
			else
			{
				iptable[host] = 0;
			};
		};
	};

	/* Permanently drop root privileges if we're setuid-root. */
	status = setuid (getuid ());
	if (status != 0)
	{
		debug (DEBUG_ERR, "Dropping privileges failed: %s\n",
				strerror (errno));
		exit (EXIT_FAILURE);
	}

	saved_set_uid = (uid_t) -1;

	ping_initialize_contexts (ping);
	pre_loop_hook (ping);

	while (TRUE)
	{
		int index;
		int status;

		if (gettimeofday (&tv_begin, NULL) < 0)
		{
			debug (DEBUG_ERR, "gettimeofday");
		}

		status = ping_send (ping);
		if (status == -EINTR)
		{
			continue;
		}
		else if (status < 0)
		{
			debug (DEBUG_ERR, "ping_send failed: %s\n",
					ping_get_error (ping));
			return;
		}

		index = 0;
		for (iter = ping_iterator_get (ping);
				iter != NULL;
				iter = ping_iterator_next (iter))
		{
			update_host_hook (iter, index);
			index++;
		}

		pre_sleep_hook (ping);

		if (gettimeofday (&tv_end, NULL) < 0)
		{
			debug (DEBUG_ERR, "gettimeofday");
			return;
		}

		time_calc (&ts_wait, &ts_int, &tv_begin, &tv_end);

		/* printf ("Sleeping for %i.%09li seconds\n", (int) ts_wait.tv_sec, ts_wait.tv_nsec); */
		while ((status = nanosleep (&ts_wait, &ts_wait)) != 0)
		{
			if (errno == EINTR)
			{
				continue;
			}
			else
			{
				debug (DEBUG_ERR, "nanosleep");
				break;
			}
		}

		post_sleep_hook (ping);
	} /* while (1) */

	ping_destroy(ping);
}

devicmpOPingSoft devICMPSoft={ 5, NULL, NULL, init_record, NULL, icmp_oping };
epicsExportAddress(dset,devICMPSoft);

int ICMPDebug = 0;
epicsExportAddress(int, ICMPDebug);

long init_record (icmpRecord* precord)
{
    if(recGblInitConstantLink(&precord->inp,DBF_DOUBLE,&precord->val))
         precord->udf = FALSE;
	return(0);
}


long icmp_oping(icmpRecord* precord)
{
	icmpRecord *picmp = (icmpRecord*)precord;
	DBADDR *pdbAddr = NULL;
    long status;
	debug(DEBUG_INFO,"=>icmpRecord Name(%s)\n", picmp->name);

	bitset<39> icmpStrValue;

	for(int i = 0;i <= ICMP_FILDAN;i++)
	{
		pdbAddr = NULL;
		switch(i)
		{
			case ICMP_FILDA:
				pdbAddr = (DBADDR*)dbGetPdbAddrFromLink(&picmp->inpa);
				if(pdbAddr!=NULL)
				{
					dbCommon *pCommon = (dbCommon*)pdbAddr->precord;
					string ipdesc = string(pCommon->desc);
					bool bval = iptable[ipdesc];
					if(bval==0)
						icmpStrValue.reset(ICMP_FILDA);
					else
						icmpStrValue.set(ICMP_FILDA);
				};
				break;
			case ICMP_FILDB:
				pdbAddr = (DBADDR*)dbGetPdbAddrFromLink(&picmp->inpb);
				if(pdbAddr!=NULL)
				{
					dbCommon *pCommon = (dbCommon*)pdbAddr->precord;
					string ipdesc = string(pCommon->desc);
					bool bval = iptable[ipdesc];
					if(bval==0)
						icmpStrValue.reset(ICMP_FILDB);
					else
						icmpStrValue.set(ICMP_FILDB);
				};
				break;
			case ICMP_FILDC:
				pdbAddr = (DBADDR*)dbGetPdbAddrFromLink(&picmp->inpc);
				if(pdbAddr!=NULL)
				{
					dbCommon *pCommon = (dbCommon*)pdbAddr->precord;
					string ipdesc = string(pCommon->desc);
					bool bval = iptable[ipdesc];
					if(bval==0)
						icmpStrValue.reset(ICMP_FILDC);
					else
						icmpStrValue.set(ICMP_FILDC);
				};
				break;
			case ICMP_FILDD:
				pdbAddr = (DBADDR*)dbGetPdbAddrFromLink(&picmp->inpd);
				if(pdbAddr!=NULL)
				{
					dbCommon *pCommon = (dbCommon*)pdbAddr->precord;
					string ipdesc = string(pCommon->desc);
					bool bval = iptable[ipdesc];
					if(bval==0)
						icmpStrValue.reset(ICMP_FILDD);
					else
						icmpStrValue.set(ICMP_FILDD);
				};
				break;
			case ICMP_FILDE:
				pdbAddr = (DBADDR*)dbGetPdbAddrFromLink(&picmp->inpe);
				if(pdbAddr!=NULL)
				{
					dbCommon *pCommon = (dbCommon*)pdbAddr->precord;
					string ipdesc = string(pCommon->desc);
					bool bval = iptable[ipdesc];
					if(bval==0)
						icmpStrValue.reset(ICMP_FILDE);
					else
						icmpStrValue.set(ICMP_FILDE);
				};
				break;
			case ICMP_FILDF:
				pdbAddr = (DBADDR*)dbGetPdbAddrFromLink(&picmp->inpf);
				if(pdbAddr!=NULL)
				{
					dbCommon *pCommon = (dbCommon*)pdbAddr->precord;
					string ipdesc = string(pCommon->desc);
					bool bval = iptable[ipdesc];
					if(bval==0)
						icmpStrValue.reset(ICMP_FILDF);
					else
						icmpStrValue.set(ICMP_FILDF);
				};
				break;
			case ICMP_FILDG:
				pdbAddr = (DBADDR*)dbGetPdbAddrFromLink(&picmp->inpg);
				if(pdbAddr!=NULL)
				{
					dbCommon *pCommon = (dbCommon*)pdbAddr->precord;
					string ipdesc = string(pCommon->desc);
					bool bval = iptable[ipdesc];
					if(bval==0)
						icmpStrValue.reset(ICMP_FILDG);
					else
						icmpStrValue.set(ICMP_FILDG);
				};
				break;
			case ICMP_FILDH:
				pdbAddr = (DBADDR*)dbGetPdbAddrFromLink(&picmp->inph);
				if(pdbAddr!=NULL)
				{
					dbCommon *pCommon = (dbCommon*)pdbAddr->precord;
					string ipdesc = string(pCommon->desc);
					bool bval = iptable[ipdesc];
					if(bval==0)
						icmpStrValue.reset(ICMP_FILDH);
					else
						icmpStrValue.set(ICMP_FILDH);
				};
				break;
			case ICMP_FILDI:
				pdbAddr = (DBADDR*)dbGetPdbAddrFromLink(&picmp->inpi);
				if(pdbAddr!=NULL)
				{
					dbCommon *pCommon = (dbCommon*)pdbAddr->precord;
					string ipdesc = string(pCommon->desc);
					bool bval = iptable[ipdesc];
					if(bval==0)
						icmpStrValue.reset(ICMP_FILDI);
					else
						icmpStrValue.set(ICMP_FILDI);
				};
				break;
			case ICMP_FILDJ:
				pdbAddr = (DBADDR*)dbGetPdbAddrFromLink(&picmp->inpj);
				if(pdbAddr!=NULL)
				{
					dbCommon *pCommon = (dbCommon*)pdbAddr->precord;
					string ipdesc = string(pCommon->desc);
					bool bval = iptable[ipdesc];
					if(bval==0)
						icmpStrValue.reset(ICMP_FILDJ);
					else
						icmpStrValue.set(ICMP_FILDJ);
				};
				break;
			case ICMP_FILDK:
				pdbAddr = (DBADDR*)dbGetPdbAddrFromLink(&picmp->inpk);
				if(pdbAddr!=NULL)
				{
					dbCommon *pCommon = (dbCommon*)pdbAddr->precord;
					string ipdesc = string(pCommon->desc);
					bool bval = iptable[ipdesc];
					if(bval==0)
						icmpStrValue.reset(ICMP_FILDK);
					else
						icmpStrValue.set(ICMP_FILDK);
				};
				break;
			case ICMP_FILDL:
				pdbAddr = (DBADDR*)dbGetPdbAddrFromLink(&picmp->inpl);
				if(pdbAddr!=NULL)
				{
					dbCommon *pCommon = (dbCommon*)pdbAddr->precord;
					string ipdesc = string(pCommon->desc);
					bool bval = iptable[ipdesc];
					if(bval==0)
						icmpStrValue.reset(ICMP_FILDL);
					else
						icmpStrValue.set(ICMP_FILDL);
				};
				break;
			case ICMP_FILDM:
				pdbAddr = (DBADDR*)dbGetPdbAddrFromLink(&picmp->inpm);
				if(pdbAddr!=NULL)
				{
					dbCommon *pCommon = (dbCommon*)pdbAddr->precord;
					string ipdesc = string(pCommon->desc);
					bool bval = iptable[ipdesc];
					if(bval==0)
						icmpStrValue.reset(ICMP_FILDM);
					else
						icmpStrValue.set(ICMP_FILDM);
				};
				break;
			case ICMP_FILDN:
				pdbAddr = (DBADDR*)dbGetPdbAddrFromLink(&picmp->inpn);
				if(pdbAddr!=NULL)
				{
					dbCommon *pCommon = (dbCommon*)pdbAddr->precord;
					string ipdesc = string(pCommon->desc);
					bool bval = iptable[ipdesc];
					if(bval==0)
						icmpStrValue.reset(ICMP_FILDN);
					else
						icmpStrValue.set(ICMP_FILDN);
				};
				break;
			case ICMP_FILDO:
				pdbAddr = (DBADDR*)dbGetPdbAddrFromLink(&picmp->inpo);
				if(pdbAddr!=NULL)
				{
					dbCommon *pCommon = (dbCommon*)pdbAddr->precord;
					string ipdesc = string(pCommon->desc);
					bool bval = iptable[ipdesc];
					if(bval==0)
						icmpStrValue.reset(ICMP_FILDO);
					else
						icmpStrValue.set(ICMP_FILDO);
				};
				break;
			case ICMP_FILDP:
				pdbAddr = (DBADDR*)dbGetPdbAddrFromLink(&picmp->inpp);
				if(pdbAddr!=NULL)
				{
					dbCommon *pCommon = (dbCommon*)pdbAddr->precord;
					string ipdesc = string(pCommon->desc);
					bool bval = iptable[ipdesc];
					if(bval==0)
						icmpStrValue.reset(ICMP_FILDP);
					else
						icmpStrValue.set(ICMP_FILDP);
				};
				break;
			case ICMP_FILDQ:
				pdbAddr = (DBADDR*)dbGetPdbAddrFromLink(&picmp->inpq);
				if(pdbAddr!=NULL)
				{
					dbCommon *pCommon = (dbCommon*)pdbAddr->precord;
					string ipdesc = string(pCommon->desc);
					bool bval = iptable[ipdesc];
					if(bval==0)
						icmpStrValue.reset(ICMP_FILDQ);
					else
						icmpStrValue.set(ICMP_FILDQ);
				};
				break;
			case ICMP_FILDR:
				pdbAddr = (DBADDR*)dbGetPdbAddrFromLink(&picmp->inpr);
				if(pdbAddr!=NULL)
				{
					dbCommon *pCommon = (dbCommon*)pdbAddr->precord;
					string ipdesc = string(pCommon->desc);
					bool bval = iptable[ipdesc];
					if(bval==0)
						icmpStrValue.reset(ICMP_FILDR);
					else
						icmpStrValue.set(ICMP_FILDR);
				};
				break;
			case ICMP_FILDS:
				pdbAddr = (DBADDR*)dbGetPdbAddrFromLink(&picmp->inps);
				if(pdbAddr!=NULL)
				{
					dbCommon *pCommon = (dbCommon*)pdbAddr->precord;
					string ipdesc = string(pCommon->desc);
					bool bval = iptable[ipdesc];
					if(bval==0)
						icmpStrValue.reset(ICMP_FILDS);
					else
						icmpStrValue.set(ICMP_FILDS);
				};
				break;
			case ICMP_FILDT:
				pdbAddr = (DBADDR*)dbGetPdbAddrFromLink(&picmp->inpt);
				if(pdbAddr!=NULL)
				{
					dbCommon *pCommon = (dbCommon*)pdbAddr->precord;
					string ipdesc = string(pCommon->desc);
					bool bval = iptable[ipdesc];
					if(bval==0)
						icmpStrValue.reset(ICMP_FILDT);
					else
						icmpStrValue.set(ICMP_FILDT);
				};
				break;
			case ICMP_FILDU:
				pdbAddr = (DBADDR*)dbGetPdbAddrFromLink(&picmp->inpu);
				if(pdbAddr!=NULL)
				{
					dbCommon *pCommon = (dbCommon*)pdbAddr->precord;
					string ipdesc = string(pCommon->desc);
					bool bval = iptable[ipdesc];
					if(bval==0)
						icmpStrValue.reset(ICMP_FILDU);
					else
						icmpStrValue.set(ICMP_FILDU);
				};
				break;
			case ICMP_FILDV:
				pdbAddr = (DBADDR*)dbGetPdbAddrFromLink(&picmp->inpv);
				if(pdbAddr!=NULL)
				{
					dbCommon *pCommon = (dbCommon*)pdbAddr->precord;
					string ipdesc = string(pCommon->desc);
					bool bval = iptable[ipdesc];
					if(bval==0)
						icmpStrValue.reset(ICMP_FILDV);
					else
						icmpStrValue.set(ICMP_FILDV);
				};
				break;
			case ICMP_FILDW:
				pdbAddr = (DBADDR*)dbGetPdbAddrFromLink(&picmp->inpw);
				if(pdbAddr!=NULL)
				{
					dbCommon *pCommon = (dbCommon*)pdbAddr->precord;
					string ipdesc = string(pCommon->desc);
					bool bval = iptable[ipdesc];
					if(bval==0)
						icmpStrValue.reset(ICMP_FILDW);
					else
						icmpStrValue.set(ICMP_FILDW);
				};
				break;
			case ICMP_FILDX:
				pdbAddr = (DBADDR*)dbGetPdbAddrFromLink(&picmp->inpx);
				if(pdbAddr!=NULL)
				{
					dbCommon *pCommon = (dbCommon*)pdbAddr->precord;
					string ipdesc = string(pCommon->desc);
					bool bval = iptable[ipdesc];
					if(bval==0)
						icmpStrValue.reset(ICMP_FILDX);
					else
						icmpStrValue.set(ICMP_FILDX);
				};
				break;
			case ICMP_FILDY:
				pdbAddr = (DBADDR*)dbGetPdbAddrFromLink(&picmp->inpy);
				if(pdbAddr!=NULL)
				{
					dbCommon *pCommon = (dbCommon*)pdbAddr->precord;
					string ipdesc = string(pCommon->desc);
					bool bval = iptable[ipdesc];
					if(bval==0)
						icmpStrValue.reset(ICMP_FILDY);
					else
						icmpStrValue.set(ICMP_FILDY);
				};
				break;
			case ICMP_FILDZ:
				pdbAddr = (DBADDR*)dbGetPdbAddrFromLink(&picmp->inpz);
				if(pdbAddr!=NULL)
				{
					dbCommon *pCommon = (dbCommon*)pdbAddr->precord;
					string ipdesc = string(pCommon->desc);
					bool bval = iptable[ipdesc];
					if(bval==0)
						icmpStrValue.reset(ICMP_FILDZ);
					else
						icmpStrValue.set(ICMP_FILDZ);
				};
				break;
			case ICMP_FILDAA:
				pdbAddr = (DBADDR*)dbGetPdbAddrFromLink(&picmp->inpaa);
				if(pdbAddr!=NULL)
				{
					dbCommon *pCommon = (dbCommon*)pdbAddr->precord;
					string ipdesc = string(pCommon->desc);
					bool bval = iptable[ipdesc];
					if(bval==0)
						icmpStrValue.reset(ICMP_FILDAA);
					else
						icmpStrValue.set(ICMP_FILDAA);
				};
				break;
			case ICMP_FILDAB:
				pdbAddr = (DBADDR*)dbGetPdbAddrFromLink(&picmp->inpab);
				if(pdbAddr!=NULL)
				{
					dbCommon *pCommon = (dbCommon*)pdbAddr->precord;
					string ipdesc = string(pCommon->desc);
					bool bval = iptable[ipdesc];
					if(bval==0)
						icmpStrValue.reset(ICMP_FILDAB);
					else
						icmpStrValue.set(ICMP_FILDAB);
				};
				break;
			case ICMP_FILDAC:
				pdbAddr = (DBADDR*)dbGetPdbAddrFromLink(&picmp->inpac);
				if(pdbAddr!=NULL)
				{
					dbCommon *pCommon = (dbCommon*)pdbAddr->precord;
					string ipdesc = string(pCommon->desc);
					bool bval = iptable[ipdesc];
					if(bval==0)
						icmpStrValue.reset(ICMP_FILDAC);
					else
						icmpStrValue.set(ICMP_FILDAC);
				};
				break;
			case ICMP_FILDAD:
				pdbAddr = (DBADDR*)dbGetPdbAddrFromLink(&picmp->inpad);
				if(pdbAddr!=NULL)
				{
					dbCommon *pCommon = (dbCommon*)pdbAddr->precord;
					string ipdesc = string(pCommon->desc);
					bool bval = iptable[ipdesc];
					if(bval==0)
						icmpStrValue.reset(ICMP_FILDAD);
					else
						icmpStrValue.set(ICMP_FILDAD);
				};
				break;
			case ICMP_FILDAE:
				pdbAddr = (DBADDR*)dbGetPdbAddrFromLink(&picmp->inpad);
				if(pdbAddr!=NULL)
				{
					dbCommon *pCommon = (dbCommon*)pdbAddr->precord;
					string ipdesc = string(pCommon->desc);
					bool bval = iptable[ipdesc];
					if(bval==0)
						icmpStrValue.reset(ICMP_FILDAE);
					else
						icmpStrValue.set(ICMP_FILDAE);
				};
				break;
			case ICMP_FILDAF:
				pdbAddr = (DBADDR*)dbGetPdbAddrFromLink(&picmp->inpaf);
				if(pdbAddr!=NULL)
				{
					dbCommon *pCommon = (dbCommon*)pdbAddr->precord;
					string ipdesc = string(pCommon->desc);
					bool bval = iptable[ipdesc];
					if(bval==0)
						icmpStrValue.reset(ICMP_FILDAF);
					else
						icmpStrValue.set(ICMP_FILDAF);
				};
				break;
			case ICMP_FILDAG:
				pdbAddr = (DBADDR*)dbGetPdbAddrFromLink(&picmp->inpag);
				if(pdbAddr!=NULL)
				{
					dbCommon *pCommon = (dbCommon*)pdbAddr->precord;
					string ipdesc = string(pCommon->desc);
					bool bval = iptable[ipdesc];
					if(bval==0)
						icmpStrValue.reset(ICMP_FILDAG);
					else
						icmpStrValue.set(ICMP_FILDAG);
				};
				break;
			case ICMP_FILDAH:
				pdbAddr = (DBADDR*)dbGetPdbAddrFromLink(&picmp->inpah);
				if(pdbAddr!=NULL)
				{
					dbCommon *pCommon = (dbCommon*)pdbAddr->precord;
					string ipdesc = string(pCommon->desc);
					bool bval = iptable[ipdesc];
					if(bval==0)
						icmpStrValue.reset(ICMP_FILDAH);
					else
						icmpStrValue.set(ICMP_FILDAH);
				};
				break;
			case ICMP_FILDAI:
				pdbAddr = (DBADDR*)dbGetPdbAddrFromLink(&picmp->inpai);
				if(pdbAddr!=NULL)
				{
					dbCommon *pCommon = (dbCommon*)pdbAddr->precord;
					string ipdesc = string(pCommon->desc);
					bool bval = iptable[ipdesc];
					if(bval==0)
						icmpStrValue.reset(ICMP_FILDAI);
					else
						icmpStrValue.set(ICMP_FILDAI);
				};
				break;
			case ICMP_FILDAJ:
				pdbAddr = (DBADDR*)dbGetPdbAddrFromLink(&picmp->inpaj);
				if(pdbAddr!=NULL)
				{
					dbCommon *pCommon = (dbCommon*)pdbAddr->precord;
					string ipdesc = string(pCommon->desc);
					bool bval = iptable[ipdesc];
					if(bval==0)
						icmpStrValue.reset(ICMP_FILDAJ);
					else
						icmpStrValue.set(ICMP_FILDAJ);
				};
				break;
			case ICMP_FILDAK:
				pdbAddr = (DBADDR*)dbGetPdbAddrFromLink(&picmp->inpak);
				if(pdbAddr!=NULL)
				{
					dbCommon *pCommon = (dbCommon*)pdbAddr->precord;
					string ipdesc = string(pCommon->desc);
					bool bval = iptable[ipdesc];
					if(bval==0)
						icmpStrValue.reset(ICMP_FILDAK);
					else
						icmpStrValue.set(ICMP_FILDAK);
				};
				break;
			case ICMP_FILDAL:
				pdbAddr = (DBADDR*)dbGetPdbAddrFromLink(&picmp->inpal);
				if(pdbAddr!=NULL)
				{
					dbCommon *pCommon = (dbCommon*)pdbAddr->precord;
					string ipdesc = string(pCommon->desc);
					bool bval = iptable[ipdesc];
					if(bval==0)
						icmpStrValue.reset(ICMP_FILDAL);
					else
						icmpStrValue.set(ICMP_FILDAL);
				};
				break;
			case ICMP_FILDAM:
				pdbAddr = (DBADDR*)dbGetPdbAddrFromLink(&picmp->inpam);
				if(pdbAddr!=NULL)
				{
					dbCommon *pCommon = (dbCommon*)pdbAddr->precord;
					string ipdesc = string(pCommon->desc);
					bool bval = iptable[ipdesc];
					if(bval==0)
						icmpStrValue.reset(ICMP_FILDAM);
					else
						icmpStrValue.set(ICMP_FILDAM);
				};
				break;
			case ICMP_FILDAN:
				pdbAddr = (DBADDR*)dbGetPdbAddrFromLink(&picmp->inpan);
				if(pdbAddr!=NULL)
				{
					dbCommon *pCommon = (dbCommon*)pdbAddr->precord;
					string ipdesc = string(pCommon->desc);
					bool bval = iptable[ipdesc];
					if(bval==0)
						icmpStrValue.reset(ICMP_FILDAN);
					else
						icmpStrValue.set(ICMP_FILDAN);
				};
				break;
			default:
				break;
		};

		if(pdbAddr!=NULL)
		{
#if 0
			dbCommon *pCommon = (dbCommon*)pdbAddr->precord;
			string ipdesc = string(pCommon->desc);
			bool bval = iptable[ipdesc];
			string record = string(pCommon->rdes->name);
			if(record.compare("bi") == 0)
			{
				long *pfieldLink =  (long*)pdbAddr->pfield;
				pfieldLink[0] = bval;
				dbScanLock(pCommon);
				dbProcess(pCommon);
				dbScanUnlock(pCommon);
			} 
#else
			biRecord *pRec = (biRecord*)pdbAddr->precord;
			string ipdesc = string(((dbCommon*)pRec)->desc);
			bool bval = iptable[ipdesc];
			pRec->val = bval;
			dbScanLock((dbCommon*)pRec);
			dbProcess((dbCommon*)pRec);
			dbScanUnlock((dbCommon*)pRec);
#endif
			debug(DEBUG_INFO,"==>Link IP(%s),Value(%d) <==\n", ipdesc.c_str(),bval);
		};
	};
	strcpy(picmp->val,icmpStrValue.to_string().c_str());

	return (0);
}
