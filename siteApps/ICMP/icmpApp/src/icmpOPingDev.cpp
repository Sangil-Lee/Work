/* Example showing how to register a new command with iocsh */

#include <epicsExport.h>
#include <iocsh.h>
#include "icmpOPingDev.h"

//after compile
//sudo install -o root -g root -m u=rxs,g=rx,o=x -t /usr/local/bin icmp

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
		firstTime = 0;
	};
}
epicsExportRegistrar(icmpOPingInitRegisterCommands);

//icmpOPingInit
epicsShareFunc int icmpOPingInit(const char *ipfile, const char *device, int timeout)
{
#if 0
	printf("======>Filename:%s, Device:%s, Timeout:%d\n",ipfile, device,timeout);
	return(1);
	gOPingmanager = new OPingManager(string(ipfile),string(device),timeout);
#endif
	return 0;
};

devicmpOPingSoft devICMPSoft={ 5, NULL, NULL, init_record, NULL, icmp_oping };
epicsExportAddress(dset,devICMPSoft);

int ICMPDebug = 0;
epicsExportAddress(int, ICMPDebug);


OPingManager::OPingManager(const string filename, const string device, const int timeout):ruid(0),euid(0),
			suid(0),rgid(0),egid(0),sgid(0),send_qos(0),send_ttl(64)
{
	mfilename = filename;
	mdevice = device;
	mtimeout = timeout;
	init();
}

OPingManager::~OPingManager()
{
	dropPrivileges();
	ping_destroy(ping);
	context_destroy_all();
}

void OPingManager::init()
{
	gainPrivileges();
	opingInit();
}

int OPingManager::gainPrivileges()
{
	if (getresuid(&ruid, &euid, &suid) == -1) {
		fprintf(stderr, "Cannot obtain user identity: %m.\n");
		return EXIT_FAILURE;
	}
	if (getresgid(&rgid, &egid, &sgid) == -1) {
		fprintf(stderr, "Cannot obtain group identity: %m.\n");
		return EXIT_FAILURE;
	}
	if (ruid != (uid_t)TARGET_UID && ruid < (uid_t)UID_MIN) {
		fprintf(stderr, "Invalid user.\n");
		return EXIT_FAILURE;
	}
	if (rgid != (gid_t)TARGET_UID && rgid < (gid_t)GID_MIN) {
		fprintf(stderr, "Invalid group.\n");
		return EXIT_FAILURE;
	}

	/* Switch to target user. setuid bit handles this, but doing it again does no harm. */
	if (seteuid((uid_t)TARGET_UID) == -1) {
		fprintf(stderr, "Insufficient user privileges.\n");
		return EXIT_FAILURE;
	}

	/* Switch to target group. setgid bit handles this, but doing it again does no harm.
	 * If TARGET_UID == 0, we need no setgid bit, as root has the privilege. */
	if (setegid((gid_t)TARGET_GID) == -1) {
		fprintf(stderr, "Insufficient group privileges.\n");
		return EXIT_FAILURE;
	}

	return (0);
}

int OPingManager::dropPrivileges()
{
	int gerr = 0, uerr =0;
	if (setresgid(rgid, rgid, rgid) == -1) {
		gerr = errno;
		if (!gerr)
			gerr = EINVAL;
	};

	if (setresuid(ruid, ruid, ruid) == -1) {
		uerr = errno;
		if (!uerr)
			uerr = EINVAL;
	};

	if (uerr || gerr) {
		if (uerr)
			fprintf(stderr, "Cannot drop user privileges: %s.\n", strerror(uerr));
		if (gerr)
			fprintf(stderr, "Cannot drop group privileges: %s.\n", strerror(gerr));
		return EXIT_FAILURE;
	};
	return (0);
}

