/*
 * LakeShore device support
 */

#include <epicsStdio.h>
#include <devCommonGpib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

//#define DEBUG

/******************************************************************************
 *
 * The following define statements are used to declare the names to be used
 * for the dset tables.   
 *
 * A DSET_AI entry must be declared here and referenced in an application
 * database description file even if the device provides no AI records.
 *
 ******************************************************************************/

#define DSET_BI     devBiLakeShore		// L224 Communication status Monitor	(*STB ? Qyuery)
#define DSET_AI     devAiLakeShore

#include <devGpib.h> /* must be included after DSET defines */

#define TIMEOUT     0.5    /* I/O must complete within this time */
#define TIMEWINDOW  2.0    /* Wait this long after device timeout */

static int
check_validReply(char *buf, int len) 
{
	int	i, nPeriod = 0;
	
	if ((buf[0] != '+') && (buf[0] != '-'))
		return (-1);
	for (i = 1; i < 8; i++) {
		if ((buf[i] >= '0') && (buf[i] <= '9'))
			continue;
		if (buf[i] == '.') {
			nPeriod++;
			continue;
		}
		break;
	}
	if ((i >= 8) && (nPeriod < 2))
		return(0);
	return(-1);
}

#define MAX_SENSOR	50

struct {
	int		cx;			// must be between 1 ~ MAX_SENSOR
	double	rl;
	double	ru;
	double	zl;
	double	zu;
	double	coeffi[10];
} calCurveTable[MAX_SENSOR*3];

static int sensorIndex[MAX_SENSOR];
static int max_sensor = 0;

static int
convertCalCurve(int cx, double rval, double *val)
{
	int	i;
	int	m, j;
	long double	zl, zu, coeffi[10], z, k, K;

	if ((cx < 1) || (cx > max_sensor))
		return -1;

	i = sensorIndex[cx - 1];
	
	if (calCurveTable[i].cx != cx)
			return -1;
			
	j = i + 3;
	for (; i < j; i++) {
		if ((calCurveTable[i].rl <= rval) && (rval <= calCurveTable[i].ru)) {
			zl = calCurveTable[i].zl;
			zu = calCurveTable[i].zu;
			for (m = 0; m < 10; m++)
					coeffi[m] = calCurveTable[i].coeffi[m];

			z = log10(rval);
			k = ((z-zl) - (zu-z))/(zu-zl);
			K = 0;
			for (m = 0; m < 10; m++)
				K += coeffi[m]*cos((long double)m*acos((k)));
	
			*val = K;
			
			return 0;
		}
	}
	
	return -1;
}

static int
getToken(char *cp, char *token)
{
	int	i, n = 0;

	for ( i = 0; i < 20; i++) {
		if (*cp >= '0' && *cp <= '9')
			break;
		if (*cp >= 'A' && *cp <= 'Z')
			break;
		if (*cp >= 'a' && *cp <= 'z')
			break;
		if (*cp == '.' || *cp == '+' || *cp == '-')
			break;

		if (*cp == ',' || *cp == ' ') {
			cp++;
			n++;
		}
		if (*cp == 0 || *cp == 0x0d || *cp == 0x0a)
			return -1;
	}
	for (i = 0; i < 20; i++) {
		if (*cp == ',' || *cp == ' ' || *cp == 0x0d || *cp == 0x0a) {
			token[i] = 0;
			break;
		}
		token[i] = *cp++;
		n++;
	}
	return n;
}

