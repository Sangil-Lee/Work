#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include <epicsExport.h>
#include <devSup.h>
#include <recSup.h>
#include <recGbl.h>
#include <dbAccess.h>
#include <callback.h>
#include <aiRecord.h>

#include <wiringPi.h>

typedef struct _DS1820_INFO
{
  CALLBACK callback;

  int pin_num;

  float temper;
  float pre_temper;
}DS1820_INFO;

void ds1820_read(DS1820_INFO *ds1820_info);
int onewire_reset(int pin);
void onewire_write(int pin, uint8_t data);
void onewire_write_bit(int pin, int bit);
uint8_t onewire_read(int pin);
int onewire_read_bit(int pin);
uint8_t crc_read();
uint8_t crc_cal(uint8_t crc, uint8_t data);

static long ai_init_record(aiRecord *pai);
static long read_ai(aiRecord *pai);

static void myCallback(CALLBACK *pcallback)
{
  aiRecord *precord;
  struct rset *prset;

  callbackGetUser(precord, pcallback);
  prset = (struct rset *)(precord->rset);

  dbScanLock((dbCommon*)precord);
  (*prset->process)(precord);
  dbScanUnlock((dbCommon*)precord);
}

static long ai_init_record(aiRecord *pai)
{
  DS1820_INFO *ds1820_info = malloc(sizeof(DS1820_INFO));

  callbackSetCallback(myCallback, &ds1820_info->callback);
  callbackSetPriority(priorityLow, &ds1820_info->callback);
  callbackSetUser(pai, &ds1820_info->callback);

  if(wiringPiSetup() == -1)
    return 1;

  char *para;
  int pin_num = 0;

  para = pai->inp.value.instio.string;

  pin_num = atoi(para);

  ds1820_info->temper = 0.0f;
  ds1820_info->pre_temper = 0.0f;

  ds1820_info->pin_num = pin_num;

  pai->dpvt = ds1820_info;

  return 0;
}

static long read_ai(aiRecord *pai)
{
  DS1820_INFO *ds1820_info = pai->dpvt;

  if(pai->pact)
  {
    ds1820_read(ds1820_info);

    pai->val = ds1820_info->temper;

    pai->udf = FALSE;

    return 2;
  }

  pai->pact = TRUE;
  callbackRequestDelayed(&ds1820_info->callback, pai->disv);

  return 0;
}

struct
{
  long num;
  DEVSUPFUN     report;
  DEVSUPFUN     init;
  DEVSUPFUN     init_record;
  DEVSUPFUN     get_ioint_info;
  DEVSUPFUN     read_ai;
  DEVSUPFUN     special_linconv;
} devAiDS1820Async = {
  6,
  NULL,
  NULL,
  ai_init_record,
  NULL,
  read_ai,
  NULL
};

epicsExportAddress(dset,devAiDS1820Async);

void ds1820_read(DS1820_INFO* ds1820_info)
{
  uint8_t busy = 1;
  int pin = ds1820_info->pin_num;

  onewire_reset(pin);
  onewire_write(pin, 0xCC);
  onewire_write(pin, 0x44);

  delay(750);
  while(busy == 0)
  {
    busy = onewire_read(pin);
    printf("busy: %d\n", busy);
  }

  onewire_reset(pin);
  onewire_write(pin, 0xCC);
  onewire_write(pin, 0xBE);

  uint8_t lsb, msb, th, tl, reserved1, reserved2, count_remain, count_per_c, crc;
  float real_temp = 0.0f;
  float pre_real_temp = ds1820_info->temper;
  signed char temp_read = 0;

  lsb = onewire_read(pin);
  msb = onewire_read(pin);
  th = onewire_read(pin);
  tl = onewire_read(pin);
  reserved1 = onewire_read(pin);
  reserved2 = onewire_read(pin);
  count_remain = onewire_read(pin);
  count_per_c = onewire_read(pin);
  crc = onewire_read(pin);

  uint8_t data[] = {lsb, msb, th, tl, reserved1, reserved2, count_remain, count_per_c};

  onewire_reset(pin);

  if(crc_read(data) == crc)
  {
    temp_read = (signed char)(lsb>>1);

    if(msb == 255)
      temp_read = temp_read | 0x80;

    real_temp = (float)temp_read + 0.85f - (float)count_remain/(float)count_per_c;
    real_temp = (int)(real_temp * 10) / 10.0f;

    ds1820_info->temper = real_temp;
    ds1820_info->pre_temper = real_temp;
  }
  else
    ds1820_info->temper = pre_real_temp;

}

int onewire_reset(int pin)
{
  int result;

  pinMode(pin, OUTPUT);

  digitalWrite(pin, LOW);
  delayMicroseconds(480);

  pinMode(pin, INPUT);
  delayMicroseconds(70);

  result = digitalRead(pin);

  delayMicroseconds(410);

  return result;
}

void onewire_write(int pin, uint8_t data)
{
  int loop;

  for(loop=0; loop<8; loop++)
  {
    onewire_write_bit(pin, data & 0x01);

    data >>= 1;
  }
}

void onewire_write_bit(int pin, int bit)
{
  pinMode(pin, OUTPUT);

  if(bit)
  {
    digitalWrite(pin, LOW);
    delayMicroseconds(6);
    digitalWrite(pin, HIGH);
    delayMicroseconds(64);
  }
  else
  {
    digitalWrite(pin, LOW);
    delayMicroseconds(60);
    digitalWrite(pin, HIGH);
    delayMicroseconds(10);
  }

}

uint8_t onewire_read(int pin)
{
  int loop, result=0;

  for(loop=0; loop<8; loop++)
  {
    result >>= 1;

    if(onewire_read_bit(pin))
      result |= 0x80;
  }

  return result;
}

int onewire_read_bit(int pin)
{
  int result;

  pinMode(pin, OUTPUT);

  digitalWrite(pin, LOW);
  delayMicroseconds(6);

  pinMode(pin, INPUT);
  delayMicroseconds(9);

  result = digitalRead(pin) & 0x01;
  delayMicroseconds(55);

  return result;
}

uint8_t crc_read(uint8_t *data)
{
 uint8_t i, crc;

 crc = 0x00;

 for(i=0; i<8; i++)
  crc = crc_cal(crc, data[i]);

 return crc;
}

uint8_t crc_cal(uint8_t crc, uint8_t data)
{
  int j;
  for(j=0;j<8;j++) {
      if ((data & 0x01 ) ^ (crc & 0x01)) {
          // DATA ^ LSB CRC = 1
          crc = crc>>1;
          // Set the MSB to 1
          crc = crc | 0x80;
          // Check bit 3
          if (crc & 0x04) {
              crc = crc & 0xFB; // Bit 3 is set, so clear it
          } else {
              crc = crc | 0x04; // Bit 3 is clear, so set it
          }
          // Check bit 4
          if (crc & 0x08) {
              crc = crc & 0xF7; // Bit 4 is set, so clear it
          } else {
              crc = crc | 0x08; // Bit 4 is clear, so set it
          }
      } else {
          // DATA ^ LSB CRC = 0
          crc = crc>>1;
          // clear MSB
          crc = crc & 0x7F;
          // No need to check bits, with DATA ^ LSB CRC = 0, they will remain unchanged
      }
      data = data>>1;
  }

  return crc;

}