int OPingManager::opingInit()
{
	if ((ping = ping_construct ()) == NULL)
	{
		fprintf (stderr, "ping_construct failed\n");
		return (-1);
	}

	if (ping_setopt (ping, PING_OPT_TTL, &send_ttl) != 0)
	{
		fprintf (stderr, "Setting TTL to %i failed: %s\n",
				send_ttl, ping_get_error (ping));
			return (-1);
	}

	if (ping_setopt (ping, PING_OPT_QOS, &send_qos) != 0)
	{
		fprintf (stderr, "Setting TOS to %i failed: %s\n",
				send_qos, ping_get_error (ping));
			return (-1);
	}

	{
		double temp_sec;
		double temp_nsec;

		interval = 1.0;
		temp_nsec = modf (interval, &temp_sec);
		ts_int.tv_sec  = (time_t) temp_sec;
		ts_int.tv_nsec = (long) (temp_nsec * 1000000000L);
		/* printf ("ts_int = %i.%09li\n", (int) ts_int.tv_sec, ts_int.tv_nsec); */
	}

	if( mtimeout == 0) mtimeout    = PING_DEF_TIMEOUT;

	if (ping_setopt (ping, PING_OPT_TIMEOUT, (void*)(&mtimeout)) != 0)
	{
		fprintf (stderr, "Setting timeout failed: %s\n",
				ping_get_error (ping));
			return (-1);
	}

	addrfamily = AF_INET;
	ping_setopt (ping, PING_OPT_AF, (void *) &addrfamily);

#if 0
	if (ping_setopt (ping, PING_OPT_SOURCE, (void *) NULL) != 0)
	{
		fprintf (stderr, "Setting source address failed: %s\n",
				ping_get_error (ping));
	}
#endif

	if (mdevice.c_str() != NULL)
	{
		if (ping_setopt (ping, PING_OPT_DEVICE, (void *)mdevice.c_str()) != 0)
		{
			fprintf (stderr, "Setting device failed: %s\n",
					ping_get_error (ping));
			return (-1);
		}
	}

	return (0);
}

int OPingManager::pre_loop_hook ()
{
	for (iter = (pingobj_iter_t *)ping_iterator_get (ping);
			iter != NULL;
			iter = (pingobj_iter_t *)ping_iterator_next (iter))
	{
		ping_context_t *ctx;
		size_t buffer_size;

		ctx = (ping_context_t *)ping_iterator_get_context (iter);
		if (ctx == NULL) continue;

		buffer_size = 0;
		ping_iterator_get_info (iter, PING_INFO_DATA, NULL, &buffer_size);

		printf ("PING %s (%s) %zu bytes of data.\n",
				ctx->host, ctx->addr, buffer_size);
	}

	return (0);
}

int OPingManager::Ping()
{
	int status = ping_send (ping);
	if (status == -EINTR)
	{
		fprintf (stderr, "ping_send failed: %s\n",
				ping_get_error (ping));
		return (status);
	}
	else if (status < 0)
	{
		fprintf (stderr, "ping_send failed: %s\n",
				ping_get_error (ping));
		return (status);
	};

	int index = 0;
	for (iter = (pingobj_iter_t*)ping_iterator_get (ping);
			iter != NULL;
			iter = (pingobj_iter_t*)ping_iterator_next (iter))
	{
		update_host_hook (iter, index);
		index++;
	}

	if(gettimeofday(&tv_end, NULL) < 0)
	{
		fprintf (stderr, "ping_send failed: %s\n",
				ping_get_error (ping));
		return(-1);
	}

	time_calc (&ts_wait, &ts_int, &tv_begin, &tv_end);
	while ((status = nanosleep (&ts_wait, &ts_wait)) != 0)
	{
		if (errno == EINTR) continue;
		else break;
	};

	return (0);
}

void OPingManager::context_destroy_all()
{
	pingobj_iter_t *iter;
	for (iter = (pingobj_iter_t*)ping_iterator_get (ping); iter != NULL;
			iter =(pingobj_iter_t*)ping_iterator_next (iter))
	{
		ping_context_t *context;
		context = ping_iterator_get_context (iter);
		ping_iterator_set_context (iter, NULL);
		context_destroy (context);
	}
}

void OPingManager::context_destroy (ping_context_t *context)
{
	if (context == NULL) return;
	free (context);
}

