#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <epicsExport.h>
#include <devSup.h>
#include <recSup.h>
#include <recGbl.h>
#include <dbAccess.h>
#include <callback.h>
#include <bmeRecord.h>

#include <wiringPi.h>
#include <wiringPiI2C.h>

#define DEV_ADDR 0x76

int digT[3];
int digP[9];
int digH[6];
double t_fine;

void writeReg(int fd, int reg_address, int data);
void reset(int fd);
void setup(int fd);
void getCalbParm(int fd);

double compensate_T(int adc_T);
double compensate_P(int adc_P);
double compensate_H(int adc_H);


static long init_record(bmeRecord *pbme);
static long read_bme(bmeRecord *pbme);

typedef struct Bme_Info
{
  CALLBACK *callback;

  int fd;

  double temp;
  double humi;
  double pres;

}BME_INFO;

void readBME(BME_INFO *bme_info);

static void myCallback(CALLBACK *pcallback)
{
  bmeRecord *precord;
  struct rset *prset;

  callbackGetUser(precord, pcallback);
  prset = (struct rset *)(precord->rset);

  dbScanLock((dbCommon*)precord);
  (*prset->process)(precord);
  dbScanUnlock((dbCommon*)precord);
}

static long init_record(bmeRecord *pbme)
{
  BME_INFO *bme_info = malloc(sizeof(BME_INFO));
  CALLBACK *pcallback = (CALLBACK *)(calloc(1, sizeof(CALLBACK)));

  callbackSetCallback(myCallback, pcallback);
  callbackSetPriority(priorityLow, pcallback);
  callbackSetUser(pbme, pcallback);

  if(wiringPiSetup() == -1)
  {
    printf("Unable to start wiringPi\n");
    return 1;
  }

  int fd = wiringPiI2CSetup(DEV_ADDR);

  if(fd == -1)
  {
    printf("Can't open i2c device\n");
    return 1;
  }

  reset(fd);
  setup(fd);
  getCalbParm(fd);

  bme_info->fd = fd;
  bme_info->callback = pcallback;

  pbme->dpvt = bme_info;

  return 0;
}

