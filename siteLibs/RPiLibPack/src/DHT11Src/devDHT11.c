#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <epicsExport.h>
#include <devSup.h>
#include <recSup.h>
#include <recGbl.h>
#include <dbAccess.h>
#include <callback.h>
#include <dhtRecord.h>

#include <wiringPi.h>

#define MAX_TIME 85

static long init_record(dhtRecord *pdht);
static long read_dht(dhtRecord *pdht);

typedef struct Dht_Info
{
  CALLBACK *callback;

  int pin;

  double temp;
  double humi;

  double pre_temp;
  double pre_humi;

}DHT_INFO;

void readDHT(DHT_INFO *dht_info);

int dht11_val[5];

static void myCallback(CALLBACK *pcallback)
{
  dhtRecord *precord;
  struct rset *prset;

  callbackGetUser(precord, pcallback);
  prset = (struct rset *)(precord->rset);

  dbScanLock((dbCommon*)precord);
  (*prset->process)(precord);
  dbScanUnlock((dbCommon*)precord);
}

static long init_record(dhtRecord *pdht)
{
  DHT_INFO *dht_info = malloc(sizeof(DHT_INFO));
  CALLBACK *pcallback = (CALLBACK *)(calloc(1, sizeof(CALLBACK)));

  callbackSetCallback(myCallback, pcallback);
  callbackSetPriority(priorityLow, pcallback);
  callbackSetUser(pdht, pcallback);

  if(wiringPiSetup() == -1)
    return 1;

  int i;
  for(i=0;i<5;i++)
    dht11_val[i] = 0;

  int pin = atoi(pdht->inp.value.instio.string);

  dht_info->pin = pin;
  dht_info->callback = pcallback;

  pdht->dpvt = dht_info;

  return 0;
}

static long read_dht(dhtRecord *pdht)
{
  DHT_INFO *dht_info = pdht->dpvt;
  CALLBACK *pcallback = (CALLBACK *)dht_info->callback;

  if(pdht->pact)
  {
    readDHT(dht_info);

    pdht->val = dht_info->temp;
    pdht->tem = dht_info->temp;
    pdht->hum = dht_info->humi;

    pdht->udf = FALSE;

    return 2;
  }
  else
  {
    pdht->pact = TRUE;
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
  DEVSUPFUN     read_dht;
  DEVSUPFUN     special_linconv;
} devDhtAsyn = {
  6,
  NULL,
  NULL,
  init_record,
  NULL,
  read_dht,
  NULL
};

epicsExportAddress(dset,devDhtAsyn);

void readDHT(DHT_INFO *dht_info)
{
  int pin = dht_info->pin;

  int i=0;
  for(i=0;i<5;i++)
      dht11_val[i]=0;

  epicsUInt8 lststate=HIGH;
  epicsUInt8 counter=0;

  pinMode(pin,OUTPUT);
  digitalWrite(pin,LOW);
  delay(18);
  digitalWrite(pin,HIGH);
  delayMicroseconds(40);
  pinMode(pin,INPUT);

  int j=0;
  for(i=0;i<MAX_TIME;i++)
  {
    counter=0;
    while(digitalRead(pin)==lststate)
    {
      counter++;
      delayMicroseconds(1);
      if(counter==255)
        break;
    }

    lststate=digitalRead(pin);
    if(counter==255)
       break;
    // top 3 transistions are ignored
    if((i>=4)&&(i%2==0))
    {
      dht11_val[j/8]<<=1;
      if(counter>16)
        dht11_val[j/8]|=1;
      j++;
    }
  }

  float val = 0.0f;
  char tmp[10];

  if((j>=40)&&(dht11_val[4]==((dht11_val[0]+dht11_val[1]+dht11_val[2]+dht11_val[3])& 0xFF)))
  {
    sprintf(tmp, "%d.%d", dht11_val[0], dht11_val[1]);
    val = atof(tmp);

    dht_info->humi = val;
    dht_info->pre_humi = val;

    sprintf(tmp, "%d.%d", dht11_val[2], dht11_val[3]);
    val = atof(tmp);

    dht_info->temp = val;
    dht_info->pre_temp = val;
  }
  else
  {
      dht_info->humi = dht_info->pre_humi;
      dht_info->temp = dht_info->pre_temp;
  }
}

