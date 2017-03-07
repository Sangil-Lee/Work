/* Example showing how to register a new command with iocsh */

#include <stdint.h>
#include <signal.h>
#include <string>
#include <cstdio>
#include <cmath>

#include <epicsExport.h>
#include <iocsh.h>
#include "icmpOPingDev.h"

#if 0
typedef struct devMySQLPvt {
	ELLNODE		devRDBNode;
	IOSCANPVT	ioScanPvt;
	dbCommon	*prec;
	epicsMutexId mutexId;
} devMySQLPvt;
#endif

static int     opt_send_ttl   = 64;
static uint8_t opt_send_qos   = 0;
static double  opt_interval   = 1.0;
static double  opt_timeout    = PING_DEF_TIMEOUT;
static int     opt_addrfamily = PING_DEF_AF;
static char   *opt_srcaddr    = NULL;
static char   *opt_device     = NULL;

//icmpOPingInit IOC Shell command registration
static const iocshArg icmpOPingArg0 = {"iplist0", iocshArgString};
static const iocshArg icmpOPingArg1 = {"iplist1", iocshArgString};
static const iocshArg icmpOPingArg2 = {"iplist2", iocshArgString};
static const iocshArg icmpOPingArg3 = {"iplist3", iocshArgString};
static const iocshArg icmpOPingArg4 = {"iplist4", iocshArgString};
static const iocshArg *icmpOPingArgs[] = {&icmpOPingArg0, &icmpOPingArg1, &icmpOPingArg2,&icmpOPingArg3, &icmpOPingArg4};
static const iocshFuncDef icmpOPingInitFuncDef = 
	{ "icmpOPingInit", 5, icmpOPingArgs };

static void icmpOPingCallFunc(const iocshArgBuf *args)
{
	icmpOPingInit(args[0].sval, args[1].sval, args[2].sval, args[3].sval, args[4].sval);
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
epicsShareFunc int icmpOPingInit(const char *iplist1, const char *iplist2, const char *iplist3, 
		const char *iplist4, const char *iplist5 )
{
	pingobj_t      *ping;
	pingobj_iter_t *iter;

	struct sigaction sigint_action;

	struct timeval  tv_begin;
	struct timeval  tv_end;
	struct timespec ts_wait;
	struct timespec ts_int;

	if ((ping = ping_construct ()) == NULL)
	{
		fprintf (stderr, "ping_construct failed\n");
		return (1);
	}

	if (ping_setopt (ping, PING_OPT_TTL, &opt_send_ttl) != 0)
	{
		fprintf (stderr, "Setting TTL to %i failed: %s\n",
				opt_send_ttl, ping_get_error (ping));
	}

	if (ping_setopt (ping, PING_OPT_QOS, &opt_send_qos) != 0)
	{
		fprintf (stderr, "Setting TOS to %i failed: %s\n",
				opt_send_qos, ping_get_error (ping));
	}

	{
		double temp_sec;
		double temp_nsec;

		temp_nsec = modf (opt_interval, &temp_sec);
		ts_int.tv_sec  = (time_t) temp_sec;
		ts_int.tv_nsec = (long) (temp_nsec * 1000000000L);
		/* printf ("ts_int = %i.%09li\n", (int) ts_int.tv_sec, ts_int.tv_nsec); */
	}

	if (ping_setopt (ping, PING_OPT_TIMEOUT, (void*)(&opt_timeout)) != 0)
	{
		fprintf (stderr, "Setting timeout failed: %s\n",
				ping_get_error (ping));
	}

	opt_addrfamily = AF_INET;
	ping_setopt (ping, PING_OPT_AF, (void *) &opt_addrfamily);

	if (opt_srcaddr != NULL)
	{
		if (ping_setopt (ping, PING_OPT_SOURCE, (void *) opt_srcaddr) != 0)
		{
			fprintf (stderr, "Setting source address failed: %s\n",
					ping_get_error (ping));
		}
	}

	if (opt_device != NULL)
	{
		if (ping_setopt (ping, PING_OPT_DEVICE, (void *) opt_device) != 0)
		{
			fprintf (stderr, "Setting device failed: %s\n",
					ping_get_error (ping));
		}
	}

	//for example
	int host_num = 0;
	string host = string("10.1.5.65");
	if (ping_host_add(ping, host.c_str()) < 0)
	{
		const char *errmsg = ping_get_error (ping);
		fprintf (stderr, "Adding host `%s' failed: %s\n", host.c_str(), errmsg);
	}
	else
	{
		host_num++;
	}




	return 0;
};

int ICMPDebug = 0;
epicsExportAddress(int, ICMPDebug);

devicmpOPingSoft icmpOPingDev={ 5, NULL, NULL, init_record, NULL, icmp_oping };
epicsExportAddress(dset,icmpOPingDev);


long init_record (icmpRecord* precord)
{
	icmpRecord *picmp = (icmpRecord*)precord;
	return(0);
}


long icmp_oping(icmpRecord* precord)
{
	icmpRecord *picmp = (icmpRecord*)precord;

	return (0);
}