static long read_bme(bmeRecord *pbme)
{
  BME_INFO *bme_info = pbme->dpvt;
  CALLBACK *pcallback = (CALLBACK *)bme_info->callback;

  if(pbme->pact)
  {
    readBME(bme_info);

    pbme->val = bme_info->temp;
    pbme->tem = bme_info->temp;
    pbme->hum = bme_info->humi;
    pbme->pre = bme_info->pres;

    pbme->udf = FALSE;

    return 2;
  }
  else
  {
    pbme->pact = TRUE;
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
  DEVSUPFUN     read_bme;
  DEVSUPFUN     special_linconv;
} devBmeAsyn = {
  6,
  NULL,
  NULL,
  init_record,
  NULL,
  read_bme,
  NULL
};

epicsExportAddress(dset,devBmeAsyn);

void writeReg(int fd, int reg_address, int data)
{
  wiringPiI2CWriteReg8(fd, reg_address, data);
}

void reset(int fd)
{
  writeReg(fd, 0xE0, 0xB6);
}

void setup(int fd)
{
  unsigned char osrs_t = 1;
  unsigned char osrs_p = 1;                      //Pressure oversampling x 1
  unsigned char osrs_h = 1;                      //Humidity oversampling x 1
  unsigned char mode   = 3;                      //Normal mode
  unsigned char t_sb   = 5;                      //Tstandby 1000ms
  unsigned char filter = 0;                      //Filter off
  unsigned char spi3w_en = 0;                    //3-wire SPI Disable

  unsigned char ctrl_meas_reg = (osrs_t << 5) | (osrs_p << 2) | mode;
  unsigned char config_reg    = (t_sb << 5) | (filter << 2) | spi3w_en;
  unsigned char ctrl_hum_reg  = osrs_h;

  writeReg(fd, 0xF2,ctrl_hum_reg);
  writeReg(fd, 0xF4,ctrl_meas_reg);
  writeReg(fd, 0xF5,config_reg);
}

void getCalbParm(int fd)
{
  int calib[32];
  int reg = 0x88;

  int i;
  for(i = 0; i < 24; i++)
    calib[i] = wiringPiI2CReadReg8(fd, reg+i);

  calib[24] = wiringPiI2CReadReg8(fd, 0xA1);

  for(i = 0; i < 7; i++)
    calib[i+25] = wiringPiI2CReadReg8(fd, 0xE1+i);

  digT[0] = calib[1] << 8 | calib[0];
  digT[1] = calib[3] << 8 | calib[2];
  digT[2] = calib[5] << 8 | calib[4];
  digP[0] = calib[7] << 8 | calib[6];
  digP[1] = calib[9] << 8 | calib[8];
  digP[2] = calib[11] << 8 | calib[10];
  digP[3] = calib[13] << 8 | calib[12];
  digP[4] = calib[15] << 8 | calib[14];
  digP[5] = calib[17] << 8 | calib[16];
  digP[6] = calib[19] << 8 | calib[18];
  digP[7] = calib[21] << 8 | calib[20];
  digP[8] = calib[23] << 8 | calib[22];
  digH[0] = calib[24];
  digH[1] = calib[26] << 8 | calib[25];
  digH[2] = calib[27];
  digH[3] = calib[28] << 4 | (0x0F & calib[29]);
  digH[4] = calib[30] << 4 | ((calib[29] >> 4) & 0x0F);
  digH[5] = calib[31];

  for(i = 1; i < 2; i++)
    if(digT[i] & 0x8000) digT[i] = (-digT[i] ^ 0xFFFF) + 1;

  for(i = 1; i < 8; i++)
    if(digP[i] & 0x8000) digP[i] = (-digP[i] ^ 0xFFFF) + 1;

  for(i = 0; i < 6; i++)
    if(digH[i] & 0x8000) digH[i] = (-digH[i] ^ 0xFFFF) + 1;
}

void readBME(BME_INFO *bme_info)
{
  int fd = bme_info->fd;

  int data[8];
  int reg = 0xF7;

  double temp;
  double humi;
  double pres;

  int i;
  for(i = 0; i < 8; i++)
    data[i] = wiringPiI2CReadReg8(fd, reg+i);

  int pres_raw = data[0] << 12 | data[1] << 4 | data[2] >> 4;
  int temp_raw = data[3] << 12 | data[4] << 4 | data[5] >> 4;
  int hum_raw = data[6] << 8 | data[7];

  temp = compensate_T(temp_raw);
  humi = compensate_H(hum_raw);
  pres = compensate_P(pres_raw);

  bme_info->temp = temp;
  bme_info->humi = humi;
  bme_info->pres = pres;
}

double compensate_P(int adc_P)
{
  double pressure = 0.0;
  double var1, var2;

  var1 = (t_fine / 2.0) - 64000.0;
  var2 = (((var1 / 4.0) * (var1 / 4.0)) / 2048) * digP[5];
  var2 = var2 + ((var1 * digP[4]) * 2.0);
  var2 = (var2 / 4.0) + (digP[3] * 65536.0);
  var1 = (((digP[2] * (((var1 / 4.0) * (var1 / 4.0)) / 8192)) / 8) + ((digP[1] * var1) / 2.0)) / 262144;
  var1 = (32768 + var1) * digP[0] / 32768;

  if(var1 == 0) return 9876.0;

  pressure = ((1048576 - adc_P) - (var2 / 4096)) * 3125;

  if(pressure < 0x80000000) pressure = (pressure * 2.0) / var1;
  else
    pressure = (pressure / var1) * 2.0;

  var1 = (digP[8] * (((pressure / 8.0) * (pressure / 8.0)) / 8192.0)) / 4096;
  var2 = ((pressure / 4.0) * digP[7]) / 8192.0;
  pressure = pressure + ((var1 + var2 + digP[6]) / 16.0);

  return pressure / 100;
//  printf("pres : %.2fhPa\n", pressure/100);

}


double compensate_T(int adc_T)
{
  double var1, var2, T;
  var1 = (adc_T / 16384.0 - digT[0] / 1024.0) * digT[1];
  var2 = (adc_T / 131072.0 - digT[0] / 8192.0) * (adc_T / 131072.0 - digT[0] / 8192.0) * digT[2];
  t_fine = var1 + var2;
  T = t_fine / 5120.0;

  return T;
//  printf("temp : %.2f℃\n", T);
}

double compensate_H(int adc_H)
{
  double var_h;

  var_h = t_fine - 76800.0;

  if(var_h != 0)
    var_h = (adc_H - (digH[3] * 64.0 + digH[4]/16384.0 * var_h)) * (digH[1] / 65536.0 * (1.0 + digH[5] / 67108864.0 * var_h * (1.0 + digH[2] / 67108864.0 * var_h)));
  else return 9876.0;

  var_h = var_h * (1.0 - digH[0] * var_h / 524288.0);

  if(var_h > 100.0) var_h = 100.0;
  else if(var_h < 0.0) var_h = 0.0;

  return var_h;
//  printf("hum : %.2f％\n", var_h);
}

