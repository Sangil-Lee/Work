/*
 * Sorensen device support
 */

#include <epicsStdio.h>
#include <devCommonGpib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define	DEBUG 1

/******************************************************************************
 *
 * The following define statements are used to declare the names to be used
 * for the dset tables.   
 *
 * A DSET_AI entry must be declared here and referenced in an application
 * database description file even if the device provides no AI records.
 *
 ******************************************************************************/

#define DSET_BI     devBiSorensen		// Not Used
#define DSET_AI     devAiSorensen
#define DSET_AO     devAoSorensen

#include <devGpib.h> /* must be included after DSET defines */

#define TIMEOUT     0.5    /* I/O must complete within this time */
#define TIMEWINDOW  2.0    /* Wait this long after device timeout */

/******************************************************************************
** Not used 
**
** static int
** check_validReply(char *buf, int len) 
** {
** 	int	i, nPeriod = 0;
** 	
** 	if ((buf[0] != '+') && (buf[0] != '-'))
** 		return (-1);
** 	for (i = 1; i < 8; i++) {
** 		if ((buf[i] >= '0') && (buf[i] <= '9'))
** 			continue;
** 		if (buf[i] == '.') {
** 			nPeriod++;
** 			continue;
** 		}
** 		break;
** 	}
** 	if ((i >= 8) && (nPeriod < 2))
** 		return(0);
** 	return(-1);
** }
 ******************************************************************************/



static int
Sorensen_convertDataReplyBI(struct gpibDpvt *pdpvt, int P1, int P2, char **P3)
{
        struct biRecord *pbi = ((struct biRecord *)(pdpvt->precord));

        asynOctet *pasynOctet = pdpvt->pasynOctet;
        void *asynOctetPvt = pdpvt->asynOctetPvt;

        int     len;
        int     val;
        char cmd[64] = {0,};
        char cbuf[64] = {0,};

        strcpy(cmd, "*TST?\r\n");
#ifdef DEBUG
	printf("FUNC::Sorensen_convertDataReplyBI() \n");
	printf("    cmd = %s\n", cmd);
#endif

        if((pasynOctet->setInputEos(asynOctetPvt,pdpvt->pasynUser,NULL,0) != asynSuccess))
                return -1;

        if((pasynOctet->setOutputEos(asynOctetPvt,pdpvt->pasynUser,NULL,0) != asynSuccess))
                return -1;
        if(pasynOctet->write(asynOctetPvt,pdpvt->pasynUser,cmd,strlen(cmd),(size_t *) &len) != asynSuccess)
                return -1;

        pasynOctet->read(asynOctetPvt,pdpvt->pasynUser,cbuf, sizeof(cbuf),(size_t *) &len, NULL);

        if (len > 1)
                val = 0;        // normal
        else
                val = 1;        // abnormal
        pbi->rval = val;

        return 0;
}




static int
Sorensen_convertDataReplyCurrentAI(struct gpibDpvt *pdpvt, int P1, int P2, char **P3)
{
	struct aiRecord *pai = ((struct aiRecord *)(pdpvt->precord));

	asynOctet *pasynOctet = pdpvt->pasynOctet;
	void *asynOctetPvt = pdpvt->asynOctetPvt;

	int	 len;
	char cmd[64] = {0,};
	char cbuf[64] = {0,};

	int addr;
	
	addr = pai->inp.value.gpibio.addr;
	switch (addr) {
		default:
			strcpy(cmd, "MEAS:CURR?\r\n");
			break;
	}
#ifdef DEBUG
	printf("FUNC::Sorensen_convertDataReplyCurrentAI() \n");
	printf("    cmd = %s\n", cmd);
#endif

	if((pasynOctet->setInputEos(asynOctetPvt,pdpvt->pasynUser,NULL,0) != asynSuccess))
		return -1;
	if((pasynOctet->setOutputEos(asynOctetPvt,pdpvt->pasynUser,NULL,0) != asynSuccess))
		return -1;
	if(pasynOctet->write(asynOctetPvt,pdpvt->pasynUser,cmd,strlen(cmd),(size_t *) &len) != asynSuccess)
		return -1;

	memset( cbuf, 0x00, sizeof(cbuf) );
	pasynOctet->read(asynOctetPvt,pdpvt->pasynUser,cbuf, sizeof(cbuf),(size_t *) &len, NULL);

	if (len == 0){	/* no response: timeout	*/
		epicsSnprintf(pdpvt->pasynUser->errorMessage, pdpvt->pasynUser->errorMessageSize, "No Response");
		pai->scan = 10.0;	/* increase scan period	*/
		return -1;	
	} 
	else
		pai->scan = 1.0;	/* increase scan period	*/

	if (len < 1){	/* invalid length	*/
		epicsSnprintf(pdpvt->pasynUser->errorMessage, pdpvt->pasynUser->errorMessageSize, "Invalid reply %d", pdpvt->msgInputLen);
		return -1;	
	}
	
	// if (check_validReply(cbuf, len) < 0) { /* invalid reply data	*/
	//		epicsSnprintf(pdpvt->pasynUser->errorMessage, pdpvt->pasynUser->errorMessageSize, "Invalid reply %d", pdpvt->msgInputLen);
	//		return -1;
	// }

	double rval;

	rval = atof(cbuf);
	pai->rval = rval;

    pai->val = rval;
	pai->udf = 0;

#ifdef DEBUG
	printf("    SG Read Current (%s), cbuf string:(%s) rval:%f val = %g\n", pai->name, cbuf, rval, pai->val);  
#endif

	
	return 0;
} // end of Sorensen_convertDataReplyCurrentAI

