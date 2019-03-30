#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <epicsExport.h>
#include <devSup.h>
#include <recSup.h>
#include <recGbl.h>
#include <dbAccess.h>
#include <callback.h>
#include <shtRecord.h>

#include "RPi_SHT1x.h"

static long init_record(shtRecord *psht);
static long read_sht(shtRecord *psht);

typedef struct Sht_Info
{
  CALLBACK *callback;

  int spin;
  int dpin;

  double temp;
  double humi;

}SHT_INFO;

void readSHT7x(SHT_INFO *sht_info);


static void myCallback(CALLBACK *pcallback)
{
  shtRecord *precord;
  struct rset *prset;

  callbackGetUser(precord, pcallback);
  prset = (struct rset *)(precord->rset);

  dbScanLock((dbCommon*)precord);
  (*prset->process)(precord);
  dbScanUnlock((dbCommon*)precord);
}

static long init_record(shtRecord *psht)
{
  SHT_INFO *sht_info = malloc(sizeof(SHT_INFO));
  CALLBACK *pcallback = (CALLBACK *)(calloc(1, sizeof(CALLBACK)));

  callbackSetCallback(myCallback, pcallback);
  callbackSetPriority(priorityLow, pcallback);
  callbackSetUser(psht, pcallback);

  if(wiringPiSetup() == -1)
    return 1;

  int spin = 0;
  int dpin = 1;

  char *para = psht->inp.value.instio.string;
  spin = atoi(strtok(para, " "));
  dpin = atoi(strtok(NULL, " "));

  sht_info->spin = spin;
  sht_info->dpin = dpin;
  sht_info->callback = pcallback;

  psht->dpvt = sht_info;

  return 0;
}

static long read_sht(shtRecord *psht)
{
  SHT_INFO *sht_info = psht->dpvt;
  CALLBACK *pcallback = (CALLBACK *)sht_info->callback;

  if(psht->pact)
  {
    readSHT7x(sht_info);

    psht->val = sht_info->temp;
    psht->tem = sht_info->temp;
    psht->hum = sht_info->humi;

    psht->udf = FALSE;

    return 2;
  }
  else
  {
    psht->pact = TRUE;
    callbackRequestDelayed(pcallback, 0.5);

    return 0;
  }

}

struct
{
  long num;
  DEVSUPFUN     report;
  DEVSUPFUN     init;
  DEVSUPFUN     init_record;
  DEVSUPFUN     get_ioint_info;
  DEVSUPFUN     read_sht;
  DEVSUPFUN     special_linconv;
} devShtin = {
  6,
  NULL,
  NULL,
  init_record,
  NULL,
  read_sht,
  NULL
};

epicsExportAddress(dset,devShtin);

void readSHT7x(SHT_INFO *sht_info)
{
        unsigned char noError = 1;
        value humi_val,temp_val;

        int spin = sht_info->spin;
        int dpin = sht_info->dpin;

        // Wait at least 11ms after power-up (chapter 3.1)
        delay(20); 

        // Set up the SHT1x Data and Clock Pins
        SHT1x_InitPins(spin, dpin);

        // Reset the SHT1x
        SHT1x_Reset(spin, dpin);

        // Request Temperature measurement
        noError = SHT1x_Measure_Start( spin, dpin ,SHT1xMeaT );
        if (!noError) {
                printf("Measure Start Error1\n");
                return;
                }

        // Read Temperature measurement
        noError = SHT1x_Get_Measure_Value( spin, dpin, (unsigned short int*) &temp_val.i );
        if (!noError) {
                printf("Measure Value Error1\n");
                return;
                }

        // Request Humidity Measurement
        noError = SHT1x_Measure_Start( spin, dpin, SHT1xMeaRh );
        if (!noError) {
                printf("Measure Start Error2\n");
                return;
                }

        // Read Humidity measurement
        noError = SHT1x_Get_Measure_Value( spin, dpin, (unsigned short int*) &humi_val.i );
        if (!noError) {
                printf("Measure Value Error2\n");
                return;
                }

        // Convert intergers to float and calculate true values
        temp_val.f = (float)temp_val.i;
        humi_val.f = (float)humi_val.i;

       // Calculate Temperature and Humidity
        SHT1x_Calc(&humi_val.f, &temp_val.f);

	sht_info->temp = (double)temp_val.f;
	sht_info->humi = (double)humi_val.f;

        //Print the Temperature to the console
//        printf("Temperature: %0.2f%cC  ",temp_val.f,0x00B0);

        //Print the Humidity to the console
//        printf("Humidity: %0.2f%%\n",humi_val.f);
}