static int
make_sensorIndex()
{
	int	i, n, idx = 0;
	FILE	*fp;
	char	*cp, line[256], token[20];

	for (i = 0; i < MAX_SENSOR; i++)
		sensorIndex[i] = -1;
	for (i = 0; i < MAX_SENSOR*3; i++)
		calCurveTable[i].cx = -1;

	if ((fp = fopen("CxCurveTable", "r")) == NULL) {
		printf("devLakeShore 224: Cannot open CxCurveTable\n");
		return -1;
	}

	if (fgets(line, 256, fp) == NULL)	// read first comment line
		return -1;
	for (i = 0; i < 256; i++) {
		if (line[i] >= '0' && line[i] <= '9')
			continue;
		else
			break;
	}
	line[i] = '\0';
	max_sensor = atoi(line);
	if (max_sensor < 0 || max_sensor >= MAX_SENSOR)
		return -1;

	if (fgets(line, 256, fp) == NULL)	// read first comment line
		return -1;

	while (fgets(line, 256, fp) != NULL) {
		if (strlen(line) < 100)			// remove blank line
			continue;
		cp = line;
		n = getToken(cp, token); calCurveTable[idx].cx = atoi(token+2); cp += n;
		n = getToken(cp, token); cp += n;	// skip serial number
		n = getToken(cp, token); calCurveTable[idx].rl = atof(token); cp += n;
		n = getToken(cp, token); calCurveTable[idx].ru = atof(token); cp += n;
		n = getToken(cp, token); calCurveTable[idx].zl = atof(token); cp += n;
		n = getToken(cp, token); calCurveTable[idx].zu = atof(token); cp += n;
		n = getToken(cp, token); calCurveTable[idx].coeffi[0] = atof(token); cp += n;
		n = getToken(cp, token); calCurveTable[idx].coeffi[1] = atof(token); cp += n;
		n = getToken(cp, token); calCurveTable[idx].coeffi[2] = atof(token); cp += n;
		n = getToken(cp, token); calCurveTable[idx].coeffi[3] = atof(token); cp += n;
		n = getToken(cp, token); calCurveTable[idx].coeffi[4] = atof(token); cp += n;
		n = getToken(cp, token); calCurveTable[idx].coeffi[5] = atof(token); cp += n;
		n = getToken(cp, token); calCurveTable[idx].coeffi[6] = atof(token); cp += n;
		n = getToken(cp, token); calCurveTable[idx].coeffi[7] = atof(token); cp += n;
		n = getToken(cp, token); calCurveTable[idx].coeffi[8] = atof(token); cp += n;
		n = getToken(cp, token); calCurveTable[idx].coeffi[9] = atof(token); cp += n;
		idx++;
		if (idx >= max_sensor*3)
			break;
	}
	fclose(fp);

	for (i = 0; i < max_sensor * 3; i++) {
		n = calCurveTable[i].cx - 1;
		if (n < max_sensor) {
			if (sensorIndex[n] == -1)
				sensorIndex[n] = i;
		}
	}

	return 0;
}

static int
getCX(char *name)
{
	int	i, n;
	char	s[40];

	strncpy(s, name, 40);

	for (i = 0; i < 35; i++) {
		if (s[i] == '\0')
			return -1;
			
		if (s[i] == 'C' && s[i+1] == 'X') {
			if (((s[i+2] >= '0') && (s[i+2] <= '9')) && ((s[i+3] >= '0') && (s[i+3] <= '9')) && ((s[i+4] >= '0') && (s[i+4] <= '9'))) {
				s[5] = '\0';
				n = atoi(s+i+2);
				if (n > 0 && n <= max_sensor)
					return n;
			}
		}
	}
	return -1;
}

static int
temp_PRTResistanceValue(double aTemp, double R0, double *Resistance)
{
// The function temp_PRTResistanceValue calculates the electrical
// resistance for a Platinum Resistance Thermometer (PRT) at given
// temperature and defined resistance R0 at 0��C for a PRT element.
// E.g. for a Pt1000 element resistance R0=1000. Function returns
// TRUE if aTemp is within the ranges polynomial approximation
// are defined, the respective resistance for the temperature aTemp
// is found in Resistance.
//----------------------------------------------------------------
// Units:
//  aTemp        Input temperature in ITS-90                   ��C
//  R0           Type of PRT (resistance at 0��C; e.g. Pt100)   Ohm
//  Resistance   Returned resistance of R0 type PRT at aTemp    Ohm
//----------------------------------------------------------------

  if ((aTemp >= -200) && (aTemp <= 850)) {
    // aTemp decides which polynomial to use
    if (aTemp >= 0)
	*Resistance = R0 * (( -5.775e-7 * aTemp + 3.9083e-3) * aTemp + 1);
    else
	*Resistance = R0 * ((((aTemp - 100) * - 4.183e-12 * aTemp - 5.775e-7) * aTemp + 3.9083e-3) * aTemp + 1);
	  return 0;
  } else
	return -1;
}

