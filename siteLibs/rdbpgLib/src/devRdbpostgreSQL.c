/* devRdbpostgreSQLSoft.c */
/* Example device support module */

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
#include "menuScan.h"
//#include "ifstatRecord.h"
#include "rdbpostgreSQLRecord.h"
#include "epicsExport.h"

/*Create the dset for devRdbpostgreSQLSoft */
static long init_record();
static long read_rdb();

//struct ifstat_driver driver;


struct {
	long		number;
	DEVSUPFUN	report;
	DEVSUPFUN	init;
	DEVSUPFUN	init_record;
	DEVSUPFUN	get_ioint_info;
	DEVSUPFUN	read_rdb;
}devRdbpostgreSQLSoft={
	5,
	NULL,
	NULL,
	init_record,
	NULL,
	read_rdb
};

epicsExportAddress(dset,devRdbpostgreSQLSoft);

int RdbpostgreSQLDebug = 0;
epicsExportAddress(int, RdbpostgreSQLDebug);

static long init_record(struct rdbpostgreSQLRecord	*prdbpostgreSQL)
{
	char *dopts = NULL;

    if(recGblInitConstantLink(&prdbpostgreSQL->inp,DBF_DOUBLE,&prdbpostgreSQL->val))
         prdbpostgreSQL->udf = FALSE;

	//prdbpostgreSQL->dpvt = &driver;

    return(0);
}

static long read_rdb(struct rdbpostgreSQL	*prdbpostgreSQL)
{
    long status;
	epicsTimeStamp currTime;
	epicsTimeGetCurrent (&currTime);

	int type = prdbpostgreSQL->tonc;
	double scale;

	if(RdbpostgreSQLDebug)
		printf("Scan(%d), In-B:%f Out-B:%f, Delay:%f \n",prdbpostgreSQL->scan, kbin, kbout, delay);

	prdbpostgreSQL->osec = currTime.secPastEpoch;
	prdbpostgreSQL->ouse = currTime.nsec;

    status = dbGetLink(&(prdbpostgreSQL->inp),DBF_DOUBLE, &(prdbpostgreSQL->val),0,0);
    /*If return was succesful then set undefined false*/
    if(!status) prdbpostgreSQL->udf = FALSE;
    return(0);
}