static int
Sorensen_convertDataReplyVoltageAI(struct gpibDpvt *pdpvt, int P1, int P2, char **P3)
{
	struct aiRecord *pai = ((struct aiRecord *)(pdpvt->precord));

	asynOctet *pasynOctet = pdpvt->pasynOctet;
	void *asynOctetPvt = pdpvt->asynOctetPvt;

	int	 len;
	char cmd[64] = {0,};
	char cbuf[64] = {0,};

	int addr;
	
	addr = pai->inp.value.gpibio.addr;
	switch (addr) {
		default:
			strcpy(cmd, "MEAS:VOLT?\r\n");
			break;
	}
#ifdef DEBUG
	printf("FUNC::Sorensen_convertDataReplyVoltageAI() \n");
	printf("    cmd = %s\n", cmd);
#endif

	if((pasynOctet->setInputEos(asynOctetPvt,pdpvt->pasynUser,NULL,0) != asynSuccess))
		return -1;
	if((pasynOctet->setOutputEos(asynOctetPvt,pdpvt->pasynUser,NULL,0) != asynSuccess))
		return -1;
	if(pasynOctet->write(asynOctetPvt,pdpvt->pasynUser,cmd,strlen(cmd),(size_t *) &len) != asynSuccess)
		return -1;

	memset( cbuf, 0x00, sizeof(cbuf) );
	pasynOctet->read(asynOctetPvt,pdpvt->pasynUser,cbuf, sizeof(cbuf), (size_t *) &len, NULL);

	if (len == 0){	/* no response: timeout	*/
		epicsSnprintf(pdpvt->pasynUser->errorMessage, pdpvt->pasynUser->errorMessageSize, "No Response");
		pai->scan = 10.0;	/* increase scan period	*/
		return -1;	
	} 
	 else
		pai->scan = 1.0;	/* increase scan period	*/

	if (len < 1){	/* invalid length	*/
		epicsSnprintf(pdpvt->pasynUser->errorMessage, pdpvt->pasynUser->errorMessageSize, "Invalid reply %d", pdpvt->msgInputLen);
		return -1;	
	}
	
//	if (check_validReply(cbuf, len) < 0) { /* invalid reply data	*/
//			epicsSnprintf(pdpvt->pasynUser->errorMessage, pdpvt->pasynUser->errorMessageSize, "Invalid reply %d", pdpvt->msgInputLen);
//			return -1;
//	}

	double val = 0, rval;

	rval = atof(cbuf);
	pai->rval = rval;

#ifdef DEBUG
	printf("    SG Read Volt (%s), cbuf string:(%s) rval:%f val = %g\n", pai->name, cbuf, rval, val);
#endif

	// pai->val = val; // sylee 2016.11.10
	pai->val = rval;
	pai->udf = 0;
	
	return 0;
} // end of Sorensen_convertDataReplyVoltageAI