void OPingManager::update_host_hook (pingobj_iter_t *iter, int index)
{
	double          latency;
	unsigned int    sequence;
	int             recv_ttl;
	uint8_t         recv_qos;
	char            recv_qos_str[16];
	size_t          buffer_len;
	size_t          data_len;
	ping_context_t *context;

	latency = -1.0;
	buffer_len = sizeof (latency);
	ping_iterator_get_info (iter, PING_INFO_LATENCY, &latency, &buffer_len);

	sequence = 0;
	buffer_len = sizeof (sequence);
	ping_iterator_get_info (iter, PING_INFO_SEQUENCE,
			&sequence, &buffer_len);

	recv_ttl = -1;
	buffer_len = sizeof (recv_ttl);
	ping_iterator_get_info (iter, PING_INFO_RECV_TTL,
			&recv_ttl, &buffer_len);

	recv_qos = 0;
	buffer_len = sizeof (recv_qos);
	ping_iterator_get_info (iter, PING_INFO_RECV_QOS, &recv_qos, &buffer_len);

	data_len = 0;
	ping_iterator_get_info (iter, PING_INFO_DATA, NULL, &data_len);

	context = (ping_context_t *) ping_iterator_get_context (iter);

# define HOST_PRINTF(...) printf(__VA_ARGS__)
	update_context (context, latency);

	if (latency > 0.0)
	{
		HOST_PRINTF ("%zu bytes from %s (%s): icmp_seq=%u ttl=%i ",
				data_len,
				context->host, context->addr,
				sequence, recv_ttl);
		HOST_PRINTF ("time=%.2f ms\n", latency);
	}
	else /* if (!(latency > 0.0)) */
	{
		HOST_PRINTF ("echo reply from %s (%s): icmp_seq=%u timeout\n",
				context->host, context->addr, sequence);
		failist.push_back(context->addr);
	};
}

void OPingManager::update_context (ping_context_t *ctx, double latency)
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