static int
convertPt100Curve(double aResistance, double R0, double *Temperature)
{
// The function temp_PRTTemperatureValue calculates the temperature
// measured by a Platinum Resistance Thermometer (PRT) at given
// electrical resistance and defined resistance R0 at 0��C for a
// PRT element. E.g. for a Pt1000 element resistance R0=1000.
// Function returns TRUE if aResistance is within the ranges
// polynomial approximation are defined, the respective temperature
// for resistance aResistance is found in Temperature.
//----------------------------------------------------------------
// Units:
//  aResistance  Input electrical resistance                   Ohm
//  R0           Type of PRT (electrical resistance at 0��C)    Ohm
//  Temperature  Returned ITS-90 Temperature                   ��C
//----------------------------------------------------------------

	double	MinResistance, MaxResistance;


	*Temperature = 0;

  // find min and max resistance values for defined range
	if (temp_PRTResistanceValue(-200, R0, &MinResistance) || 
        temp_PRTResistanceValue(+850, R0, &MaxResistance))
		return -1;

 
	if ((aResistance >= MinResistance) && (aResistance <= MaxResistance)) {
      // then calculate temperature
		*Temperature = (-R0 * 3.9083e-3 +
                      sqrt(R0 * R0 * 3.9083e-3 * 3.9083e-3 - 4 * R0 * -5.775e-7 * (R0 - aResistance))) /
                      (2 * R0 * -5.775e-7);
	}
	return 0;
}

static int
LakeShore_convertDataReplyBI(struct gpibDpvt *pdpvt, int P1, int P2, char **P3)
{
	struct biRecord *pbi = ((struct biRecord *)(pdpvt->precord));

	asynOctet *pasynOctet = pdpvt->pasynOctet;
	void *asynOctetPvt = pdpvt->asynOctetPvt;

	int	len;
	int	val;
	char cmd[64] = {0,};
	char cbuf[64] = {0,};
	
	strcpy(cmd, "*TST?\r\n");
	
	if((pasynOctet->setInputEos(asynOctetPvt,pdpvt->pasynUser,NULL,0) != asynSuccess))
		return -1;

	if((pasynOctet->setOutputEos(asynOctetPvt,pdpvt->pasynUser,NULL,0) != asynSuccess))
		return -1;
//printf("cmd = %s\n", cmd);
	if(pasynOctet->write(asynOctetPvt,pdpvt->pasynUser,cmd,strlen(cmd),(size_t *) &len) != asynSuccess)
		return -1;

	pasynOctet->read(asynOctetPvt,pdpvt->pasynUser,cbuf, sizeof(cbuf),(size_t *) &len, NULL);

	if (len > 1)
		val = 0;	// normal 
	else
		val = 1;	// abnormal
	pbi->rval = val;

	return 0;
}

static int
LakeShore_convertDataReplyAI(struct gpibDpvt *pdpvt, int P1, int P2, char **P3)
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
		case 1:
			strcpy(cmd, "KRDG? A\r\n");
			break;
		case 2:
			strcpy(cmd, "KRDG? B\r\n");
			break;
		case 3:
		case 4:		
		case 5:		
		case 6:		
		case 7:
			strcpy(cmd, "KRDG? C1\r\n");
			cmd[7] = 0x30 + addr - 2;
			break;
		case 8:
		case 9:		
		case 10:		
		case 11:		
		case 12:
			strcpy(cmd, "KRDG? D1\r\n");
			cmd[7] = 0x30 + addr - 7;
			break;					
		default:
			strcpy(cmd, "KRDG? A\r\n");
			break;
	}