static int
Sorensen_convertDataSetCurrentAO(struct gpibDpvt *pdpvt, int P1, int P2, char **P3)
{
	// struct aiRecord *pai = ((struct aiRecord *)(pdpvt->precord));
	struct aoRecord *pao = ((struct aoRecord *)(pdpvt->precord));

	asynOctet *pasynOctet = pdpvt->pasynOctet;
	void *asynOctetPvt = pdpvt->asynOctetPvt;

	int	 len;
	char cmd[64] = {0,};
	char cbuf[64] = {0,};

	sprintf(cmd, "SOUR:CURR %e \r\n", pao->val );
#ifdef DEBUG
	printf("FUNC::Sorensen_convertDataSetCurrentAO() \n");
	printf("    cmd = %s\n", cmd);
#endif

	if((pasynOctet->setInputEos(asynOctetPvt,pdpvt->pasynUser,NULL,0) != asynSuccess))
		return -1;
	if((pasynOctet->setOutputEos(asynOctetPvt,pdpvt->pasynUser,NULL,0) != asynSuccess))
		return -1;
	if(pasynOctet->write(asynOctetPvt,pdpvt->pasynUser,cmd,strlen(cmd),(size_t *) &len) != asynSuccess)
		return -1;

	pasynOctet->read(asynOctetPvt,pdpvt->pasynUser,cbuf, sizeof(cbuf),(size_t *) &len, NULL);

	if (len == 0){	/* no response: timeout	*/
		epicsSnprintf(pdpvt->pasynUser->errorMessage, pdpvt->pasynUser->errorMessageSize, "No Response");
		return -1;	
	} 
	
	pao->udf = 0;
	return 0;
} // end of Sorensen_convertDataSetCurrentAO

static int
Sorensen_convertDataSetVoltageAO(struct gpibDpvt *pdpvt, int P1, int P2, char **P3)
{
	// struct aiRecord *pai = ((struct aiRecord *)(pdpvt->precord));
	struct aoRecord *pao = ((struct aoRecord *)(pdpvt->precord));

	asynOctet *pasynOctet = pdpvt->pasynOctet;
	void *asynOctetPvt = pdpvt->asynOctetPvt;

	int	 len;
	char cmd[64] = {0,};
	char cbuf[64] = {0,};

	sprintf(cmd, "SOUR:VOLT %e\r\n", pao->val );
#ifdef DEBUG
	printf("FUNC::Sorensen_convertDataSetVoltageAO() \n");
	printf("    cmd = %s\n", cmd);
#endif

	if((pasynOctet->setInputEos(asynOctetPvt,pdpvt->pasynUser,NULL,0) != asynSuccess))
		return -1;
	if((pasynOctet->setOutputEos(asynOctetPvt,pdpvt->pasynUser,NULL,0) != asynSuccess))
		return -1;
	if(pasynOctet->write(asynOctetPvt,pdpvt->pasynUser,cmd,strlen(cmd),(size_t *) &len) != asynSuccess)
		return -1;

	pasynOctet->read(asynOctetPvt,pdpvt->pasynUser,cbuf, sizeof(cbuf), (size_t *) &len, NULL);

	if (len == 0){	/* no response: timeout	*/
		epicsSnprintf(pdpvt->pasynUser->errorMessage, pdpvt->pasynUser->errorMessageSize, "No Response");
		return -1;	
	} 

	pao->udf = 0;
	return 0;
} // end of Sorensen_convertDataSetVoltageAO


static int
Sorensen_convertOutputEnable(struct gpibDpvt *pdpvt, int P1, int P2, char **P3)
{
	// struct aiRecord *pai = ((struct aiRecord *)(pdpvt->precord));
	struct aoRecord *pao = ((struct aoRecord *)(pdpvt->precord));

	asynOctet *pasynOctet = pdpvt->pasynOctet;
	void *asynOctetPvt = pdpvt->asynOctetPvt;

	int	 len;
	char cmd[64] = {0,};
	char cbuf[64] = {0,};

	sprintf(cmd, "OUTP:STAT 1\r\n" );
#ifdef DEBUG
	printf("FUNC::Sorensen_convertOutputEnable() \n");
	printf("    cmd = %s\n", cmd);
#endif

	if((pasynOctet->setInputEos(asynOctetPvt,pdpvt->pasynUser,NULL,0) != asynSuccess))
		return -1;
	if((pasynOctet->setOutputEos(asynOctetPvt,pdpvt->pasynUser,NULL,0) != asynSuccess))
		return -1;
	if(pasynOctet->write(asynOctetPvt,pdpvt->pasynUser,cmd,strlen(cmd),(size_t *) &len) != asynSuccess)
		return -1;

	pasynOctet->read(asynOctetPvt,pdpvt->pasynUser,cbuf, sizeof(cbuf), (size_t *) &len, NULL);

	/*
	if (len == 0){	//  no response: timeout
		epicsSnprintf(pdpvt->pasynUser->errorMessage, pdpvt->pasynUser->errorMessageSize, "No Response");
		return -1;	
	} 
	*/

	pao->udf = 0;
	return 0;
} // end of Sorensen_convertOutputEnable


