#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <epicsExport.h>
#include <devSup.h>
#include <recSup.h>
#include <recGbl.h>
#include <boRecord.h>
#include <biRecord.h>

#include <wiringPi.h>

static long bo_init_record(boRecord *pbo);
static long bi_init_record(biRecord *pbi);

static long write_bo(boRecord *pbo);
static long read_bi(biRecord *pbi);

float readDHT11(int pin, int mode);
static int act;

struct Pin_Info
{
  int pin_num;
  int pin_mode;
};

#define MAX_TIME 85

int dht11_val[5];
int dht11_val_pre[5];

static long bo_init_record(boRecord *pbo)
{
  struct Pin_Info *pin_info = malloc(sizeof(struct Pin_Info));

  if(wiringPiSetup() == -1)
    return 1;

  int pin_num = 0;
  pin_num = atoi(pbo->out.value.instio.string);

  pinMode(pin_num, OUTPUT);

  pin_info->pin_num = pin_num;

  pbo->dpvt = pin_info;

  return 0;
}

static long bi_init_record(biRecord *pbi)
{
  struct Pin_Info *pin_info = malloc(sizeof(struct Pin_Info));

  if(wiringPiSetup() == -1)
    return 1;

  int pin_num = 0;
  pin_num = atoi(pbi->inp.value.instio.string);

  pinMode(pin_num, INPUT);

  pin_info->pin_num = pin_num;

  pbi->dpvt = pin_info;

  return 0;
}

static long write_bo(boRecord *pbo)
{
  struct Pin_Info *pin_info = pbo->dpvt;

  int pin = pin_info->pin_num;
  int val = pbo->rval;

  digitalWrite(pin, val);

  return 0;
}

static long read_bi(biRecord *pbi)
{
  struct Pin_Info *pin_info = pbi->dpvt;

  int pin = pin_info->pin_num;
  int val = digitalRead(pin);

  pbi->rval = val;

  return 0;
}

struct
{
  long num;
  DEVSUPFUN     report;
  DEVSUPFUN     init;
  DEVSUPFUN     init_record;
  DEVSUPFUN     get_ioint_info;
  DEVSUPFUN     write_bo;
  DEVSUPFUN     special_linconv;
} devBoGpioAsync = {
  6,
  NULL,
  NULL,
  bo_init_record,
  NULL,
  write_bo,
  NULL
};


struct
{
  long num;
  DEVSUPFUN     report;
  DEVSUPFUN     init;
  DEVSUPFUN     init_record;
  DEVSUPFUN     get_ioint_info;
  DEVSUPFUN     read_bi;
  DEVSUPFUN     special_linconv;
} devBiGpioAsync = {
  6,
  NULL,
  NULL,
  bi_init_record,
  NULL,
  read_bi,
  NULL
};

epicsExportAddress(dset,devBoGpioAsync);
epicsExportAddress(dset,devBiGpioAsync);