#ifdef DEBUG
	printf("FUNC::LakeShore_convertDataReplyAI()\n");
	printf("    cmd:(%s)\n", cmd);  
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
		pai->scan = 10.0;	/* increase scan period	*/
		return -1;	
	} else
		pai->scan = 1.0;	/* increase scan period	*/

	if (len < 8){	/* invalid length	*/
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
	printf("CX (%s), cbuf string:(%s) rval:%f val = %g\n", pai->name, cbuf, rval, pai->val);  
#endif
	
	return 0;
}

static int
LakeShore_convertDataReplyCurveAI(struct gpibDpvt *pdpvt, int P1, int P2, char **P3)
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
		case 1:
			strcpy(cmd, "SRDG? A\r\n");
			break;
		case 2:
			strcpy(cmd, "SRDG? B\r\n");
			break;
		case 3:
		case 4:		
		case 5:		
		case 6:		
		case 7:
			strcpy(cmd, "SRDG? C1\r\n");
			cmd[7] = 0x30 + addr - 2;
			break;
		case 8:
		case 9:		
		case 10:		
		case 11:		
		case 12:
			strcpy(cmd, "SRDG? D1\r\n");
			cmd[7] = 0x30 + addr - 7;
			break;					
		default:
			strcpy(cmd, "SRDG? A\r\n");
			break;
	}
#ifdef DEBUG
	printf("FUNC::LakeShore_convertDataReplyCurveAI()\n");
	printf("    cmd:(%s)\n", cmd);  
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
		pai->scan = 10.0;	/* increase scan period	*/
		return -1;	
	} else
		pai->scan = 1.0;	/* increase scan period	*/

	if (len < 8){	/* invalid length	*/
		epicsSnprintf(pdpvt->pasynUser->errorMessage, pdpvt->pasynUser->errorMessageSize, "Invalid reply %d", pdpvt->msgInputLen);
		return -1;	
	}
	
//	if (check_validReply(cbuf, len) < 0) { /* invalid reply data	*/
//			epicsSnprintf(pdpvt->pasynUser->errorMessage, pdpvt->pasynUser->errorMessageSize, "Invalid reply %d", pdpvt->msgInputLen);
//			return -1;
//	}

	double val = 0, rval;
	int	cx;

	rval = atof(cbuf);
	pai->rval = rval;

	if ((cx = getCX(pai->name)) < -1) {
		printf("Invalid CX sensor name (must include CX### (%s)\n", pai->name);
		return -1;
	}
  
	if (convertCalCurve(cx, rval, &val) < 0)		{
		printf("Conversion Error: CX(%d) rval:%f val = %g\n", cx, rval, val);
		return -1;
	}
printf("CX (%s), CX=%d, cbuf string:(%s) rval:%f val = %g\n", pai->name, cx, cbuf, rval, val);

	pai->val = val;
	pai->udf = 0;
	
	return 0;
}

static int
LakeShore_convertDataReplyPt100AI(struct gpibDpvt *pdpvt, int P1, int P2, char **P3)
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
		case 1:
			strcpy(cmd, "SRDG? A\r\n");
			break;
		case 2:
			strcpy(cmd, "SRDG? B\r\n");
			break;
		case 3:
		case 4:		
		case 5:		
		case 6:		
		case 7:
			strcpy(cmd, "SRDG? C1\r\n");
			cmd[7] = 0x30 + addr - 2;
			break;
		case 8:
		case 9:		
		case 10:		
		case 11:		
		case 12:
			strcpy(cmd, "SRDG? D1\r\n");
			cmd[7] = 0x30 + addr - 7;
			break;					
		default:
			strcpy(cmd, "SRDG? A\r\n");
			break;
	}

	if((pasynOctet->setInputEos(asynOctetPvt,pdpvt->pasynUser,NULL,0) != asynSuccess))
		return -1;
	if((pasynOctet->setOutputEos(asynOctetPvt,pdpvt->pasynUser,NULL,0) != asynSuccess))
		return -1;