static int
Sorensen_convertOutputDisable(struct gpibDpvt *pdpvt, int P1, int P2, char **P3)
{
	// struct aiRecord *pai = ((struct aiRecord *)(pdpvt->precord));
	struct aoRecord *pao = ((struct aoRecord *)(pdpvt->precord));

	asynOctet *pasynOctet = pdpvt->pasynOctet;
	void *asynOctetPvt = pdpvt->asynOctetPvt;

	int	 len;
	char cmd[64] = {0,};
	char cbuf[64] = {0,};

	sprintf(cmd, "OUTP:STAT 0\r\n" );
#ifdef DEBUG
	printf("FUNC::Sorensen_convertOutputDisable() \n");
	printf("    cmd = %s\n", cmd);
#endif

	if((pasynOctet->setInputEos(asynOctetPvt,pdpvt->pasynUser,NULL,0) != asynSuccess))
		return -1;
	if((pasynOctet->setOutputEos(asynOctetPvt,pdpvt->pasynUser,NULL,0) != asynSuccess))
		return -1;
	if(pasynOctet->write(asynOctetPvt,pdpvt->pasynUser,cmd,strlen(cmd),(size_t *) &len) != asynSuccess)
		return -1;

	pasynOctet->read(asynOctetPvt,pdpvt->pasynUser,cbuf, sizeof(cbuf), (size_t *) &len, NULL);

	/*
	if (len == 0){	//  no response: timeout	
		epicsSnprintf(pdpvt->pasynUser->errorMessage, pdpvt->pasynUser->errorMessageSize, "No Response");
		return -1;	
	} 
	*/

	pao->udf = 0;
	return 0;
} // end of Sorensen_convertOutputDisable


static int
Sorensen_convertOutputStatus(struct gpibDpvt *pdpvt, int P1, int P2, char **P3)
{
	struct aiRecord *pai = ((struct aiRecord *)(pdpvt->precord));

	asynOctet *pasynOctet = pdpvt->pasynOctet;
	void *asynOctetPvt = pdpvt->asynOctetPvt;

	int	 len;
	char cmd[64] = {0,};
	char cbuf[64] = {0,};

	int addr;
	
	addr = pai->inp.value.gpibio.addr;
	switch (addr) {
		default:
			strcpy(cmd, "OUTP:STAT?\r\n");
			break;
	}
#ifdef DEBUG
	printf("FUNC::Sorensen_convertOutputStatus() \n");
	printf("    cmd = %s\n", cmd);
#endif

	if((pasynOctet->setInputEos(asynOctetPvt,pdpvt->pasynUser,NULL,0) != asynSuccess))
		return -1;
	if((pasynOctet->setOutputEos(asynOctetPvt,pdpvt->pasynUser,NULL,0) != asynSuccess))
		return -1;
	if(pasynOctet->write(asynOctetPvt,pdpvt->pasynUser,cmd,strlen(cmd),(size_t *) &len) != asynSuccess)
		return -1;

	memset( cbuf, 0x00, sizeof(cbuf) );
	pasynOctet->read(asynOctetPvt,pdpvt->pasynUser,cbuf, sizeof(cbuf), (size_t *) &len, NULL);

	if (len == 0){	/* no response: timeout	*/
		epicsSnprintf(pdpvt->pasynUser->errorMessage, pdpvt->pasynUser->errorMessageSize, "No Response");
		pai->scan = 10.0;	/* increase scan period	*/
		return -1;	
	} 
	 else
		pai->scan = 1.0;	/* increase scan period	*/

	if (len < 1){	/* invalid length	*/
		epicsSnprintf(pdpvt->pasynUser->errorMessage, pdpvt->pasynUser->errorMessageSize, "Invalid reply %d", pdpvt->msgInputLen);
		return -1;	
	}
	
//	if (check_validReply(cbuf, len) < 0) { /* invalid reply data	*/
//			epicsSnprintf(pdpvt->pasynUser->errorMessage, pdpvt->pasynUser->errorMessageSize, "Invalid reply %d", pdpvt->msgInputLen);
//			return -1;
//	}

	int val = 0, rval;

	rval = atoi(cbuf);
	pai->rval = rval;

#ifdef DEBUG
	printf("    SG OutputStatus (%s), cbuf string:(%s) rval:%d val = %d\n", pai->name, cbuf, rval, val);
#endif

	// pai->val = val; // sylee 2016.11.10
	pai->val = rval;
	pai->udf = 0;
	
	return 0;
} // end of Sorensen_convertOutputStatus



