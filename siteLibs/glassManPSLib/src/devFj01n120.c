/***************************************************************
* Stream Device record interface for string output records     *
*                                                              *
* (C) 1999 Dirk Zimoch (zimoch@delta.uni-dortmund.de)          *
* (C) 2005 Dirk Zimoch (dirk.zimoch@psi.ch)                    *
*                                                              *
* This is an EPICS record Interface for StreamDevice.          *
* Please refer to the HTML files in ../doc/ for a detailed     *
* documentation.                                               *
*                                                              *
* If you do any changes in this file, you are not allowed to   *
* redistribute it any more. If there is a bug or a missing     *
* feature, send me an email and/or your patch. If I accept     *
* your changes, they will go to the next release.              *
*                                                              *
* DISCLAIMER: If this software breaks something or harms       *
* someone, it's your problem.                                  *
*                                                              *
***************************************************************/
#include <stdio.h>
#include <string.h>

#include "devStream.h"
#include <gmpsRecord.h>
#include <epicsExport.h>

void padding(char inpStr[], long len, long tot_len);

static long readData (dbCommon *record, format_t *format)
{
    gmpsRecord *so = (gmpsRecord *) record;

    if (format->type == DBF_STRING)
    {
	return streamScanfN (record, format, so->val, sizeof(so->val));
    }

    return ERROR;
}

static long writeData (dbCommon *record, format_t *format)
{
    gmpsRecord *pgmps = (gmpsRecord *) record;

    const double Voltage_Max_HEX = 4095.0;
    const double Current_Max_HEX = 4095.0;
    const double HV_Max_Voltage = 1000.0;
    const double HV_Max_Current = 120;

    const long cmd_len = 3;
    const long not_implemented = 6;
    const long chk_len = 2;

    double inpa = 0;
    double inpb = 0;
    long inpc = 0;

    long status;
    status = dbGetLink(&(pgmps->inpa), DBF_DOUBLE, &inpa, 0, 0);
    status = dbGetLink(&(pgmps->inpb), DBF_DOUBLE, &inpb, 0, 0);
    status = dbGetLink(&(pgmps->inpc), DBF_LONG, &inpc, 0, 0);

    double coefV = Voltage_Max_HEX / HV_Max_Voltage;
    double coefC = Current_Max_HEX / HV_Max_Current;

    int inpaHex = (int)(coefV * inpa);
    int inpbHex = (int)(coefC * inpb);

    char inpaStr[cmd_len];
    char inpbStr[cmd_len];
    char inpcStr = '0';

    sprintf(inpaStr, "%X", inpaHex);
    sprintf(inpbStr, "%X", inpbHex);

    long lena = strlen(inpaStr);
    long lenb = strlen(inpbStr);

    padding(inpaStr, lena, cmd_len);
    padding(inpbStr, lenb, cmd_len);
	    
    int inpcASCII = 49;
    if(inpc == 1)
    {
      inpcStr = '2';
      inpcASCII = 50;
    }
    else 
    {
      inpcStr = '1';
      inpcASCII = 49;
    }

    int sum = 0;

    long i;
    for(i=0; i<cmd_len; i++)
      sum += inpaStr[i];


    for(i=0; i<cmd_len; i++)
      sum += inpbStr[i];

    int SASCII = 83;
    int zeroASCII = 48;

    int checkSum = (SASCII + sum + (not_implemented * zeroASCII) + inpcASCII) % 256;
	    
    char checkSumStr[chk_len];
    sprintf(checkSumStr, "%X", checkSum);
	    
    long lenCheckSum = strlen(checkSumStr);
    padding(checkSumStr, lenCheckSum, chk_len);
	     
    long count = 0;
    for(i=0; i<cmd_len; i++)
    {
      pgmps->val[i] = inpaStr[i];
      count++;
    }

    for(i=0; i<cmd_len; i++)
    {
      pgmps->val[count] = inpbStr[i];
      count++;
    }

    for(i=0; i<not_implemented; i++)
    {
      pgmps->val[count] = '0';
      count++;
    }

    pgmps->val[count] = inpcStr;
    count++;

    for(i=0; i<chk_len; i++)
    {
      pgmps->val[count] = checkSumStr[i];
      count++;
    }

    if (format->type == DBF_STRING)
    {
	return streamPrintf (record, format, pgmps->val);
    }
    return ERROR;
}

static long initRecord (dbCommon *record)
{
    gmpsRecord *so = (gmpsRecord *) record;

    return streamInitRecord (record, &so->out, readData, writeData) == ERROR ?
	ERROR : OK;
}

struct {
    long number;
    DEVSUPFUN report;
    DEVSUPFUN init;
    DEVSUPFUN init_record;
    DEVSUPFUN get_ioint_info;
    DEVSUPFUN write;
} devgmpsFj01n120 = {
    5,
    streamReport,
    streamInit,
    initRecord,
    streamGetIointInfo,
    streamWrite
};

epicsExportAddress(dset,devgmpsFj01n120);