//printf("cmd = %s\n", cmd);

	if(pasynOctet->write(asynOctetPvt,pdpvt->pasynUser,cmd,strlen(cmd),(size_t *) &len) != asynSuccess)
		return -1;

	pasynOctet->read(asynOctetPvt,pdpvt->pasynUser,cbuf, sizeof(cbuf),(size_t *) &len, NULL);
//printf("cbuf = %s\n", cbuf);

	if (len == 0){	/* no response: timeout	*/
		epicsSnprintf(pdpvt->pasynUser->errorMessage, pdpvt->pasynUser->errorMessageSize, "No Response");
		pai->scan = 10.0;	/* increase scan period	*/
		return -1;	
	} else
		pai->scan = 1.0;	/* increase scan period	*/

	if (len < 8){	/* invalid length	*/
		epicsSnprintf(pdpvt->pasynUser->errorMessage, pdpvt->pasynUser->errorMessageSize, "Invalid reply %d", pdpvt->msgInputLen);
		return -1;	
	}
	
	// if (check_validReply(cbuf, len) < 0) { /* invalid reply data	*/
	//	epicsSnprintf(pdpvt->pasynUser->errorMessage, pdpvt->pasynUser->errorMessageSize, "Invalid reply %d", pdpvt->msgInputLen);
	//	return -1;
	// }

	double val = 0, rval;

	rval = atof(cbuf);
	pai->rval = rval;
  
	if (convertPt100Curve(rval, 100, &val) < 0)		{
		printf("Conversion Error: rval:%f val = %g\n", rval, val);
		return -1;
	}
  
//	pai->val = val;

	pai->val = val + 273.15;
	pai->udf = 0;
	
//printf("PT-100(%s) cbuf string:(%s) rval:%f val = %g\n", pai->name, cbuf, rval, pai->val);
	
	return 0;
}

/******************************************************************************
 * Array of structures that define all GPIB messages
 * supported for this type of instrument.
 ******************************************************************************/

static struct gpibCmd gpibCmds[] = {
    /* Param 0 -- Get Data */
/* {&DSET_SI, GPIBREAD, IB_Q_LOW, "#RD\r", NULL, 0, 40, GP_convertDataReplyString, 0, 0, NULL, NULL, "\r"}*/
   {&DSET_BI, GPIBCVTIO, IB_Q_LOW, NULL, NULL, 0, 10, LakeShore_convertDataReplyBI, 0, 0, NULL, NULL, NULL},			// L224 status check in DB L# A# @0
   {&DSET_AI, GPIBCVTIO, IB_Q_LOW, NULL, NULL, 0, 10, LakeShore_convertDataReplyPt100AI, 0, 0, NULL, NULL, NULL},	// PT Sensor in DB L# A# @1
   {&DSET_AI, GPIBCVTIO, IB_Q_LOW, NULL, NULL, 0, 10, LakeShore_convertDataReplyAI, 0, 0, NULL, NULL, NULL},     	// CX Sensor in DB L# A# @2 (KRDG? command: get from TM monitor
   {&DSET_AI, GPIBCVTIO, IB_Q_LOW, NULL, NULL, 0, 10, LakeShore_convertDataReplyCurveAI, 0, 0, NULL, NULL, NULL} 	// CX Sensor in DB L# A# @3 (SRDG? command: get from curve table
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
        devSupParms.name = "devLakeShore";
        devSupParms.gpibCmds = gpibCmds;
        devSupParms.numparams = NUMPARAMS;
        devSupParms.timeout = TIMEOUT;
        devSupParms.timeWindow = TIMEWINDOW;
        devSupParms.respond2Writes = -1;

		make_sensorIndex();	/* calulate sensor index table 	*/
    }
    return(0);
}