/******************************************************************************
 * Array of structures that define all GPIB messages
 * supported for this type of instrument.
 ******************************************************************************/

static struct gpibCmd gpibCmds[] = {
    /* Param 0 -- Get Data */
/* {&DSET_SI, GPIBREAD, IB_Q_LOW, "#RD\r", NULL, 0, 40, GP_convertDataReplyString, 0, 0, NULL, NULL, "\r"}*/
   {&DSET_BI, GPIBCVTIO, IB_Q_LOW, NULL, NULL, 0, 10, Sorensen_convertDataReplyBI, 0, 0, NULL, NULL, NULL},		// status check in DB L# A# @0
   {&DSET_AI, GPIBCVTIO, IB_Q_LOW, NULL, NULL, 0, 10, Sorensen_convertDataReplyCurrentAI, 0, 0, NULL, NULL, NULL},     	// Power in DB L# A# @1 (MEAS:CURR? command: get the current value
   {&DSET_AI, GPIBCVTIO, IB_Q_LOW, NULL, NULL, 0, 10, Sorensen_convertDataReplyVoltageAI, 0, 0, NULL, NULL, NULL},	// Power in DB L# A# @2 (MEAS:VOLT? command: get the voltage valuse
   {&DSET_AO, GPIBCVTIO, IB_Q_LOW, NULL, NULL, 0, 10, Sorensen_convertDataSetCurrentAO, 0, 0, NULL, NULL, NULL},     	// Power in DB L# A# @3 (SOUR:CURR command: get the current value
   {&DSET_AO, GPIBCVTIO, IB_Q_LOW, NULL, NULL, 0, 10, Sorensen_convertDataSetVoltageAO, 0, 0, NULL, NULL, NULL}, 	// Power in DB L# A# @4 (SOUR:VOLT command: get the voltage valuse
   {&DSET_AO, GPIBCVTIO, IB_Q_LOW, NULL, NULL, 0, 10, Sorensen_convertOutputEnable, 0, 0, NULL, NULL, NULL}, 	// Power in DB L# A# @5 (OUTP:STAT 1 command:  set output enable
   {&DSET_AO, GPIBCVTIO, IB_Q_LOW, NULL, NULL, 0, 10, Sorensen_convertOutputDisable, 0, 0, NULL, NULL, NULL}, 	// Power in DB L# A# @6 (OUTP:STAT 0 command:  set output disable
   {&DSET_AI, GPIBCVTIO, IB_Q_LOW, NULL, NULL, 0, 10, Sorensen_convertOutputStatus, 0, 0, NULL, NULL, NULL}, 	// Power in DB L# A# @7 (OUTP:STAT?  command:  set output disable
};

/* The following is the number of elements in the command array above.  */
#define NUMPARAMS sizeof(gpibCmds)/sizeof(struct gpibCmd)

/******************************************************************************
 * Initialize device support parameters
 *
 *****************************************************************************/
static long init_ai(int parm)
{
    if (parm==0) {
        devSupParms.name = "devSorensen";
        devSupParms.gpibCmds = gpibCmds;
        devSupParms.numparams = NUMPARAMS;
        devSupParms.timeout = TIMEOUT;
        devSupParms.timeWindow = TIMEWINDOW;
        devSupParms.respond2Writes = -1;

    }
    return(0);
}