void OPingManager::time_normalize (struct timespec *ts)
{
	while (ts->tv_nsec < 0) 
	{
		if (ts->tv_sec == 0)
		{
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


void OPingManager::time_calc (struct timespec *ts_dest, const struct timespec *ts_int,
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

int OPingManager::addHost()
{
	if (mfilename.c_str() != NULL)
	{
		FILE *infile;
		char line[256];
		char host[256];

		infile = fopen(mfilename.c_str(), "r");

		if (infile == NULL)
		{
			fprintf (stderr, "Opening %s failed: %s\n",mfilename.c_str(), strerror(errno));
			return (-1);
		}

		while (fgets(line, sizeof(line), infile))
		{
			/* Strip whitespace */
			if (sscanf(line, "%s", host) != 1)
				continue;

			if ((host[0] == 0) || (host[0] == '#'))
				continue;

			if (ping_host_add(ping, host) < 0)
			{
				const char *errmsg = ping_get_error (ping);

				fprintf (stderr, "Adding host `%s' failed: %s\n", host, errmsg);
				continue;
			}
			else
			{
				iplist.push_back(string(host));
				//host_num++;
			}
		};

		fclose(infile);
	}
	return (0);
}

int OPingManager::ping_initialize_contexts ()
{
	if (ping == NULL) return (EINVAL);

	int index = 0;
	for (iter = (pingobj_iter_t*)ping_iterator_get (ping); iter != NULL;
			iter = (pingobj_iter_t*)ping_iterator_next (iter))
	{
		ping_context_t *context;
		size_t buffer_size;

		context = context_create();
		context->index = index;

		buffer_size = sizeof (context->host);
		ping_iterator_get_info (iter, PING_INFO_HOSTNAME, context->host, &buffer_size);

		buffer_size = sizeof (context->addr);
		ping_iterator_get_info (iter, PING_INFO_ADDRESS, context->addr, &buffer_size);

		ping_iterator_set_context (iter, (void *) context);

		index++;
	}

	return (0);
}

ping_context_t *OPingManager::context_create()
{
	ping_context_t *ret;

	if ((ret = (ping_context_t*)malloc (sizeof (ping_context_t))) == NULL)
		return (NULL);

	memset (ret, '\0', sizeof (ping_context_t));

	ret->latency_total = 0.0;
	return (ret);
} 


long init_record (icmpRecord* precord)
{
	icmpRecord *picmp = (icmpRecord*)precord;

	DBADDR *pdbAddr = NULL;

	string ipfile = string(picmp->filename);
	string device = string(picmp->device);
	int timeout = picmp->timeout;

	OPingManager *gOPingmanager = new OPingManager(string(ipfile),string(device),timeout);

	for(int i = 0;i <= ICMP_FILDZ;i++)
	{
		pdbAddr = NULL;
		switch(i)
		{
			case ICMP_FILD:
				pdbAddr = (DBADDR*)dbGetPdbAddrFromLink(&picmp->inp);
				break;
			case ICMP_FILDA:
				pdbAddr = (DBADDR*)dbGetPdbAddrFromLink(&picmp->inpa);
				break;
			case ICMP_FILDB:
				pdbAddr = (DBADDR*)dbGetPdbAddrFromLink(&picmp->inpb);
				break;
			case ICMP_FILDC:
				pdbAddr = (DBADDR*)dbGetPdbAddrFromLink(&picmp->inpc);
				break;
			case ICMP_FILDD:
				pdbAddr = (DBADDR*)dbGetPdbAddrFromLink(&picmp->inpd);
				break;
			case ICMP_FILDE:
				pdbAddr = (DBADDR*)dbGetPdbAddrFromLink(&picmp->inpe);
				break;
			case ICMP_FILDF:
				pdbAddr = (DBADDR*)dbGetPdbAddrFromLink(&picmp->inpf);
				break;
			case ICMP_FILDG:
				pdbAddr = (DBADDR*)dbGetPdbAddrFromLink(&picmp->inpg);
				break;
			case ICMP_FILDH:
				pdbAddr = (DBADDR*)dbGetPdbAddrFromLink(&picmp->inph);
				break;
			case ICMP_FILDI:
				pdbAddr = (DBADDR*)dbGetPdbAddrFromLink(&picmp->inpi);
				break;
			case ICMP_FILDJ:
				pdbAddr = (DBADDR*)dbGetPdbAddrFromLink(&picmp->inpj);
				break;
			case ICMP_FILDK:
				pdbAddr = (DBADDR*)dbGetPdbAddrFromLink(&picmp->inpk);
				break;
			case ICMP_FILDL:
				pdbAddr = (DBADDR*)dbGetPdbAddrFromLink(&picmp->inpl);
				break;
			case ICMP_FILDM:
				pdbAddr = (DBADDR*)dbGetPdbAddrFromLink(&picmp->inpm);
				break;
			case ICMP_FILDN:
				pdbAddr = (DBADDR*)dbGetPdbAddrFromLink(&picmp->inpn);
				break;
			case ICMP_FILDO:
				pdbAddr = (DBADDR*)dbGetPdbAddrFromLink(&picmp->inpo);
				break;
			case ICMP_FILDP:
				pdbAddr = (DBADDR*)dbGetPdbAddrFromLink(&picmp->inpp);
				break;
			case ICMP_FILDQ:
				pdbAddr = (DBADDR*)dbGetPdbAddrFromLink(&picmp->inpq);
				break;
			case ICMP_FILDR:
				pdbAddr = (DBADDR*)dbGetPdbAddrFromLink(&picmp->inpr);
				break;
			case ICMP_FILDS:
				pdbAddr = (DBADDR*)dbGetPdbAddrFromLink(&picmp->inps);
				break;
			case ICMP_FILDT:
				pdbAddr = (DBADDR*)dbGetPdbAddrFromLink(&picmp->inpt);
				break;
			case ICMP_FILDU:
				pdbAddr = (DBADDR*)dbGetPdbAddrFromLink(&picmp->inpu);
				break;
			case ICMP_FILDV:
				pdbAddr = (DBADDR*)dbGetPdbAddrFromLink(&picmp->inpv);
				break;
			case ICMP_FILDW:
				pdbAddr = (DBADDR*)dbGetPdbAddrFromLink(&picmp->inpw);
				break;
			case ICMP_FILDX:
				pdbAddr = (DBADDR*)dbGetPdbAddrFromLink(&picmp->inpx);
				break;
			case ICMP_FILDY:
				pdbAddr = (DBADDR*)dbGetPdbAddrFromLink(&picmp->inpy);
				break;
			case ICMP_FILDZ:
				pdbAddr = (DBADDR*)dbGetPdbAddrFromLink(&picmp->inpz);
				break;
			default:
				pdbAddr = (DBADDR*)dbGetPdbAddrFromLink(&picmp->inp);
				break;
		};

		if(pdbAddr!=NULL)
		{
			dbCommon *pCommon = (dbCommon*)pdbAddr->precord;
			string ipdesc = string(pCommon->desc);
			printf("Link IP:%s\n", ipdesc.c_str());
		};
	};

	picmp->dpvt = (void*)gOPingmanager;

	return(0);
}


long icmp_oping(icmpRecord* precord)
{
	icmpRecord *picmp = (icmpRecord*)precord;

	return (0);
}
