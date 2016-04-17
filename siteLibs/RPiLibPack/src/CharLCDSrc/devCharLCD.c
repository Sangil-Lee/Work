#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <epicsExport.h>
#include <devSup.h>
#include <recSup.h>
#include <recGbl.h>
#include <dbAccess.h>
#include <lcdoutRecord.h>

#include <wiringPi.h>
#include <lcd.h>

static long init_record(lcdoutRecord *pstrout);
static long write_lcdout(lcdoutRecord *pstrout);

struct Pin_Info
{
  int handle;
  int bits;
};

static long init_record(lcdoutRecord *pstrout)
{
  struct Pin_Info *pin_info = malloc(sizeof(struct Pin_Info));

  if(wiringPiSetup() == -1)
    return 1;

  int rows = 2;
  int cols = 16;
  int bits = 4;
  int rs = 4;
  int strb = 5;
  int data[8] = {0,1,2,3,0,0,0,0};

  char *para = pstrout->out.value.instio.string;
  rows = atoi(strtok(para, " "));
  cols = atoi(strtok(NULL, " "));
  bits = atoi(strtok(NULL, " "));
  rs = atoi(strtok(NULL, " "));
  strb = atoi(strtok(NULL, " "));

  pinMode(rs, OUTPUT);
  pinMode(strb, OUTPUT);

  int i;
  for(i=0; i<bits; i++)
  {
    data[i] = atoi(strtok(NULL, " "));
    pinMode(data[i], OUTPUT);
  }

  int handle = -1;

  handle = lcdInit(rows,cols,bits,rs,strb,data[0],data[1],data[2],data[3],data[4],data[5],data[6],data[7]);

  pin_info->bits = bits;
  pin_info->handle = handle;

  pstrout->dpvt = pin_info;

  return 0;
}

static long write_lcdout(lcdoutRecord *pstrout)
{
  struct Pin_Info *pin_info = pstrout->dpvt;

  int handle = pin_info->handle;
  int bits = pin_info->bits;

  lcdHome(handle);
  lcdClear(handle);

  int dirs = pstrout->dirs;

  if(dirs == 1)
  {
    char *val = pstrout->val;

    lcdPuts(handle, val);
//    lcdPrintf(handle, "%s", val);
  }
  else
  {
    double inpa = 0.0;
    double inpb = 0.0;
    double inpc = 0.0;
    double inpd = 0.0;

    long status;
    status = dbGetLink(&(pstrout->inpa), DBF_DOUBLE, &inpa, 0, 0);
    status = dbGetLink(&(pstrout->inpb), DBF_DOUBLE, &inpb, 0, 0);
    status = dbGetLink(&(pstrout->inpc), DBF_DOUBLE, &inpc, 0, 0);
    status = dbGetLink(&(pstrout->inpd), DBF_DOUBLE, &inpd, 0, 0);

    char *stra = pstrout->stra;
    char *strb = pstrout->strb;
    char *strc = pstrout->strc;
    char *strd = pstrout->strd;

    char *unia = pstrout->unia;
    char *unib = pstrout->unib;
    char *unic = pstrout->unic;
    char *unid = pstrout->unid;

    lcdPosition(handle, 0,0);
    lcdPrintf(handle, "%s%.2f%s", stra, inpa, unia);

    lcdPosition(handle, 0,1);
    lcdPrintf(handle, "%s%.2f%s", strb, inpb, unib);

    if(bits > 2)
    {
      lcdPosition(handle, 0,2);
      lcdPrintf(handle, "%s%.2f%s", strc, inpc, unic);

      lcdPosition(handle, 0,3);
      lcdPrintf(handle, "%s%.2f%s", strd, inpd, unid);
    }
  }

  return 0;
}

struct
{
  long num;
  DEVSUPFUN     report;
  DEVSUPFUN     init;
  DEVSUPFUN     init_record;
  DEVSUPFUN     get_ioint_info;
  DEVSUPFUN     write_stringout;
  DEVSUPFUN     special_linconv;
} devLcdout = {
  6,
  NULL,
  NULL,
  init_record,
  NULL,
  write_lcdout,
  NULL
};

epicsExportAddress(dset,devLcdout);
