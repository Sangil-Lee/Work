//Wrapping class for system calls of timing device driver.
#include "timingDriver.h"
#include <stdlib.h>
#include <termios.h>


#define NUM_REGS_MAX 350
//#define D_TestMode_AXICLOCK

namespace timing{

//==============================================================================
//----===@
//==============================================================================
  static char evSrcName[EV_SRC_MAX][20] =
  {
    {"USER_TRIGGER"},
    {"MXC_00"},    {"MXC_01"},    {"MXC_02"},    {"MXC_03"},
    {"MXC_04"},    {"MXC_05"},    {"MXC_06"},    {"MXC_07"},
    {"MXC_08"},    {"MXC_09"},    {"MXC_10"},    {"MXC_11"},
    {"MXC_12"},    {"MXC_13"},
    {"EXT_IN_SL_00"},    {"EXT_IN_SL_01"},    {"EXT_IN_SL_02"},    {"EXT_IN_SL_03"},
    {"EXT_IN_SL_04"},    {"EXT_IN_SL_05"},    {"EXT_IN_SL_06"},    {"EXT_IN_SL_07"},
    {"EXT_IN_SL_08"},    {"EXT_IN_SL_09"},    {"EXT_IN_SL_10"},    {"EXT_IN_SL_11"},
    {"EXT_IN_SL_12"},    {"EXT_IN_SL_13"},    {"EXT_IN_SL_14"},    {"EXT_IN_SL_15"},
    {"EXT_IN_SH_00"},    {"EXT_IN_SH_01"},    {"EXT_IN_SH_02"},    {"EXT_IN_SH_03"},
    {"EXT_IN_SH_04"},    {"EXT_IN_SH_05"},    {"EXT_IN_SH_06"},    {"EXT_IN_SH_07"},
    {"EXT_IN_SH_08"},    {"EXT_IN_SH_09"},    {"EXT_IN_SH_10"},    {"EXT_IN_SH_11"},
    {"EXT_IN_SH_12"},    {"EXT_IN_SH_13"},    {"EXT_IN_SH_14"},    {"EXT_IN_SH_15"}
  };

  static char evOutSrcName[EV_OUT_SRC_MAX][20] =
  {
   {"evrOut_1pps"},
   {"RXDBUS_00"},   {"RXDBUS_01"},   {"RXDBUS_02"},   {"RXDBUS_03"},   
   {"RXDBUS_04"},   {"RXDBUS_05"},   {"RXDBUS_06"},   {"RXDBUS_07"},
   {"PDP_OUT_00"},   {"PDP_OUT_01"},   {"PDP_OUT_02"},   {"PDP_OUT_03"},
   {"PDP_OUT_04"},   {"PDP_OUT_05"},   {"PDP_OUT_06"},   {"PDP_OUT_07"},
   {"PDP_OUT_08"},   {"PDP_OUT_09"},   {"PDP_OUT_10"},   {"PDP_OUT_11"},
   {"PDP_OUT_12"},   {"PDP_OUT_13"},   {"PDP_OUT_14"},   {"PDP_OUT_15"},
   {"PDP_OUT_16"},   {"PDP_OUT_17"},   {"PDP_OUT_18"},   {"PDP_OUT_19"},
   {"PDP_OUT_20"},   {"PDP_OUT_21"},   {"PDP_OUT_22"},   {"PDP_OUT_23"},
   {"PDP_OUT_24"},   {"PDP_OUT_25"},   {"PDP_OUT_26"},   {"PDP_OUT_27"},
   {"PDP_OUT_28"},   {"PDP_OUT_29"},   {"PDP_OUT_30"},   {"PDP_OUT_31"} 
  };

  static char drvName[IIC_maxDrv][IIC_maxDrvNameL] =
  {
    {DRV_NAME_iic_bp},
    {DRV_NAME_iic_ob},
    {DRV_NAME_iic_sl},
    {DRV_NAME_iic_sh}
  };
  /* Back plane IIC Slave Information */
  static s_ts2iic iicSlvs_bp[IIC_maxSlv_bp] = 
  {
    {SLV_MAX7313_A, 0x18, 0, 0, 0}        // MAX7313(0x18)
  };

  /* on board IIC Slave Information */
  static s_ts2iic iicSlvs_ob[IIC_maxSlv_ob] = 
  {
    {SLV_SI5338A, 0x70, 0,           0, 0}, 
    {SLV_SI570  , 0x5D, 1, IIC_MUX_ADDR, 1},
    {SLV_ADN4604, 0x4B, 1, IIC_MUX_ADDR, 8},
    {SLV_INA226A, 0x40, 1, IIC_MUX_ADDR, 4}
  };

  /* slave board Low floor IIC Slave Information */
  static s_ts2iic iicSlvs_sl[IIC_maxSlv_sl] = 
  {
    {SLV_MAX7313_A, 0x18, 1, IIC_MUX_ADDR ,1},  
    {SLV_MAX7313_B, 0x19, 1, IIC_MUX_ADDR ,2} ,
    {SLV_TMP112A,   0x48, 1, IIC_MUX_ADDR ,4} ,
    {SLV_ADN4604,   0x4B, 1, IIC_MUX_ADDR ,8}
  };
  
  /* slave board High floor IIC Slave Information */
  static s_ts2iic iicSlvs_sh[IIC_maxSlv_sh] = 
  {
    {SLV_MAX7313_A, 0x18, 1, IIC_MUX_ADDR ,1},  
    {SLV_MAX7313_B, 0x19, 1, IIC_MUX_ADDR ,2} ,
    {SLV_TMP112A,   0x48, 1, IIC_MUX_ADDR ,4} ,
    {SLV_ADN4604,   0x4B, 1, IIC_MUX_ADDR ,8}
  };

  /* System information */
  static s_ts2iic* piicBus[IIC_maxDrv] = 
  {
    (s_ts2iic*) &iicSlvs_bp,
    (s_ts2iic*) &iicSlvs_ob,
    (s_ts2iic*) &iicSlvs_sl,
    (s_ts2iic*) &iicSlvs_sh
  };

  static int iicSlvNum[IIC_maxDrv] = 
  {
    IIC_maxSlv_bp,
    IIC_maxSlv_ob,
    IIC_maxSlv_sl,
    IIC_maxSlv_sh
  };

//==============================================================================
//----===@
//==============================================================================

#define NUM_REGS_MAX 350

  typedef struct Reg_Data{
    unsigned char Reg_Addr;
    unsigned char Reg_Val;
    unsigned char Reg_Mask;
  } Reg_Data;

  static Reg_Data Reg_ob25MHz[NUM_REGS_MAX] = {
    {  0,0x00,0x00}, {  1,0x00,0x00}, {  2,0x00,0x00}, {  3,0x00,0x00}, {  4,0x00,0x00}, {  5,0x00,0x00},   {  6,0x08,0x1D}, {  7,0x00,0x00}, {  8,0x70,0x00}, {  9,0x0F,0x00}, { 10,0x00,0x00}, { 11,0x00,0x00},
    { 12,0x00,0x00}, { 13,0x00,0x00}, { 14,0x00,0x00}, { 15,0x00,0x00}, { 16,0x00,0x00}, { 17,0x00,0x00},   { 18,0x00,0x00}, { 19,0x00,0x00}, { 20,0x00,0x00}, { 21,0x00,0x00}, { 22,0x00,0x00}, { 23,0x00,0x00},
    { 24,0x00,0x00}, { 25,0x00,0x00}, { 26,0x00,0x00}, { 27,0x70,0x80}, { 28,0x0B,0xFF}, { 29,0x08,0xFF},   { 30,0xB0,0xFF}, { 31,0xC0,0xFF}, { 32,0xC0,0xFF}, { 33,0xC0,0xFF}, { 34,0xC0,0xFF}, { 35,0xAA,0xFF},
    { 36,0x06,0x1F}, { 37,0x06,0x1F}, { 38,0x06,0x1F}, { 39,0x06,0x1F}, { 40,0x84,0xFF}, { 41,0x10,0x7F},   { 42,0x24,0x3F}, { 43,0x00,0x00}, { 44,0x00,0x00}, { 45,0x00,0xFF}, { 46,0x00,0xFF}, { 47,0x14,0x3F},
    { 48,0x3D,0xFF}, { 49,0x00,0xFF}, { 50,0xC4,0xFF}, { 51,0x07,0xFF}, { 52,0x10,0xFF}, { 53,0x00,0xFF},   { 54,0x0D,0xFF}, { 55,0x00,0xFF}, { 56,0x00,0xFF}, { 57,0x00,0xFF}, { 58,0x00,0xFF}, { 59,0x01,0xFF},
    { 60,0x00,0xFF}, { 61,0x00,0xFF}, { 62,0x00,0x3F}, { 63,0x10,0xFF}, { 64,0x00,0xFF}, { 65,0x0D,0xFF},   { 66,0x00,0xFF}, { 67,0x00,0xFF}, { 68,0x00,0xFF}, { 69,0x00,0xFF}, { 70,0x01,0xFF}, { 71,0x00,0xFF},
    { 72,0x00,0xFF}, { 73,0x00,0x3F}, { 74,0x10,0xFF}, { 75,0x00,0xFF}, { 76,0x0D,0xFF}, { 77,0x00,0xFF},   { 78,0x00,0xFF}, { 79,0x00,0xFF}, { 80,0x00,0xFF}, { 81,0x01,0xFF}, { 82,0x00,0xFF}, { 83,0x00,0xFF},
    { 84,0x00,0x3F}, { 85,0x10,0xFF}, { 86,0x00,0xFF}, { 87,0x0D,0xFF}, { 88,0x00,0xFF}, { 89,0x00,0xFF},   { 90,0x00,0xFF}, { 91,0x00,0xFF}, { 92,0x01,0xFF}, { 93,0x00,0xFF}, { 94,0x00,0xFF}, { 95,0x00,0x3F},
    { 96,0x10,0x00}, { 97,0xC0,0xFF}, { 98,0x2E,0xFF}, { 99,0x00,0xFF}, {100,0x00,0xFF}, {101,0x00,0xFF},   {102,0x00,0xFF}, {103,0x02,0xFF}, {104,0x00,0xFF}, {105,0x00,0xFF}, {106,0x80,0xBF}, {107,0x00,0xFF},
    {108,0x00,0xFF}, {109,0x00,0xFF}, {110,0x00,0xFF}, {111,0x00,0xFF}, {112,0x00,0xFF}, {113,0x00,0xFF},   {114,0xC0,0xFF}, {115,0x00,0xFF}, {116,0x80,0xFF}, {117,0x00,0xFF}, {118,0xC0,0xFF}, {119,0x00,0xFF},
    {120,0x00,0xFF}, {121,0x00,0xFF}, {122,0x00,0xFF}, {123,0x00,0xFF}, {124,0x00,0xFF}, {125,0x00,0xFF},   {126,0x00,0xFF}, {127,0x00,0xFF}, {128,0x00,0xFF}, {129,0x00,0x0F}, {130,0x00,0x0F}, {131,0x00,0xFF},
    {132,0x00,0xFF}, {133,0x00,0xFF}, {134,0x00,0xFF}, {135,0x00,0xFF}, {136,0x00,0xFF}, {137,0x00,0xFF},   {138,0x00,0xFF}, {139,0x00,0xFF}, {140,0x00,0xFF}, {141,0x00,0xFF}, {142,0x00,0xFF}, {143,0x00,0xFF},
    {144,0x00,0xFF}, {145,0x00,0x00}, {146,0xFF,0x00}, {147,0x00,0x00}, {148,0x00,0x00}, {149,0x00,0x00},   {150,0x00,0x00}, {151,0x00,0x00}, {152,0x00,0xFF}, {153,0x00,0xFF}, {154,0x00,0xFF}, {155,0x00,0xFF},
    {156,0x00,0xFF}, {157,0x00,0xFF}, {158,0x00,0x0F}, {159,0x00,0x0F}, {160,0x00,0xFF}, {161,0x00,0xFF},   {162,0x00,0xFF}, {163,0x00,0xFF}, {164,0x00,0xFF}, {165,0x00,0xFF}, {166,0x00,0xFF}, {167,0x00,0xFF},
    {168,0x00,0xFF}, {169,0x00,0xFF}, {170,0x00,0xFF}, {171,0x00,0xFF}, {172,0x00,0xFF}, {173,0x00,0xFF},   {174,0x00,0xFF}, {175,0x00,0xFF}, {176,0x00,0xFF}, {177,0x00,0xFF}, {178,0x00,0xFF}, {179,0x00,0xFF},
    {180,0x00,0xFF}, {181,0x00,0x0F}, {182,0x00,0xFF}, {183,0x00,0xFF}, {184,0x00,0xFF}, {185,0x00,0xFF},   {186,0x00,0xFF}, {187,0x00,0xFF}, {188,0x00,0xFF}, {189,0x00,0xFF}, {190,0x00,0xFF}, {191,0x00,0xFF},
    {192,0x00,0xFF}, {193,0x00,0xFF}, {194,0x00,0xFF}, {195,0x00,0xFF}, {196,0x00,0xFF}, {197,0x00,0xFF},   {198,0x00,0xFF}, {199,0x00,0xFF}, {200,0x00,0xFF}, {201,0x00,0xFF}, {202,0x00,0xFF}, {203,0x00,0x0F},
    {204,0x00,0xFF}, {205,0x00,0xFF}, {206,0x00,0xFF}, {207,0x00,0xFF}, {208,0x00,0xFF}, {209,0x00,0xFF},   {210,0x00,0xFF}, {211,0x00,0xFF}, {212,0x00,0xFF}, {213,0x00,0xFF}, {214,0x00,0xFF}, {215,0x00,0xFF},
    {216,0x00,0xFF}, {217,0x00,0xFF}, {218,0x00,0x00}, {219,0x00,0x00}, {220,0x00,0x00}, {221,0x0D,0x00},   {222,0x00,0x00}, {223,0x00,0x00}, {224,0xF4,0x00}, {225,0xF0,0x00}, {226,0x00,0x00}, {227,0x00,0x00},
    {228,0x00,0x00}, {229,0x00,0x00}, {231,0x00,0x00}, {232,0x00,0x00}, {233,0x00,0x00}, {234,0x00,0x00},   {235,0x00,0x00}, {236,0x00,0x00}, {237,0x00,0x00}, {238,0x14,0x00}, {239,0x00,0x00}, {240,0x00,0x00},
    {242,0x00,0x02}, {243,0xF0,0x00}, {244,0x00,0x00}, {245,0x00,0x00}, {247,0x00,0x00}, {248,0x00,0x00},   {249,0xA8,0x00}, {250,0x00,0x00}, {251,0x84,0x00}, {252,0x00,0x00}, {253,0x00,0x00}, {254,0x00,0x00},
    {255, 1, 0xFF}, // set page bit to 1 
    {  0,0x00,0x00}, {  1,0x00,0x00}, {  2,0x00,0x00}, {  3,0x00,0x00}, {  4,0x00,0x00}, {  5,0x00,0x00},   {  6,0x00,0x00}, {  7,0x00,0x00}, {  8,0x00,0x00}, {  9,0x00,0x00}, { 10,0x00,0x00}, { 11,0x00,0x00},
    { 12,0x00,0x00}, { 13,0x00,0x00}, { 14,0x00,0x00}, { 15,0x00,0x00}, { 16,0x00,0x00}, { 17,0x01,0x00},   { 18,0x00,0x00}, { 19,0x00,0x00}, { 20,0x90,0x00}, { 21,0x31,0x00}, { 22,0x00,0x00}, { 23,0x00,0x00},
    { 24,0x01,0x00}, { 25,0x00,0x00}, { 26,0x00,0x00}, { 27,0x00,0x00}, { 28,0x00,0x00}, { 29,0x00,0x00},   { 30,0x00,0x00}, { 31,0x00,0xFF}, { 32,0x00,0xFF}, { 33,0x01,0xFF}, { 34,0x00,0xFF}, { 35,0x00,0xFF},
    { 36,0x90,0xFF}, { 37,0x31,0xFF}, { 38,0x00,0xFF}, { 39,0x00,0xFF}, { 40,0x01,0xFF}, { 41,0x00,0xFF},   { 42,0x00,0xFF}, { 43,0x00,0x0F}, { 44,0x00,0x00}, { 45,0x00,0x00}, { 46,0x00,0x00}, { 47,0x00,0xFF},
    { 48,0x00,0xFF}, { 49,0x01,0xFF}, { 50,0x00,0xFF}, { 51,0x00,0xFF}, { 52,0x90,0xFF}, { 53,0x31,0xFF},   { 54,0x00,0xFF}, { 55,0x00,0xFF}, { 56,0x01,0xFF}, { 57,0x00,0xFF}, { 58,0x00,0xFF}, { 59,0x00,0x0F},
    { 60,0x00,0x00}, { 61,0x00,0x00}, { 62,0x00,0x00}, { 63,0x00,0xFF}, { 64,0x00,0xFF}, { 65,0x01,0xFF},   { 66,0x00,0xFF}, { 67,0x00,0xFF}, { 68,0x90,0xFF}, { 69,0x31,0xFF}, { 70,0x00,0xFF}, { 71,0x00,0xFF},
    { 72,0x01,0xFF}, { 73,0x00,0xFF}, { 74,0x00,0xFF}, { 75,0x00,0x0F}, { 76,0x00,0x00}, { 77,0x00,0x00},   { 78,0x00,0x00}, { 79,0x00,0xFF}, { 80,0x00,0xFF}, { 81,0x00,0xFF}, { 82,0x00,0xFF}, { 83,0x00,0xFF},
    { 84,0x90,0xFF}, { 85,0x31,0xFF}, { 86,0x00,0xFF}, { 87,0x00,0xFF}, { 88,0x01,0xFF}, { 89,0x00,0xFF},   { 90,0x00,0xFF}, { 91,0x00,0x0F}, { 92,0x00,0x00}, { 93,0x00,0x00}, { 94,0x00,0x00}, {255, 0, 0xFF} 
    }; // set page bit to 0

  static Reg_Data Reg_extClk[NUM_REGS_MAX] = {
    {  0,0x00,0x00}, {  1,0x00,0x00}, {  2,0x00,0x00}, {  3,0x00,0x00}, {  4,0x00,0x00}, {  5,0x00,0x00},   {  6,0x08,0x1D}, {  7,0x00,0x00}, {  8,0x70,0x00}, {  9,0x0F,0x00}, { 10,0x00,0x00}, { 11,0x00,0x00},
    { 12,0x00,0x00}, { 13,0x00,0x00}, { 14,0x00,0x00}, { 15,0x00,0x00}, { 16,0x00,0x00}, { 17,0x00,0x00},   { 18,0x00,0x00}, { 19,0x00,0x00}, { 20,0x00,0x00}, { 21,0x00,0x00}, { 22,0x00,0x00}, { 23,0x00,0x00},
    { 24,0x00,0x00}, { 25,0x00,0x00}, { 26,0x00,0x00}, { 27,0x70,0x80}, { 28,0x03,0xFF}, { 29,0x42,0xFF},   { 30,0xB0,0xFF}, { 31,0xC0,0xFF}, { 32,0xC0,0xFF}, { 33,0xC0,0xFF}, { 34,0xC0,0xFF}, { 35,0xAA,0xFF},
    { 36,0x06,0x1F}, { 37,0x06,0x1F}, { 38,0x06,0x1F}, { 39,0x06,0x1F}, { 40,0x84,0xFF}, { 41,0x10,0x7F},   { 42,0x24,0x3F}, { 43,0x00,0x00}, { 44,0x00,0x00}, { 45,0x00,0xFF}, { 46,0x00,0xFF}, { 47,0x14,0x3F},
    { 48,0x4B,0xFF}, { 49,0x00,0xFF}, { 50,0xC4,0xFF}, { 51,0x07,0xFF}, { 52,0x10,0xFF}, { 53,0x00,0xFF},   { 54,0x0D,0xFF}, { 55,0x00,0xFF}, { 56,0x00,0xFF}, { 57,0x00,0xFF}, { 58,0x00,0xFF}, { 59,0x01,0xFF},
    { 60,0x00,0xFF}, { 61,0x00,0xFF}, { 62,0x00,0x3F}, { 63,0x10,0xFF}, { 64,0x00,0xFF}, { 65,0x0D,0xFF},   { 66,0x00,0xFF}, { 67,0x00,0xFF}, { 68,0x00,0xFF}, { 69,0x00,0xFF}, { 70,0x01,0xFF}, { 71,0x00,0xFF},
    { 72,0x00,0xFF}, { 73,0x00,0x3F}, { 74,0x10,0xFF}, { 75,0x00,0xFF}, { 76,0x0D,0xFF}, { 77,0x00,0xFF},   { 78,0x00,0xFF}, { 79,0x00,0xFF}, { 80,0x00,0xFF}, { 81,0x01,0xFF}, { 82,0x00,0xFF}, { 83,0x00,0xFF},
    { 84,0x00,0x3F}, { 85,0x10,0xFF}, { 86,0x00,0xFF}, { 87,0x0D,0xFF}, { 88,0x00,0xFF}, { 89,0x00,0xFF},   { 90,0x00,0xFF}, { 91,0x00,0xFF}, { 92,0x01,0xFF}, { 93,0x00,0xFF}, { 94,0x00,0xFF}, { 95,0x00,0x3F},
    { 96,0x10,0x00}, { 97,0x00,0xFF}, { 98,0x3A,0xFF}, { 99,0x00,0xFF}, {100,0x00,0xFF}, {101,0x00,0xFF},   {102,0x00,0xFF}, {103,0x01,0xFF}, {104,0x00,0xFF}, {105,0x00,0xFF}, {106,0x80,0xBF}, {107,0x00,0xFF},
    {108,0x00,0xFF}, {109,0x00,0xFF}, {110,0x00,0xFF}, {111,0x00,0xFF}, {112,0x00,0xFF}, {113,0x00,0xFF},   {114,0xC0,0xFF}, {115,0x00,0xFF}, {116,0x80,0xFF}, {117,0x00,0xFF}, {118,0xC0,0xFF}, {119,0x00,0xFF},
    {120,0x00,0xFF}, {121,0x00,0xFF}, {122,0x00,0xFF}, {123,0x00,0xFF}, {124,0x00,0xFF}, {125,0x00,0xFF},   {126,0x00,0xFF}, {127,0x00,0xFF}, {128,0x00,0xFF}, {129,0x00,0x0F}, {130,0x00,0x0F}, {131,0x00,0xFF},
    {132,0x00,0xFF}, {133,0x00,0xFF}, {134,0x00,0xFF}, {135,0x00,0xFF}, {136,0x00,0xFF}, {137,0x00,0xFF},   {138,0x00,0xFF}, {139,0x00,0xFF}, {140,0x00,0xFF}, {141,0x00,0xFF}, {142,0x00,0xFF}, {143,0x00,0xFF},
    {144,0x00,0xFF}, {145,0x00,0x00}, {146,0xFF,0x00}, {147,0x00,0x00}, {148,0x00,0x00}, {149,0x00,0x00},   {150,0x00,0x00}, {151,0x00,0x00}, {152,0x00,0xFF}, {153,0x00,0xFF}, {154,0x00,0xFF}, {155,0x00,0xFF},
    {156,0x00,0xFF}, {157,0x00,0xFF}, {158,0x00,0x0F}, {159,0x00,0x0F}, {160,0x00,0xFF}, {161,0x00,0xFF},   {162,0x00,0xFF}, {163,0x00,0xFF}, {164,0x00,0xFF}, {165,0x00,0xFF}, {166,0x00,0xFF}, {167,0x00,0xFF},
    {168,0x00,0xFF}, {169,0x00,0xFF}, {170,0x00,0xFF}, {171,0x00,0xFF}, {172,0x00,0xFF}, {173,0x00,0xFF},   {174,0x00,0xFF}, {175,0x00,0xFF}, {176,0x00,0xFF}, {177,0x00,0xFF}, {178,0x00,0xFF}, {179,0x00,0xFF},
    {180,0x00,0xFF}, {181,0x00,0x0F}, {182,0x00,0xFF}, {183,0x00,0xFF}, {184,0x00,0xFF}, {185,0x00,0xFF},   {186,0x00,0xFF}, {187,0x00,0xFF}, {188,0x00,0xFF}, {189,0x00,0xFF}, {190,0x00,0xFF}, {191,0x00,0xFF},
    {192,0x00,0xFF}, {193,0x00,0xFF}, {194,0x00,0xFF}, {195,0x00,0xFF}, {196,0x00,0xFF}, {197,0x00,0xFF},   {198,0x00,0xFF}, {199,0x00,0xFF}, {200,0x00,0xFF}, {201,0x00,0xFF}, {202,0x00,0xFF}, {203,0x00,0x0F},
    {204,0x00,0xFF}, {205,0x00,0xFF}, {206,0x00,0xFF}, {207,0x00,0xFF}, {208,0x00,0xFF}, {209,0x00,0xFF},   {210,0x00,0xFF}, {211,0x00,0xFF}, {212,0x00,0xFF}, {213,0x00,0xFF}, {214,0x00,0xFF}, {215,0x00,0xFF},
    {216,0x00,0xFF}, {217,0x00,0xFF}, {218,0x00,0x00}, {219,0x00,0x00}, {220,0x00,0x00}, {221,0x0D,0x00},   {222,0x00,0x00}, {223,0x00,0x00}, {224,0xF4,0x00}, {225,0xF0,0x00}, {226,0x00,0x00}, {227,0x00,0x00},
    {228,0x00,0x00}, {229,0x00,0x00}, {231,0x00,0x00}, {232,0x00,0x00}, {233,0x00,0x00}, {234,0x00,0x00},   {235,0x00,0x00}, {236,0x00,0x00}, {237,0x00,0x00}, {238,0x14,0x00}, {239,0x00,0x00}, {240,0x00,0x00},
    {242,0x00,0x02}, {243,0xF0,0x00}, {244,0x00,0x00}, {245,0x00,0x00}, {247,0x00,0x00}, {248,0x00,0x00},   {249,0xA8,0x00}, {250,0x00,0x00}, {251,0x84,0x00}, {252,0x00,0x00}, {253,0x00,0x00}, {254,0x00,0x00},
    {255, 1, 0xFF}, // set page bit to 1 
    {  0,0x00,0x00}, {  1,0x00,0x00}, {  2,0x00,0x00}, {  3,0x00,0x00}, {  4,0x00,0x00}, {  5,0x00,0x00},   {  6,0x00,0x00}, {  7,0x00,0x00}, {  8,0x00,0x00}, {  9,0x00,0x00}, { 10,0x00,0x00}, { 11,0x00,0x00},
    { 12,0x00,0x00}, { 13,0x00,0x00}, { 14,0x00,0x00}, { 15,0x00,0x00}, { 16,0x00,0x00}, { 17,0x01,0x00},   { 18,0x00,0x00}, { 19,0x00,0x00}, { 20,0x90,0x00}, { 21,0x31,0x00}, { 22,0x00,0x00}, { 23,0x00,0x00},
    { 24,0x01,0x00}, { 25,0x00,0x00}, { 26,0x00,0x00}, { 27,0x00,0x00}, { 28,0x00,0x00}, { 29,0x00,0x00},   { 30,0x00,0x00}, { 31,0x00,0xFF}, { 32,0x00,0xFF}, { 33,0x01,0xFF}, { 34,0x00,0xFF}, { 35,0x00,0xFF},
    { 36,0x90,0xFF}, { 37,0x31,0xFF}, { 38,0x00,0xFF}, { 39,0x00,0xFF}, { 40,0x01,0xFF}, { 41,0x00,0xFF},   { 42,0x00,0xFF}, { 43,0x00,0x0F}, { 44,0x00,0x00}, { 45,0x00,0x00}, { 46,0x00,0x00}, { 47,0x00,0xFF},
    { 48,0x00,0xFF}, { 49,0x01,0xFF}, { 50,0x00,0xFF}, { 51,0x00,0xFF}, { 52,0x90,0xFF}, { 53,0x31,0xFF},   { 54,0x00,0xFF}, { 55,0x00,0xFF}, { 56,0x01,0xFF}, { 57,0x00,0xFF}, { 58,0x00,0xFF}, { 59,0x00,0x0F},
    { 60,0x00,0x00}, { 61,0x00,0x00}, { 62,0x00,0x00}, { 63,0x00,0xFF}, { 64,0x00,0xFF}, { 65,0x01,0xFF},   { 66,0x00,0xFF}, { 67,0x00,0xFF}, { 68,0x90,0xFF}, { 69,0x31,0xFF}, { 70,0x00,0xFF}, { 71,0x00,0xFF},
    { 72,0x01,0xFF}, { 73,0x00,0xFF}, { 74,0x00,0xFF}, { 75,0x00,0x0F}, { 76,0x00,0x00}, { 77,0x00,0x00},   { 78,0x00,0x00}, { 79,0x00,0xFF}, { 80,0x00,0xFF}, { 81,0x00,0xFF}, { 82,0x00,0xFF}, { 83,0x00,0xFF},
    { 84,0x90,0xFF}, { 85,0x31,0xFF}, { 86,0x00,0xFF}, { 87,0x00,0xFF}, { 88,0x01,0xFF}, { 89,0x00,0xFF},   { 90,0x00,0xFF}, { 91,0x00,0x0F}, { 92,0x00,0x00}, { 93,0x00,0x00}, { 94,0x00,0x00}, {255, 0, 0xFF} 
    }; // set page bit to 0




static uint convert(char *st) {
  char *x;
  for (x = st ; *x ; x++) {
    if (!isdigit(*x))
      return 0L;
  }
  return (strtoul(st, 0L, 10));
}




























//==============================================================================
//----===@ timingDriver
// Parameters  :
// Description :
//==============================================================================
  timingDriver::timingDriver(const int mode, const int devNum):firmware(0.1),software(3.14125)
  {
    isInit = 0;
    sys_init(mode, devNum); 
  }

//==============================================================================
//----===@ ~timingDriver
// Parameters  :
// Description :
//==============================================================================
  timingDriver::~timingDriver()
  {
    isInit = 0;
    prnM2("~timingDriver\r\n");
  }

//==============================================================================
//----===@ ~timingDriver
// Parameters  :
// Description :
//==============================================================================
  timingDriver* timingDriver::getInstance()
  {
    return this;
  }
  

//==============================================================================
//----===@ test
// Parameters  :
// Description :
//==============================================================================
int timingDriver::test(int argc, char *argv[])
{
	int val[2] = {0,};

	if(argc>2) val[0] = convert(argv[2]);
	if(argc>3) val[1] = convert(argv[3]);

	if (strcmp(argv[1], "help") == 0) 
	{
		prnM2("%-20s: %-40s\r\n", "iicInfo" , "print timing system 2 iic infomation");
		prnM2("%-20s: %-40s\r\n", "prnAll"  , "");
		prnM2("%-20s: %-40s\r\n", "prnIP"   , "");
		prnM2("%-20s: %-40s\r\n", "prnSys"  , "");
		prnM2("%-20s: %-40s\r\n", "prnGtp"  , "");
		prnM2("%-20s: %-40s\r\n", "prnEvg"  , "");
		prnM2("%-20s: %-40s\r\n", "prnEvr"  , "");
		prnM2("%-20s: %-40s\r\n", "gtpRst"  , "");
		prnM2("%-20s: %-40s\r\n", "fanSpeed", "");
		prnM2("%-20s: %-40s\r\n", "fled"    , "");
		prnM2("%-20s: %-40s\r\n", "enEvg"   , "enable txEnable_evCodeB");
		prnM2("%-20s: %-40s\r\n", "enEvr"   , "enable txEnable_evCodeB & stream data");
		prnM2("%-20s: %-40s\r\n", "ev"      , "1920 clear, 1936 log on, 1937 stop, 1938 logfifo reset");
		prnM2("%-20s: %-40s\r\n", "trg"     , "xxxx us delay");
		prnM2("%-20s: %-40s\r\n", "seqR"    , "0 or 1, seq Number");
		prnM2("%-20s: %-40s\r\n", "prnEvLog", "");
		prnM2("%-20s: %-40s\r\n", "bist", "0:dbus test[mxc0~7]");
		return RET_OK;
	}

	if (strcmp(argv[1], "iicInfo"  ) == 0){ iic_prnInfo();                  return RET_OK;    }
	if (strcmp(argv[1], "prnAll"   ) == 0){ ifRet(argc<3); prn_all(val[0]); return RET_OK;   }
	if (strcmp(argv[1], "prnIP"    ) == 0){ ifRet(argc<3); prn_ip (val[0]); return RET_OK;    }
	if (strcmp(argv[1], "prnSys"   ) == 0){ ifRet(argc<3); prn_sys(val[0]); return RET_OK;    }
	if (strcmp(argv[1], "prnGtp"   ) == 0){ ifRet(argc<3); prn_gtp(val[0]); return RET_OK;    }
	if (strcmp(argv[1], "prnEvg"   ) == 0){ ifRet(argc<3); prn_evg(val[0]); return RET_OK;    }
	if (strcmp(argv[1], "prnEvr"   ) == 0){ ifRet(argc<3); prn_evr(val[0]); return RET_OK;    }
	if (strcmp(argv[1], "gtpRst"   ) == 0){ set_gtp(SET_gtp_reset, 100000); return RET_OK;    }
	if (strcmp(argv[1], "fanSpeed" ) == 0){ ifRet(argc<3); set_fanSpeed(val[0], val[1]);    return RET_OK;    }
	if (strcmp(argv[1], "fled"     ) == 0){ ifRet(argc<3); set_sys_FMC_LED(val[0], val[1]); return RET_OK;    }
	if (strcmp(argv[1], "enEvg"    ) == 0){ set_evg_enable(1,1,1,1,1,1,1);  return RET_OK;    } //tx enable : dbus, evCodeA, evCodeB / enable mevCodeA, seqA, seqB, mxc
	if (strcmp(argv[1], "enEvr"    ) == 0){ set_evr_enable(1,1,1,1,1);      return RET_OK;    } //rx enable : dbus, evCodeA, evCodeB, stream data / enable pdp out
	if (strcmp(argv[1], "ev"    ) == 0){ ifRet(argc<3); set_evUser(val[0]);  return RET_OK;    } 
	if (strcmp(argv[1], "trg"   ) == 0){ ifRet(argc<3); set_evUTrg(val[0]);  return RET_OK;    } 
	if (strcmp(argv[1], "seqR"  ) == 0){ ifRet(argc<3); set_evSeq_Resume(val[0]);  return RET_OK;    } 
	if (strcmp(argv[1], "prnEvLog" ) == 0){ prn_evLog();  return RET_OK;    } 
	if (strcmp(argv[1], "bist" ) == 0){ ifRet(argc<3); bist(val[0]);  return RET_OK;    } 

	prnErr();
	return 0;
}

//==============================================================================
//----===@ biTest
// Parameters  :
// Description :
//==============================================================================
  int timingDriver::bist(uint tMode)
  {
    int i;

    prnM2("%-20s: %d.\r\n", "[ Built in Self Test ]" , tMode);

    switch(tMode)
    {
      case 0 :
                prnM2("-- dbus test [mxc00~07] --\r\n");

                for(i=0;i<14;i++) set_evg_mxcFreq(i, 1000.0 * (i+1));
                
                for(i=0;i<8;i++) set_evg_tx_DBusSel(i, (ev_src)(MXC_00 + i)); //default evg tx dbus soure : external Input SL 0~7

                //============================================================================================
                //-- default setting : evr ext out source select
                for(i=0;i<8;i++)
                  cfg_evr_outSrcSel((ev_out)(EXT_OUT_SL_00 + i), (ev_out_src)(RXDBUS_00 + i)); //default evr external out soure 0~7 : rxDbus 0~7
                 
                //-- default setting : evr pdp trigger from evCodeB, sl-sh swap, ext out 31 from pdp 31
                if((evMode == RAON_EVG) || (evMode == RAON_EVS))
                  set_evr_pdpAndOut(0,1);
                else
                  set_evr_pdpAndOut(0,0);

                              //dbus, evCodeA, evCodeB, mevCodeA, seqA, seqB, mxc
                set_evg_enable(    1,       0,       0,        0,    0,    0,   1); 
                set_evr_enable(    1,       0,       0,        1,    0);
                              //dbus, evCodeA, evCodeB, stream data, pdp out
                break;
      case 1 :
                prnM2("-- evCode test [USER_TRIGGER,mxc00~13] --\r\n");

                for(i=0;i<14;i++) set_evg_mxcFreq(i, 1000.0 * (i+1));
                
                //-- default setting : evg event trigger source select
                for(i=0;i<31;i++) set_evg_evTrg(i, (ev_src)(USER_TRIGGER + i));
                set_evg_evTrgMask(0x7fffffff);  

                for(i=0;i<31;i++)set_evg_mevCodeA(i,i+10);
                //============================================================================================
                for(i=0;i<31;i++)set_evr_evRam( i+10, 0x00000001<<i );
                set_evr_evRam(2,0xffffffff);

                //-- default setting : evr ext out source select
                for(i=0;i<32;i++)
                  cfg_evr_outSrcSel((ev_out)(EXT_OUT_SL_00 + i), (ev_out_src)(PDP_OUT_00 + i)); //default evr external out soure 0~7 : rxDbus 0~7

                //-- default setting : evr pdp trigger from evCodeB, sl-sh swap, ext out 31 from pdp 31
                set_evr_pdpAndOut(0,0);

                              //dbus, evCodeA, evCodeB, mevCodeA, seqA, seqB, mxc
                set_evg_enable(    0,       0,       1,        1,    0,    0,   1); 
                set_evr_enable(    0,       0,       1,        1,    1);
                              //dbus, evCodeA, evCodeB, stream data, pdp out
                break;
      case 2 :
                prnM2("-- seq test [USER_TRIGGER,mxc00~13] --\r\n");
                              //dbus, evCodeA, evCodeB, mevCodeA, seqA, seqB, mxc
                set_evg_enable(    0,       0,       1,        0,    0,    0,   1); 

                for(i=0;i<14;i++) set_evg_mxcFreq(i, 1000.0 * (i+1));
                
                i=0; set_evg_seqTable(0, i, 1920, 0, i*20);
                i++; set_evg_seqTable(0, i, 1936, 0, i*20);
                i++; set_evg_seqTable(0, i, i+10, 0, i*20);
                i++; set_evg_seqTable(0, i, i+10, 0, i*20);
                i++; set_evg_seqTable(0, i, i+10, 0, i*20);
                i++; set_evg_seqTable(0, i, i+10, 0, i*20);
                i++; set_evg_seqTable(0, i, i+10, 0, i*20);
                i++; set_evg_seqTable(0, i, i+10, 0, i*20);
                i++; set_evg_seqTable(0, i, i+10, 0, i*20);
                i++; set_evg_seqTable(0, i, i+10, 0, i*20);
                i++; set_evg_seqTable(0, i, i+10, 0, i*20);
                i++; set_evg_seqTable(0, i, 1937, 0, i*20);
                i++; set_evg_seqTable(0, i,    0, 1, i*20);

                cfg_evg_seqSrc(0,seqS_usrTrg);
                cfg_evg_seqSrc(1,seqS_usrTrg);
                cfg_evg_seqSrc(2,seqS_usrTrg|seqS_mxc00);
                cfg_evg_seqSrc(3,seqS_usrTrg);
                set_evg_seqTrg(seqS_CMB_OR0, 0, seqS_CMB_NONE, 0);
                
                //============================================================================================
                for(i=0;i<16;i++)set_evr_evRam( i+10, 0x00010000<<i );
                set_evr_evRam(1,0x00000001);

                //-- default setting : evr ext out source select
                for(i=0;i<32;i++)
                  cfg_evr_outSrcSel((ev_out)(EXT_OUT_SL_00 + i), (ev_out_src)(PDP_OUT_00 + i)); //default evr external out soure 0~7 : rxDbus 0~7

                //-- default setting : evr pdp trigger from evCodeB, sl-sh swap, ext out 31 from pdp 31
                set_evr_pdpAndOut(0,0);

                              //dbus, evCodeA, evCodeB, mevCodeA, seqA, seqB, mxc
                set_evg_enable(    0,       0,       1,        0,    1,    1,   1); 
                set_evr_enable(    0,       0,       1,        1,    1);
                              //dbus, evCodeA, evCodeB, stream data, pdp out
                break;
      case 3 :
				prn_evLog();
                break;
      case 4 :
				prn_all(4);
                break;
      default:
                break;
    }

    return RET_OK;
  }  

//==============================================================================
//----===@ init
// Parameters  : mode
// Description :
//==============================================================================
  int timingDriver::sys_init(int mode, int num)
  {
    int i = 0;
    int rt = 0;
    
    opMode = TS_OP_MODE_NORMAL;
    evMode = mode;
    evNum  = num;
    
    prnM2("\n");
    prnM2("===== timingDriver %s =====\r\n", "sys_init");

    // int adc;
    // adc = open("/sys/devices/soc0/amba/f8007100.adc",O_RDWR);
    // ifRet(adc == -1)
    // prnM2("===== ./sys/bus/platform/drivers/xadc =====\r\n");


    //-- initialize ip, get file descriptor, ev, gtp, iic 
    rt = init_ip();
    ifRet(rt == RET_ERR);

    //-- get status -> isMaster status
    get_sysStat();

    if(ev.isMaster == 0){
      rt = init_bp();
      ifRet(rt == RET_ERR);
    }

    //-- initialize onboard cross point switch chip ADN4604
    rt = init_cps(IIC_BUS_ob);
    ifRet(rt == RET_ERR); 
    
    //-- get slave boards ID, sl & sh ID
    rt = get_slaveBoardID(); //get slave board ID, 1st & 2nd floor
    ifRet(rt == RET_ERR);

    //-- initialize slave fanout module cross point switch chip ADN4604
    if(sl.id == 0x02){
      rt = init_cps(IIC_BUS_sl);
      prnMeta(rt == RET_ERR);
    }

    if(sh.id == 0x02){
      rt = init_cps(IIC_BUS_sh);
      prnMeta(rt == RET_ERR);
    }

    //-- initialize SI5338 clock distribution chip
    rt = init_clk();
    ifRet(rt == RET_ERR);
    taskDelay(500000);

    isInit = 1;
                  //dbus, evCodeA, evCodeB, mevCodeA, seqA, seqB, mxc
    set_evg_enable(    0,       0,       0,        0,    0,    0,   0); 

    // if((evMode == RAON_EVG) || (evMode == RAON_EVS))
    //   set_evr_enable(    0,       0,       0,        0,    0);
    // else
    set_evr_enable(    0,       0,       0,        1,    0);
                  //dbus, evCodeA, evCodeB, stream data, pdp out

    struct s_evTime igenTime;
    if(evMode == RAON_EVG)
    {
      ev.igen_enable   = 1;
      ev.igen_mode_ext = 1;
    }
    else
    {
      ev.igen_enable   = 0;
      ev.igen_mode_ext = 0;
    }
    set_evg_igen(ev.igen_enable, ev.igen_mode_ext, igenTime);

    //-- set value to cross point switch chip by evMode
    set_cpsByMode();

    //-- set Input/Output config slave io module by evMode
    set_slaveIO_byMode();

    //-- default setting : evg mxc freq setting 1000Hz
    for(i=0;i<14;i++) set_evg_mxcFreq(i, 1000.0);

    //-- default setting : evg event trigger source select
    for(i=1;i<16;i++) set_evg_evTrg(i, (ev_src)(EXT_IN_SL_00 + i));
    set_evg_evTrgMask(0x7fffffff);  


    //-- default setting : evg tx dbus source select
    for(i=0;i<8;i++) set_evg_tx_DBusSel(i, (ev_src)(EXT_IN_SL_00 + i)); //default evg tx dbus soure : external Input SL 0~7

    cfg_evg_seqSrc(0,seqS_usrTrg);
    cfg_evg_seqSrc(1,seqS_usrTrg);
    cfg_evg_seqSrc(2,seqS_usrTrg);
    cfg_evg_seqSrc(3,seqS_usrTrg);
    set_evg_seqTrg(seqS_CMB_NONE, 0, seqS_CMB_NONE, 0);

    //-- default setting : evr pdp - delay 0, pulse width 10 clock, polarity high.
    for(i=0;i<32;i++)
      cfg_evr_pdp(i,0,10,1);
    
    //-- default setting : evr ext out source select
    for(i=0;i<32;i++)
      cfg_evr_outSrcSel((ev_out)(EXT_OUT_SL_00 + i), (ev_out_src)(PDP_OUT_00 + i)); //default evr external out soure : pdpOutput 0~31

    //-- default setting : evr pdp trigger from evCodeB, sl-sh no swap, ext out 31 from pdp 31
    set_evr_pdpAndOut(0,0);

    //-- init gtp clock and then reset gtp
    rt = set_gtp(SET_gtp_reset, 100000);
    ifRet(rt == RET_ERR);





    get_ipInfo(ev.fd, &ev.info);
    get_ipInfo(gtp.fd, &gtp.info);


    return RET_OK;
  }




//==============================================================================
//----===@ iic_getInfo
// Parameters  :
// Description :
//==============================================================================
  s_ts2iic* timingDriver::iic_getInfo(int busId, int slvid)
  {
    return iic_piic[busId]+slvid;
  };

//==============================================================================
//----===@ iic_prnInfo
// Parameters  :
// Description :
//==============================================================================
  int timingDriver::iic_prnInfo()
  {
    int i,j;

    prnM2("-----------------------------------------------\n");
    prnM2("   M.S \r\t iicAddr \r\t\t isMux \r\t\t\t MuxAddr \r\t\t\t\t MuxCh\n");
    prnM2("-----------------------------------------------\n");

    for(i = 0; i < IIC_maxDrv; i++)
    {

      for(j = 0; j < iic_slvNum[i]; j++)
      {
        s_ts2iic* piic = iic_getInfo(i, j);
        prnM2("   %d.%d \r\t  0x%02x \r\t\t   %d \r\t\t\t   0x%02x \r\t\t\t\t   %d\n", 
            i, piic->name, piic->addr, piic->isMux, piic->muxAddress, piic->muxNum );
      };
    }
    prnM2("\n");

    return RET_OK;
  };


//==============================================================================
//----===@ 
// Parameters  :
// Description :
//==============================================================================
  int timingDriver::init_bp()
  {
    unsigned char txBuf;
    uint txReg;
    int bytesWrite;

    //---------------------------------------------------------------------------
    // port I/O Config
    // max7313 port I/O config, 1:input, 0:output 
    if(max7313_configIO(IIC_BUS_bp, SLV_MAX7313_A, 0xf0, 0xf0 )!= 1) 
      prnErrRet();

    //---------------------------------------------------------------------------
    // PWM config 
    // Master, O16 Intensity Register -> Master intensity duty cycle is 15/15 (full), O16 intensity duty cycle is 1/16
    txReg = 0x0e; txBuf = 0xf0;
    bytesWrite = iic_wr(IIC_BUS_bp, SLV_MAX7313_A, txReg, 1, (char*)&txBuf, 1 );
    if(bytesWrite != 1) 
      prnErrRet();

    // Configuration Register -> Enable blink
    txReg = 0x0f; txBuf = 0x01;
    bytesWrite = iic_wr(IIC_BUS_bp, SLV_MAX7313_A, txReg, 1, (char*)&txBuf, 1 );
    if(bytesWrite != 1) 
      prnErrRet();

    // Outputs intensity P9, P8 
    if(set_fanSpeed( 9, 4)!= 1) 
      prnErrRet();

    //---------------------------------------------------------------------------
    // port Output set to high or low
    if(max7313_setOutput(IIC_BUS_bp, SLV_MAX7313_A, 0x03, 0x00 )!= 1) 
      prnErrRet();

    return RET_OK;
  };



//==============================================================================
//----===@ 
// Parameters  :
// Description :
//==============================================================================
  int timingDriver::init_cps(int busId)
  {
    unsigned char txBuf;
    uint txReg;
    int bytesWrite;

    ifRet(busId > IIC_BUS_MAX);
    
    prnM2("init_cps %d - ",busId);

    switch (busId) 
    {
      case 1  : prnM2("Onboard.\r\n"); break;
      case 2  : prnM2("Lower Layer Slave Board.\r\n"); break;
      case 3  : prnM2("Upper Layer Slave board.\r\n"); break;
      default : prnM2("[ERR] . \r\n"); break;
    }

    //adn4604 reset
    txReg = 0x00; txBuf = 1;
    bytesWrite = iic_wr(busId, SLV_ADN4604, txReg, 1, (char*)&txBuf, 1 );
    if(bytesWrite != 1)
      prnErrRet(); 

    //adn4604 mapselect 0
    txReg = 0x81; txBuf = 0x00;
    bytesWrite = iic_wr(busId, SLV_ADN4604, txReg, 1, (char*)&txBuf, 1 );
    if(bytesWrite != 1)
      prnErrRet(); 

    //adn4604 termination - All Termination Enable
    txReg = 0xF0; txBuf = 0x00;
    bytesWrite = iic_wr(busId, SLV_ADN4604, txReg, 1, (char*)&txBuf, 1 );
    if(bytesWrite != 1)
      prnErrRet(); 

    //adn4604 update
    txReg =0x80;  txBuf = 0x01;
    bytesWrite = iic_wr(busId, SLV_ADN4604, txReg, 1, (char*)&txBuf, 1 );
    if(bytesWrite != 1)
      prnErrRet(); 

    prnM2("init_cps ok..\r\n");
    return RET_OK;
  };


//==============================================================================
//----===@ 
// Parameters  :
// Description :
//==============================================================================
int timingDriver::start_thread()
{
	prnM2("start_thread\r\n");
	int gt0_tLoss_pre = -1;

	struct s_evTime pre_evrTime;
	pre_evrTime.year = 500;
	pre_evrTime.day  = 500;
	pre_evrTime.hour = 500;
	pre_evrTime.min  = 500;
	pre_evrTime.sec  = 0;

	uint pre_rx_evCodeA_cntr = 0xffff;
	uint pre_rx_evCodeB_cntr = 0xffff;
	uint rx_evCode_miss = 1;

	float pre_slTemp = 0.0;
	float pre_shTemp = 0.0;

	char lcdStr[100];

	unsigned char fpLed_pwrOK = 1;
	unsigned char fpLed_fanERR = 0;
	unsigned char fpLed_epicsOK = 1;
	unsigned char fpLed_timingNetworkOK = 0;
	unsigned char fpLed_stat = 0;
	unsigned char fpLed_stat_pre = 0;

	unsigned char fmcLinkLED_0 = 0;
	unsigned char fmcLinkLED_1 = 0;
	unsigned char fmcLinkLED_stat = 0;
	unsigned char fmcLinkLED_pre = 0;


	uint rdData = 0;
	int  rdDataSize = 0;

	uint runCount = 0;

	lcdClear();

	sprintf(lcdStr, "%02d:\n", ev.evrTime.hour);

	switch(evMode)
	{
		case RAON_EVG    : sprintf(evName, "%s"     , "EVG"      ); sprintf(lcdStr, "   [ RAON %s ]\n", evName); break;
		case RAON_EVF    : sprintf(evName, "%s-%03d", "EVF",evNum); sprintf(lcdStr, " [ RAON %s ]\n"  , evName); break;
		case RAON_EVS    : sprintf(evName, "%s"     , "EVS"      ); sprintf(lcdStr, "   [ RAON %s ]\n", evName); break;
		case RAON_EVR    : sprintf(evName, "%s-%03d", "EVR",evNum); sprintf(lcdStr, " [ RAON %s ]\n"  , evName); break;
		case RAON_EVR_FP : sprintf(evName, "%s"     , "EVR-FP"   ); sprintf(lcdStr, "  [ RAON %s ]\n" , evName); break;
		default          : sprintf(evName, "%s"     , "NONE"     ); sprintf(lcdStr, "   [ %s ]\n"     , evName); break;
	}

	lcdSetColor(0x5C);
	lcdDrawStr(1,LCD_LINE_1,lcdStr);

	lcdSetColor(0x1F);
	lcdDrawStr(1,LCD_LINE_2,"Timing System v1.2\n");

	lcdSetColor(0xFF);
	lcdDrawStr(1,LCD_LINE_4,"Time:\n");
	lcdDrawStr(1,LCD_LINE_5,"Run :\n");
	lcdDrawStr(1,LCD_LINE_6,"Temp:\n");
	lcdDrawStr(1,LCD_LINE_7,"EV  :\n");

	set_fp_led(1,0,1,1);

	sys_intrClear(ev.fd);
	sys_intrMask(ev.fd, 0 \
			| C_intrMsk_evrW_alarm_sec \
			);

#ifdef D_TestMode_AXICLOCK
	uint clkCheck_now;
	uint clkCheck_pre;
	uint clkCheck_diff;
#endif

	while(isInit)
	{
		sys_intrEn(ev.fd, 1);
		if((rdDataSize = read(ev.fd, &rdData, sizeof(rdData))) < 0) return RET_OK;

#ifdef D_TestMode_AXICLOCK
		ip_rd(ev.fd, A_ipSys_clkCntr, &clkCheck_now);  
		clkCheck_diff = clkCheck_now - clkCheck_pre;
		clkCheck_pre = clkCheck_now;
		prnM1("clkCheck_diff : %d Hz\r\n", clkCheck_diff);
#endif

		get_evTime();
		get_sysStat();
		get_gtpStat();
		get_evrStat();
		get_slaveTemp();

		fpLed_pwrOK = 1;
		fpLed_fanERR = 0;
		fpLed_epicsOK = 1;

		if(pre_rx_evCodeA_cntr != ev.rx_evCodeA_cntr ){
			sprintf(lcdStr, "%6dA\n" , ev.rx_evCodeA_cntr  );
			lcdDrawStr(6,LCD_LINE_7,lcdStr);
		}
		pre_rx_evCodeA_cntr = ev.rx_evCodeA_cntr;

		if(pre_rx_evCodeB_cntr != ev.rx_evCodeB_cntr ){
			sprintf(lcdStr, "%6dB\n" , ev.rx_evCodeB_cntr  );
			lcdDrawStr(13,LCD_LINE_7,lcdStr);
		}
		pre_rx_evCodeB_cntr = ev.rx_evCodeB_cntr; 

		if((gt0_tLoss_pre != gtp.stat[0].trackLossCnt) || (gtp.stat[0].trackOK == 0))
		{
			fpLed_timingNetworkOK = 0;
			prnM1("[ERR]gt0 trackLoss : %d\r\n", gtp.stat[0].trackLossCnt - gt0_tLoss_pre);

			if(runCount%2 == 0)
				sprintf(lcdStr, "%-20s\n","[ERR] Timing Net   ");
			else
				sprintf(lcdStr, "%-20s\n","-------------------");

			lcdSetColor(0xE0); 
			lcdDrawStr(1,LCD_LINE_8,lcdStr);
			lcdSetColor(0xff); 
			rx_evCode_miss = 1;
		}
		else
		{
			fpLed_timingNetworkOK = 1;

			if((ev.rx_evCodeA_cntr != ev.streamRx_evCodeA_cntr ) || (ev.rx_evCodeB_cntr != ev.streamRx_evCodeB_cntr ))
			{

				if(runCount%2 == 0)
					sprintf(lcdStr, "%-20s\n","[ERR] Rx miss match");
				else
					sprintf(lcdStr, "%-20s\n","-------------------");

				lcdSetColor(0xE0); 
				lcdDrawStr(1,LCD_LINE_8,lcdStr);
				lcdSetColor(0xff); 

				rx_evCode_miss = 1;
			}
			else
			{
				if(rx_evCode_miss == 1)
				{
					sprintf(lcdStr, "%-20s\n","  EV Rx counts OK. ");
					lcdSetColor(0x5C); 
					lcdDrawStr(1,LCD_LINE_8,lcdStr);
					lcdSetColor(0xff); 
				}
				rx_evCode_miss = 0;
			}
		}

		fmcLinkLED_0 = (!gtp.stat[0].trackOK & 1) & ev.f_SFP_LossA;
		fmcLinkLED_1 = (!gtp.stat[1].trackOK & 1) & ev.f_SFP_LossB;
		fmcLinkLED_stat = (fmcLinkLED_0<<1) | fmcLinkLED_1;
		if(fmcLinkLED_stat != fmcLinkLED_pre)
			set_sys_FMC_LED(fmcLinkLED_1 & 1, fmcLinkLED_0 & 1);
		fmcLinkLED_pre = fmcLinkLED_stat;

		if(pre_evrTime.day  != ev.evrTime.day ){
			sprintf(lcdStr, "%03d.\n" , ev.evrTime.day  );
			lcdDrawStr(7,LCD_LINE_4,lcdStr);
		}

		if(pre_evrTime.hour != ev.evrTime.hour){
			sprintf(lcdStr, "%02d:\n", ev.evrTime.hour);
			lcdDrawStr(11,LCD_LINE_4,lcdStr);
		}

		if(pre_evrTime.min  != ev.evrTime.min ){
			sprintf(lcdStr, "%02d:\n", ev.evrTime.min);
			lcdDrawStr(14,LCD_LINE_4,lcdStr);
		}

		if(pre_evrTime.sec  != ev.evrTime.sec ){
			sprintf(lcdStr, "%02d\n", ev.evrTime.sec);
			lcdDrawStr(17,LCD_LINE_4,lcdStr);
		}

		pre_evrTime.day  = ev.evrTime.day ;
		pre_evrTime.hour = ev.evrTime.hour;
		pre_evrTime.min  = ev.evrTime.min ;
		pre_evrTime.sec  = ev.evrTime.sec ;

		if(pre_slTemp != sl.temp){
			sprintf(lcdStr, "L%02.01f\n" , sl.temp  );
			lcdDrawStr(7,LCD_LINE_6,lcdStr);
		}
		pre_slTemp = sl.temp;

		if(pre_shTemp != sh.temp){
			sprintf(lcdStr, " H%02.01f\n" , sh.temp  );
			lcdDrawStr(13,LCD_LINE_6,lcdStr);
		}
		pre_shTemp = sh.temp;

		if(fpLed_stat != fpLed_stat_pre)
			set_fp_led(fpLed_pwrOK, fpLed_fanERR, fpLed_epicsOK, fpLed_timingNetworkOK);
		fpLed_stat_pre = ((fpLed_pwrOK<<3) | (fpLed_fanERR<<2) | (fpLed_epicsOK<<1) | fpLed_timingNetworkOK);

		sys_intrClear(ev.fd);

		gt0_tLoss_pre = gtp.stat[0].trackLossCnt;

		runCount++;
	}

	prnM2("start_thread end...\r\n");
	return RET_OK;
};







//==============================================================================
//----===@ 
// Parameters  :
// Description :
//==============================================================================
  int timingDriver::set_cpsByMode()
  {
    ifRet(isInit != 1);

    ob.busId = IIC_BUS_ob;
    sl.busId = IIC_BUS_sl;
    sh.busId = IIC_BUS_sh;

    memset(ob.cpsOut  ,0,sizeof(ob.cpsOut));
    memset(ob.cpsOutEn,0,sizeof(ob.cpsOutEn));

    memset(sl.cpsOut  ,0,sizeof(sl.cpsOut));
    memset(sl.cpsOutEn,0,sizeof(sl.cpsOutEn));

    memset(sh.cpsOut  ,0,sizeof(sh.cpsOut));
    memset(sh.cpsOutEn,0,sizeof(sh.cpsOutEn));

    switch(evMode)
    {
      case RAON_EVG :     ob.cpsOut[CPS_GT0 ] = CPS_FMC2;
                          ob.cpsOut[CPS_GT1 ] = CPS_FMC1;
                          ob.cpsOut[CPS_FMC1] = CPS_GT0;
                          ob.cpsOut[CPS_FMC2] = CPS_GT0;
                          ob.cpsOut[CPS_SL  ] = CPS_GT0;
                          ob.cpsOut[CPS_SH  ] = CPS_GT0;
                          break;
                          
      case RAON_EVS :     ob.cpsOut[CPS_GT0 ] = CPS_GT0;
                          ob.cpsOut[CPS_GT1 ] = CPS_FMC1;
                          ob.cpsOut[CPS_FMC1] = CPS_GT0;
                          ob.cpsOut[CPS_FMC2] = CPS_GT0;
                          ob.cpsOut[CPS_SL  ] = CPS_GT0;
                          ob.cpsOut[CPS_SH  ] = CPS_GT0;
                          break;
    
      case RAON_EVF :     ob.cpsOut[CPS_GT0 ] = CPS_FMC1;
                          ob.cpsOut[CPS_GT1 ] = CPS_SL;
                          ob.cpsOut[CPS_FMC1] = CPS_FMC2;
                          ob.cpsOut[CPS_FMC2] = CPS_FMC1;
                          ob.cpsOut[CPS_SL  ] = CPS_FMC1;
                          ob.cpsOut[CPS_SH  ] = CPS_FMC1;
                          break;

      case RAON_EVR_FP :  ob.cpsOut[CPS_GT0 ] = CPS_FMC1;
                          ob.cpsOut[CPS_GT1 ] = CPS_FMC2;
                          ob.cpsOut[CPS_FMC1] = CPS_GT0;
                          ob.cpsOut[CPS_FMC2] = CPS_FMC1;
                          ob.cpsOut[CPS_SL  ] = CPS_FMC1;
                          ob.cpsOut[CPS_SH  ] = CPS_FMC1;
                          break;

      case RAON_EVR :     ob.cpsOut[CPS_GT0 ] = CPS_FMC1;
                          ob.cpsOut[CPS_GT1 ] = CPS_FMC2;
                          ob.cpsOut[CPS_FMC1] = CPS_FMC1;
                          ob.cpsOut[CPS_FMC2] = CPS_FMC1;
                          ob.cpsOut[CPS_SL  ] = CPS_FMC1;
                          ob.cpsOut[CPS_SH  ] = CPS_FMC1;
                          break;
      default   : break;
    }

    ob.cpsOutEn[CPS_GT0 ] = 0x30;
    ob.cpsOutEn[CPS_GT1 ] = 0x30;
    ob.cpsOutEn[CPS_FMC1] = 0x30;
    ob.cpsOutEn[CPS_FMC2] = 0x30;
    ob.cpsOutEn[CPS_SL  ] = 0x30;
    ob.cpsOutEn[CPS_SH  ] = 0x30;

    sl.cpsOut[CPS_SW_CTRL] = CPS_SW_00;
    sl.cpsOutEn[CPS_SW_CTRL] = 0x30;

    for(int i = 0; i < 12; i++)
    {
      sl.cpsOut[CPS_SW_00 + i] = CPS_SW_CTRL;
      sl.cpsOutEn[CPS_SW_00 + i] = 0x30;
    };

    sh.cpsOut[CPS_SW_CTRL] = CPS_SW_00;
    sh.cpsOutEn[CPS_SW_CTRL] = 0x30;

    for(int i = 0; i < 12; i++)
    {
      sh.cpsOut[CPS_SW_00 + i] = CPS_SW_CTRL;
      sh.cpsOutEn[CPS_SW_00 + i] = 0x30;
    };

    set_cpsConfig(&ob);

    if(sl.id==0x02)
      set_cpsConfig(&sl);

    if(sh.id==0x02)
      set_cpsConfig(&sh);

    return RET_OK;
  };


  
//==============================================================================
//----===@ set_cpsConfig
// Parameters  :
// Description :
//==============================================================================
  int timingDriver::set_cpsConfig(struct s_ts2slv* pCfg)
  {
    prnM0("set_cpsConfig busId : %d\r\n", pCfg->busId);

    ifRet(pCfg == NULL);
    ifRet(pCfg->busId > IIC_BUS_MAX);

    unsigned char txBuf;
    uint  txReg;
    int bytesWrite;

    int i;

    for( i = 0; i < 8; i++)
    {
      txReg = 0x90+i;
      txBuf = (pCfg->cpsOut[i*2] & 0x0f) | ((pCfg->cpsOut[i*2+1] & 0x0f) << 4);
      bytesWrite = iic_wr(pCfg->busId, SLV_ADN4604, txReg, 1, (char*)&txBuf, 1 );
      if(bytesWrite != 1) prnErrRet();
    }

    for( i = 0; i < 16; i++)
    {
      txReg = 0x20+i;
      bytesWrite = iic_wr(pCfg->busId, SLV_ADN4604, txReg, 1, (char*)&pCfg->cpsOutEn[i], 1 );
      if(bytesWrite != 1) prnErrRet();
    }

    txReg =0x80;  txBuf = 0x01;
    bytesWrite = iic_wr(pCfg->busId, SLV_ADN4604, txReg, 1, (char*)&txBuf, 1 );

    if(bytesWrite != 1) prnErrRet();

    return RET_OK;
  };



//==============================================================================
//----===@ set_slaveIO_byMode
// Parameters  :
// Description :
//==============================================================================
  int timingDriver::set_slaveIO_byMode()
  {
    ifRet(isInit != 1);

    ev.slio = 0xffff; // default : high[input]
    ev.shio = 0xffff;

    if(sl.id==0x01)
    {
      if((evMode == RAON_EVG) || (evMode == RAON_EVS))
      {
        ev.slio = 0xffff;
      }
      else
      {
        ev.slio = 0x0000;
      }
      set_slaveIO_sub(&sl, ev.slio);
    }

    if(sh.id==0x01)
    {
      ev.shio = 0x0;
      set_slaveIO_sub(&sh, ev.shio);
    }

    if(set_sys_IO(ev.slio, ev.shio) != 1) 
      prnErrRet();

    return RET_OK;
  };
  

//==============================================================================
//----===@ 
// Parameters  :
// Description :
//==============================================================================
  int timingDriver::set_slaveIO_sub(s_ts2slv* pSlv, uint setVal)
  {
    unsigned char wrData[2];

    ifRet(pSlv<0);

    //---------------------------------------------------------------------------
    // port Output set to high or low
    wrData[0] =  setVal & 0xff;
    wrData[1] = (setVal >> 8) & 0xff ;
    prnM2("%d_wrData[0] : %02x\r\n",pSlv->busId, wrData[0]);
    prnM2("%d_wrData[1] : %02x\r\n",pSlv->busId, wrData[1]);

    //---------------------------------------------------------------------------
    // port I/O Config
    // max7313 port I/O config, 1:input, 0:output 
    if(max7313_configIO(pSlv->busId, SLV_MAX7313_B, 0x00, 0x00 )!= 1) 
      prnErrRet();

    if(max7313_setOutput(pSlv->busId, SLV_MAX7313_B, wrData[1], wrData[0] )!= 1) 
      prnErrRet();

    return RET_OK;
  };

//==============================================================================
//----===@ 
// Parameters  :
// Description :
//==============================================================================
  int timingDriver::init_ip()
  {
    int i;

    for(i=0;i<5;i++)
    {
      ev.fd  = open(DRV_NAME_ip_ev,O_RDWR);
      if(ev.fd == -1)
        taskDelay(200000);
      else
        break;
    }

    for(i=0;i<5;i++)
    {
      gtp.fd = open(DRV_NAME_ip_gtp,O_RDWR);
      if(gtp.fd == -1)
        taskDelay(200000);
      else
        break;
    }

    prnM2("\n");
    prnM2("======================================================\r\n");
    prnM2("++ init_ip ++\r\n");

    if (ev.fd  == -1)prnErrRet();
    if (gtp.fd == -1)prnErrRet();

    //output reg  [31 : 0] m_st, // 3-state enable input, high=input, low=output
    ip_wr(ev.fd, A_cfgSys_IO_LSB, 0x0000ffff);
    ip_wr(ev.fd, A_cfgSys_IO_MSB, 0x0000ffff);
    ip_wr(ev.fd, A_cfgEvg_sw_evgW0, 1);

    sprintf(ev.evgTime.name, "evgTime");
    ev.evgTime.type = 1;
    sprintf(ev.evrTime.name, "evrTime");
    ev.evrTime.type = 1;
    sprintf(ev.info.buildTime.name, "ev-buildTime");
    ev.info.buildTime.type = 0;
    sprintf(gtp.info.buildTime.name, "gtp-buildTime");
    gtp.info.buildTime.type = 0;

    sprintf(ev.fStat_evCodeA.name,  "codeA");
    sprintf(ev.fStat_seqA.name,     "seqA");
    sprintf(ev.fStat_seqB.name,     "seqB");
    sprintf(ev.fStat_evUser.name,   "user");
    sprintf(ev.fStat_evLogT0.name,  "logT0");
    sprintf(ev.fStat_evLogT1.name,  "logT1");
    sprintf(ev.fStat_evLogEv.name,  "logEv");
    sprintf(ev.fStat_evLogPdp.name, "logPdp");

//        taskDelay(800000);
    for(i = 0; i < IIC_maxDrv; i++)   
    {
      /* Open the device. */
      iic_fd[i] = open(drvName[i], O_RDWR);
      
      if(iic_fd[i] < 0)
        prnErrRet();

      /* set configuration */
      iic_piic[i]   = piicBus[i];
      iic_isOpen[i] = 1;
      iic_slvNum[i] = iicSlvNum[i];
    }

#ifdef __DEBUG_1__
    iic_prnInfo();
#endif

    struct termios newtio;
    lcd_fd = open( DRV_NAME_lcd, O_RDWR | O_NOCTTY );
    ifRet(lcd_fd<0);
		memset( &newtio, 0, sizeof(newtio) );
    newtio.c_cflag = B115200;
    newtio.c_cflag |= CS8;
    newtio.c_cflag |= CLOCAL;
    newtio.c_cflag |= CREAD;
    newtio.c_iflag = IGNPAR;
    newtio.c_oflag = 0;
    newtio.c_lflag = 0;
    newtio.c_cc[VTIME] = 0;
    newtio.c_cc[VMIN] = 0;
 
    tcflush(lcd_fd, TCIFLUSH );
    tcsetattr(lcd_fd, TCSANOW, &newtio );

    prnM2("init_ip ok..\n");

    return RET_OK;
  };



























//==============================================================================
//----===@ set_fanSpeed
// Parameters  :
// Description :
//==============================================================================
  int timingDriver::set_fanSpeed(char pwmFront, char pwmRear ) // pwmIntensity
  {
    unsigned char txBuf;
    uint txReg;
    int bytesWrite;

    // Outputs intensity P9, P8 
    txReg = 0x14; txBuf = ((pwmFront & 0x0F)<<4) | (pwmRear & 0x0F);
    bytesWrite = iic_wr(IIC_BUS_bp, SLV_MAX7313_A, txReg, 1, (char*)&txBuf, 1 );
    if(bytesWrite != 1)
      prnErrRet();

    return RET_OK;
  };



//==============================================================================
//----===@ set_fp_led
// Parameters  :
// Description : front panel LED control function [if master]
//==============================================================================
  int timingDriver::set_fp_led(unsigned char pwrOK, unsigned char fanERR, unsigned char epicsOK, unsigned char timingNetworkOK)
  {
    ifRet(pwrOK > 1);
    ifRet(fanERR > 1);
    ifRet(epicsOK > 1);
    ifRet(timingNetworkOK > 1);

    unsigned char txBuf;
    uint txReg;
    int bytesWrite;

    txReg = 0x02;	
    txBuf = pwrOK | (fanERR<<1) | (epicsOK<<2) | (timingNetworkOK<<3) ;
    bytesWrite = iic_wr(IIC_BUS_bp, SLV_MAX7313_A, txReg, 1, (char*)&txBuf, 1 );
    if(bytesWrite != 1) 
      prnErrRet();	

    return RET_OK;
  }

















//==============================================================================
//----===@ set_gtp
// Parameters  :
// Description :
//==============================================================================
  int timingDriver::set_gtp(uint setCommand, uint setDelay)
  {
    ifRet(gtp.fd < 0);

    ip_wr(gtp.fd, A_setGtp_reset, setCommand);
    taskDelay(setDelay);
    ip_wr(gtp.fd, A_setGtp_reset, 0);

    return RET_OK;
  };

//==============================================================================
//----===@ set_evg
// Parameters  :
// Description :
//==============================================================================
  int timingDriver::set_evg(uint setCommand, uint setDelay)
  {
    static uint isRunning = 0;

    ifRet(isRunning == 1);
    ifRet(ev.fd < 0);
    isRunning = 1;

    ip_wr(ev.fd, A_setSysEvg, setCommand);
    taskDelay(setDelay);
    ip_wr(ev.fd, A_setSysEvg, 0);

    isRunning = 0;
    return RET_OK;
  };

//==============================================================================
//----===@ set_evr
// Parameters  :
// Description :
//==============================================================================
  int timingDriver::set_evr(uint setCommand, uint setDelay)
  {
    ifRet(ev.fd < 0);

    ip_wr(ev.fd, A_setEvr, setCommand);
    taskDelay(setDelay);
    ip_wr(ev.fd, A_setEvr, 0);

    return RET_OK;
  };


//==============================================================================
//----===@ set_evUTrg
// Parameters  :
// Description :
//==============================================================================
  int timingDriver::set_evUTrg(uint setDelay)
  {
    return set_evg(SET_usrTrg, setDelay);
  };

//==============================================================================
//----===@ set_evSeq_Resume
// Parameters  :
// Description :
//==============================================================================
  int timingDriver::set_evSeq_Resume(uint seqN)
  {
    ifRet(seqN>1);

    return set_evg(SET_seqTrgA_resume >> seqN, 10);
  };
















//==============================================================================
//----===@ 
// Parameters  :
// Description :
//==============================================================================
  int timingDriver::set_sys_IO(uint lsbV, uint msbV)
  {
    ifRet(ev.fd < 0);
    ip_wr(ev.fd, A_cfgSys_IO_LSB, lsbV);
    ip_wr(ev.fd, A_cfgSys_IO_MSB, msbV);
    return RET_OK;
  };

//==============================================================================
//----===@ set_sys_FMC_LED
// Parameters  : link0 high:Red low:Green /link1 high:Red low:Green
// Description :
//==============================================================================
  int timingDriver::set_sys_FMC_LED(uint link0, uint link1) 
  {
    ifRet(ev.fd < 0);
    ip_wr(ev.fd, A_cfgSys_FMC, (link0 & 0x1) | ((link1 & 0x1)<<1));
    return RET_OK;
  };

//==============================================================================
//----===@ 
// Parameters  :
// Description :
    //evg control config
    // igen_enable            <= `DLY slv_reg34[1];
    // igen_mode_ext          <= `DLY slv_reg34[0];  // 0 : internal irigb gen, 1 : external irigb
    // igen_set_y7            <= `DLY slv_reg34[31]; 
    // igen_set_d9            <= `DLY slv_reg35[31:23]; 
    // igen_set_y7[5:0]       <= `DLY slv_reg35[22:17]; 
    // igen_set_h5            <= `DLY slv_reg35[16:12]; 
    // igen_set_m6            <= `DLY slv_reg35[11: 6]; 
    // igen_set_s6            <= `DLY slv_reg35[ 5: 0]; 
//==============================================================================
  int timingDriver::set_evg_igen(uint igen_enable, uint igen_mode_ext, struct s_evTime igenTime)
  {
    ifRet(ev.fd < 0);

    uint wrData = 0;

    wrData |= ((igenTime.year & 0x0000003F) << 17); // 17
    wrData |= ((igenTime.day  & 0x000001FF) << 27); // 27 bit
    wrData |= ((igenTime.hour & 0x0000001F) << 12); // 12
    wrData |= ((igenTime.min  & 0x0000003F) <<  6); // 6
    wrData |= ((igenTime.sec  & 0x0000003F) <<  0); // 0

    ip_wr(ev.fd, A_cfgEvg_igen_0, igen_mode_ext | (igen_enable << 1) | ((igenTime.year & 0x00000040) << 25) );
    ip_wr(ev.fd, A_cfgEvg_igen_1, wrData);
    return RET_OK;
  };

//==============================================================================
//----===@ 
// Parameters  :
// Description :
//==============================================================================
  int timingDriver::set_evg_sw_evgW(uint idec_mode, struct s_evTime igenTime)
  {
    ifRet(ev.fd < 0);
    ip_wr(ev.fd, A_cfgEvg_sw_evgW0, 0);
    ip_wr(ev.fd, A_cfgEvg_sw_evgW1, 0);
    return RET_OK;
  };

//==============================================================================
//----===@ 
// Parameters  :
// Description :
//==============================================================================
  int timingDriver::set_evg_enable(uint dbus, uint evCodeA, uint evCodeB, uint mevCodeA, uint seqTrgA, uint seqTrgB, uint mxc)
  {
    ifRet(ev.fd < 0);

    ifRet(dbus     > 1);
    ifRet(evCodeA  > 1);
    ifRet(evCodeB  > 1);
    ifRet(mevCodeA > 1);
    ifRet(seqTrgA  > 1);
    ifRet(seqTrgB  > 1);
    ifRet(mxc > 1);

    ev.evg_txEn_DBus     = dbus     ; // slv_reg38[31];
    ev.evg_txEn_evCodeA  = evCodeA  ; // slv_reg38[30];
    ev.evg_txEn_evCodeB  = evCodeB  ; // slv_reg38[29];
    ev.evg_txEn_mevCodeA = mevCodeA ; // slv_reg38[28];

    ev.evg_seqTrgA_en  = seqTrgA ; // slv_reg38[27];
    ev.evg_seqTrgB_en  = seqTrgB ; // slv_reg38[26];
    ev.evg_mxc14Enable = mxc     ; // slv_reg38[25];

    ip_wr(ev.fd, A_cfgEvg_enable, (dbus<<31) | (evCodeA<<30) | (evCodeB<<29) | (mevCodeA<<28)  | (seqTrgA<<27)  | (seqTrgB<<26)  | (mxc<<25) );

    return RET_OK;
  };


//==============================================================================
//----===@ 
// Parameters  :
// Description :
//==============================================================================
  int timingDriver::set_evg_tx_DBusSel(uint dBusNum, ev_src evSrc)
  {
    ifRet(ev.fd < 0);
    ifRet(dBusNum > 7);
    ifRet(evSrc > EV_SRC_MAX-1);
    
    ev.tx_DBusSel[dBusNum] = evSrc;
    prnM1("set_evg_tx_DBusSel : 0x%08x\r\n",(uint)(dBusNum<<8) | (evSrc & 0x1f));
    ip_wr(ev.fd, A_cfgEvg_tx_DBusSel, (uint)(dBusNum<<8) | (evSrc & 0x1f));
    set_evg(SET_tx_DBusSel_set, 1);
    
    return RET_OK;
  };

//==============================================================================
//----===@ set_evg_evTrg
// Parameters  : evTrgNum 1~15
// Description :
//==============================================================================
  int timingDriver::set_evg_evTrg(uint evTrgNum, ev_src evSrc)
  {
    ifRet(ev.fd < 0);
    ifRet(evTrgNum > 30);
    ifRet(evSrc > EV_SRC_MAX-1);
    
    ev.evTrg[evTrgNum] = evSrc;
    prnM1("set_evg_evTrg : 0x%08x\r\n",(uint)(evTrgNum<<8) | (evSrc & 0x1f));
    ip_wr(ev.fd, A_cfgEvg_evTrgSel, (evTrgNum<<8) | (evSrc & 0x1f));
    set_evg(SET_evSrc_set, 1);

    return RET_OK;
  };


//==============================================================================
//----===@ set_evg_evTrg
// Parameters  :
// Description :
//==============================================================================
  int timingDriver::set_evg_evTrgMask(uint evTrgMask)
  {
    ifRet(ev.fd < 0);
    ip_wr(ev.fd, A_cfgEvg_evTrgMsk, evTrgMask & 0x7fffffff);    
    return RET_OK;
  };


//==============================================================================
//----===@ cfg_evg_seqSrc
// Parameters  : seqSrc_OR_Num : 0~3, A_cfgEvg_seqTrgMskA0, A1, B0, B1
//               seqSrcList    : seq trigger source OR value 
//                                 seqS_usrTrg, seqS_mxc00~13, seqS_ext00~15
// Description :
//==============================================================================
  int timingDriver::cfg_evg_seqSrc(uint seqSrc_OR_Num, uint seqSrcList)
  {
    ifRet(ev.fd < 0);
    ifRet(seqSrc_OR_Num > 3);

    ev.seqSrcOR[seqSrc_OR_Num] = seqSrcList;
    return RET_OK;
  };


//==============================================================================
//----===@ 
// Parameters  :
// Description :
//==============================================================================
  int timingDriver::set_evg_mxcFreq(uint mxcNum, float mxcFreq_Hz) 
  {
    ifRet(ev.fd < 0);
    ifRet(mxcNum > MAX_MXC_NUM-1);
    ifRet(mxcFreq_Hz > TS2_REF_CLK_FREQ/2);

    uint wrData;

    //81.250000 MHz
    ev.mxcFreq_Hz[mxcNum] = mxcFreq_Hz;
//    ip_wr(ev.fd, A_cfgEvg_mxcFreq_00 + mxcNum*4, mxcVal);


    wrData = (uint)(TS2_REF_CLK_FREQ / ev.mxcFreq_Hz[mxcNum]);
    prnM1("set_evg_mxcFreq [%d] : %f Hz, %d x12.3ns clock period.\r\n",mxcNum,mxcFreq_Hz,wrData);
    ip_wr(ev.fd, A_cfgEvg_mxcFreq_00 + mxcNum*4, wrData);

    return RET_OK;
  };


//==============================================================================
//----===@ 
// Parameters  :
// Description :
//==============================================================================
  int timingDriver::set_evr_enable(uint dbus, uint evCodeA, uint evCodeB, uint streamD, uint pdp )
  {
    ifRet(ev.fd < 0);

    ifRet(dbus     > 1);
    ifRet(evCodeA  > 1);
    ifRet(evCodeB  > 1);
    ifRet(streamD  > 1);
    ifRet(pdp      > 1);

    ev.evr_rxEn_DBus       = dbus    ;
    ev.evr_rxEn_evCodeA    = evCodeA ;
    ev.evr_rxEn_evCodeB    = evCodeB ;
    ev.evr_rxEn_streamData = streamD ;
    ev.evr_pdpEn           = pdp     ;

    ip_wr(ev.fd, A_cfgEvr_enable, (dbus << 31) | (evCodeA << 30) | (evCodeB << 29) | (streamD << 27) | pdp );
    return RET_OK;
  };







//==============================================================================
//----===@ 
// Parameters  :
// Description :
//==============================================================================
  int timingDriver::cfg_evr_pdp(uint pdpNum, uint delay, uint width, uint polarity)
  {
    ifRet(ev.fd < 0);
    ifRet(pdpNum > 31);
    ifRet(polarity > 1);

    ev.pdp[pdpNum].delay = delay;
    ev.pdp[pdpNum].width = width;
    ev.pdp[pdpNum].polarity = polarity;

    return RET_OK;
  };


//==============================================================================
//----===@ cfg_evr_outSrcSel
// Parameters  :
// Description :
//==============================================================================
  int timingDriver::cfg_evr_outSrcSel(ev_out outPortNum, ev_out_src evOutSrc)
  {
    ifRet(ev.fd < 0);
    ifRet(outPortNum > EV_OUT_MAX-1);
    ifRet(evOutSrc > EV_OUT_SRC_MAX-1);
    
    int setState = RET_ERR;

    switch(outPortNum)
    {
      case 0  :  if((evOutSrc == RXDBUS_00) || (evOutSrc == PDP_OUT_00)) setState = RET_OK; break;
      case 1  :  if((evOutSrc == RXDBUS_01) || (evOutSrc == PDP_OUT_01)) setState = RET_OK; break;
      case 2  :  if((evOutSrc == RXDBUS_02) || (evOutSrc == PDP_OUT_02)) setState = RET_OK; break;
      case 3  :  if((evOutSrc == RXDBUS_03) || (evOutSrc == PDP_OUT_03)) setState = RET_OK; break;
      case 4  :  if((evOutSrc == RXDBUS_04) || (evOutSrc == PDP_OUT_04)) setState = RET_OK; break;
      case 5  :  if((evOutSrc == RXDBUS_05) || (evOutSrc == PDP_OUT_05)) setState = RET_OK; break;
      case 6  :  if((evOutSrc == RXDBUS_06) || (evOutSrc == PDP_OUT_06)) setState = RET_OK; break;
      case 7  :  if((evOutSrc == RXDBUS_07) || (evOutSrc == PDP_OUT_07)) setState = RET_OK; break;
      case 8  :  if(evOutSrc == PDP_OUT_08) setState = RET_OK; break;
      case 9  :  if(evOutSrc == PDP_OUT_09) setState = RET_OK; break;
      case 10 :  if(evOutSrc == PDP_OUT_10) setState = RET_OK; break;
      case 11 :  if(evOutSrc == PDP_OUT_11) setState = RET_OK; break;
      case 12 :  if(evOutSrc == PDP_OUT_12) setState = RET_OK; break;
      case 13 :  if(evOutSrc == PDP_OUT_13) setState = RET_OK; break;
      case 14 :  if(evOutSrc == PDP_OUT_14) setState = RET_OK; break;
      case 15 :  if(evOutSrc == PDP_OUT_15) setState = RET_OK; break;
      case 16 :  if(evOutSrc == PDP_OUT_16) setState = RET_OK; break;
      case 17 :  if(evOutSrc == PDP_OUT_17) setState = RET_OK; break;
      case 18 :  if(evOutSrc == PDP_OUT_18) setState = RET_OK; break;
      case 19 :  if(evOutSrc == PDP_OUT_19) setState = RET_OK; break;
      case 20 :  if(evOutSrc == PDP_OUT_20) setState = RET_OK; break;
      case 21 :  if(evOutSrc == PDP_OUT_21) setState = RET_OK; break;
      case 22 :  if(evOutSrc == PDP_OUT_22) setState = RET_OK; break;
      case 23 :  if(evOutSrc == PDP_OUT_23) setState = RET_OK; break;
      case 24 :  if(evOutSrc == PDP_OUT_24) setState = RET_OK; break;
      case 25 :  if(evOutSrc == PDP_OUT_25) setState = RET_OK; break;
      case 26 :  if(evOutSrc == PDP_OUT_26) setState = RET_OK; break;
      case 27 :  if(evOutSrc == PDP_OUT_27) setState = RET_OK; break;
      case 28 :  if(evOutSrc == PDP_OUT_28) setState = RET_OK; break;
      case 29 :  if(evOutSrc == PDP_OUT_29) setState = RET_OK; break;
      case 30 :  if(evOutSrc == PDP_OUT_30) setState = RET_OK; break;
      case 31 :  if((evOutSrc == evrOut_1pps) || (evOutSrc == PDP_OUT_31)) 
                 {
                   if(evOutSrc == evrOut_1pps){
                     ev.exOut31_1pps = 1;
                   }
                   else{
                     ev.exOut31_1pps = 0;
                   }
                   setState = RET_OK;
                   break;
                 }
      default   : break;
    }
    
    if(setState == RET_OK)
    {
      prnM1("  cfg_evr_outSrcSel [outPortNum] %d - [evOutSrc] %s.\r\n",outPortNum, evOutSrcName[evOutSrc]);
      ev.evOut[outPortNum] = evOutSrc;
    }

    return setState;
  };

//==============================================================================
//----===@ 
// Parameters  :
// Description :
//==============================================================================
  int timingDriver::set_evr_evLogFifo_Enable (uint enVal)
  {
    ifRet(ev.fd < 0);
    ifRet(enVal > 1);
    ev.evLogFifo_En_user = enVal;
    ip_wr(ev.fd, A_cfgEvr_evLogFifo_Enable, enVal);
    return RET_OK;
  };











//==============================================================================
//----===@ set_evUser
// Parameters  :
// Description :
//==============================================================================
  int timingDriver::set_evUser(uint evCode )
  {
    ifRet(ev.fd < 0);
    ip_wr(ev.fd, A_set_evUser, evCode);
    return RET_OK;
  };









//==============================================================================
//----===@ set_evg_seqTrg
// Parameters  : seqX_TrgCmbSrc : 0~7 
//                                0 : evg_seq disable
//                                1 : seqSrcOR[0]
//                                2 : seqSrcOR[1]
//                                3 : seqSrcOR[0] & seqSrcOR[1]
//                                4 : evg_seq disable
//                                5 : seqSrcOR[2]
//                                6 : seqSrcOR[3]
//                                7 : seqSrcOR[2] & seqSrcOR[3]
// Description :
//==============================================================================
  int timingDriver::set_evg_seqTrg(uint seqA_TrgCmbSrc, uint seqA_repeat, uint seqB_TrgCmbSrc, uint seqB_repeat)
  {
    ifRet(ev.fd < 0);
    ifRet(seqA_TrgCmbSrc > seqS_CMB_OR2_and_OR3);
    ifRet(seqB_TrgCmbSrc > seqS_CMB_OR2_and_OR3);
    
    ev.seqTrgCmbSrcSel[0] = seqA_TrgCmbSrc;
    ev.seqTrgCmbSrcSel[1] = seqB_TrgCmbSrc;

    ev.seq_repeatReg[0] = seqA_repeat;
    ev.seq_repeatReg[1] = seqB_repeat;

    ip_wr(ev.fd, A_cfgEvg_seqTrgMskA0, ev.seqSrcOR[0] & 0x7fffffff);
    ip_wr(ev.fd, A_cfgEvg_seqTrgMskA1, ev.seqSrcOR[1] & 0x7fffffff);
    ip_wr(ev.fd, A_cfgEvg_seqTrgMskB0, ev.seqSrcOR[2] & 0x7fffffff);
    ip_wr(ev.fd, A_cfgEvg_seqTrgMskB1, ev.seqSrcOR[3] & 0x7fffffff);

    ip_wr(ev.fd, A_cfgEvg_seqTrgSel, (ev.seqTrgCmbSrcSel[0] & 0xf) | ((ev.seqTrgCmbSrcSel[1] & 0xf)<<4) ) ;
    
    ip_wr(ev.fd, A_cfgEvg_seqA_repeatReg, ev.seq_repeatReg[0]);
    ip_wr(ev.fd, A_cfgEvg_seqB_repeatReg, ev.seq_repeatReg[1]);
    return RET_OK;
  };



//==============================================================================
//----===@ set_evg_seqTable
// Parameters  : seqNum 0-A, 1-B
// Description : 
//==============================================================================
  int timingDriver::set_evg_seqTable(uint seqNum, uint offset, uint evCode, uint stopbit, uint timeStamp)
  {
    ifRet(ev.fd < 0);
    ifRet(seqNum > 1);
    ifRet(evCode > 0x7ff);
    ifRet(stopbit > 1);

    uint seqMemOffset;
    
    if(seqNum == 0)
      seqMemOffset = 0;
    else
      seqMemOffset = 0x4000;

    ip_wr(ev.fd, Mem_seqA_Config + seqMemOffset + (offset*4), C_seq_stop(stopbit) | C_seq_evCodeB(evCode));
    ip_wr(ev.fd, Mem_seqA_Config + seqMemOffset + 0x2000 + (offset*4), timeStamp);

    return RET_OK;
  }

//==============================================================================
//----===@ 
// Parameters  : 
// Description : 
//==============================================================================
  int timingDriver::set_evg_mevCodeA(uint trgNum, uint evCode)
  {
    ifRet(ev.fd < 0);
    ifRet(trgNum > 30);
    ifRet(evCode > 0x7ff);

    ip_wr(ev.fd, Mem_evCodeA + (trgNum+1) * 4 , evCode);
    return RET_OK;
  }
















//==============================================================================
//----===@ set_evr_pdpAndOut
// Parameters  : pdpTrgMode   : trigger from 1-evCodeA or 0-evCodeB]
//               extOutSwap   : swap sl <-> sh
// Description :
//==============================================================================
  int timingDriver::set_evr_pdpAndOut(uint pdpTrgMode, uint extOutSwap )
  {
    ifRet(ev.fd < 0);
    ifRet(pdpTrgMode > 1);
    ifRet(extOutSwap > 1);

    int i;
    uint wrData;
    uint wrData_bit;

    wrData = 0 ;
    wrData_bit = 1;

    ev.pdpTrgMode   = pdpTrgMode;
    ev.extOutSwap   = extOutSwap;

    for(i=0;i<MAX_PDP_NUM;i++)
    {
      ip_wr(ev.fd, MemC_EVR_pdpDelay + 4 * i, ev.pdp[i].delay);
      ip_wr(ev.fd, MemC_EVR_pdpWidth + 4 * i, ev.pdp[i].width);

      if(ev.pdp[i].polarity == 1) 
        wrData |= wrData_bit;

      wrData_bit = wrData_bit << 1;
    }
    
    set_evr(SET_evr_setCtrlReg, 1000);
    ip_wr(ev.fd, A_cfgEvr_pdpPolarity, wrData);
    // ip_wr(ev.fd, A_cfgEvr_pdpPolarity, 0);
    
    wrData = 0 ;
    wrData_bit = 1;

    for(i=0;i<8;i++)
    {
      if(ev.evOut[i] > RXDBUS_07) 
        wrData |= wrData_bit;

      wrData_bit = wrData_bit << 1;
    }

    wrData = wrData | (ev.pdpTrgMode << 8) | (ev.extOutSwap << 31) | (((!ev.exOut31_1pps) & 1) << 30); 
    prnM1("set_evr_pdpAndOut evOut wrData : 0x%08x.\r\n",wrData);
    
    ip_wr(ev.fd, A_cfgEvr_rx_DBusSel, wrData);

    return RET_OK;
  };






//==============================================================================
//----===@ 
// Parameters  : 
// Description : 
//==============================================================================
  int timingDriver::set_evr_evRam(uint evCode, uint mapData)
  {
    ifRet(ev.fd < 0);
    ifRet(evCode > 0x7ff);

    ip_wr(ev.fd, MemA_EVR_0 + evCode * 4 , mapData);
    return RET_OK;
  }




































//==============================================================================
//----===@ get_slaveBoardID
// Parameters  :
// Description : get slave board ID 1st & 2nd floor
//==============================================================================
  int timingDriver::get_slaveBoardID()
  {
    unsigned char rxData[2];
    int bytesRead;

    bytesRead = iic_rd(IIC_BUS_sl, SLV_MAX7313_A, 0, 1, (char*)rxData, sizeof(rxData));
    if(!bytesRead)
      prnErrRet();  

    sl.id = rxData[0] & 0x0f;

    bytesRead = iic_rd(IIC_BUS_sh, SLV_MAX7313_A, 0, 1, (char*)rxData, sizeof(rxData));
    if(!bytesRead)
      prnErrRet();  

    sh.id = rxData[0] & 0x0f;

    return RET_OK;
  };

//==============================================================================
//----===@ get_slaveTemp
// Parameters  :
// Description : get slave board ID 1st & 2nd floor
//==============================================================================
  int timingDriver::get_slaveTemp()
  {
    unsigned char rxData[2];
    int bytesRead;

    rxData[0]=0;
    rxData[1]=0;
    bytesRead = iic_rd(IIC_BUS_sl, SLV_TMP112A, 0, 1, (char*)rxData, sizeof(rxData));
    if(!bytesRead)
      prnErrRet();	
    sl.temp = (((float)rxData[0] *0.0625) *16) + ((float)(rxData[1] >> 4)*0.0625);

    rxData[0]=0;
    rxData[1]=0;
    bytesRead = iic_rd(IIC_BUS_sh, SLV_TMP112A, 0, 1, (char*)rxData, sizeof(rxData));
    if(!bytesRead)
      prnErrRet();	
    sh.temp = (((float)rxData[0] *0.0625) *16) + ((float)(rxData[1] >> 4)*0.0625);
    return RET_OK;
  };



//==============================================================================
//----===@ get_ipInfo
// Parameters  :
// Description :
//==============================================================================
  int timingDriver::get_ipInfo(int l_fd, struct s_ipInfo *pInfo)
  {
    ifRet(pInfo < 0);
    
    uint rdData;

    ip_rd(l_fd, A_ipSys_clkCntr, &rdData);  
    pInfo->ipTick = rdData;

    ip_rd(l_fd, A_ipSys_vendor, &rdData); 
    pInfo->vendor = rdData;

    ip_rd(l_fd, A_ipSys_info, &rdData);
    pInfo->ipID            = ((rdData >>  24) & 0xff);
    pInfo->ipID_sub        = ((rdData >>  16) & 0xff);  
    pInfo->ipVer_major     = ((rdData >>  12) & 0xf);
    pInfo->ipVer_miner     = ((rdData >>  4) & 0xff);
    pInfo->ipVer_rev       = ((rdData >>  0) & 0xf);

    switch(pInfo->ipID)
    {
      case 0x40 : sprintf(pInfo->name,"gtp"); break;
      case 0x41 : sprintf(pInfo->name,"ev"); break;
      default   : sprintf(pInfo->name,"unknown"); break;
    }


    ip_rd(l_fd, A_ipSys_buildTime, &rdData);
    pInfo->buildTime.year  = ((rdData & 0x007E0000) >> 17); // 17
    pInfo->buildTime.month = ((rdData & 0x07800000) >> 23); // 23
    pInfo->buildTime.day   = ((rdData & 0xF8000000) >> 27); // 27 bit
    pInfo->buildTime.hour  = ((rdData & 0x0001F000) >> 12); // 12
    pInfo->buildTime.min   = ((rdData & 0x00000FC0) >>  6); // 6
    pInfo->buildTime.sec   = ((rdData & 0x0000003F) >>  0); // 0

    return RET_OK;
  }


//==============================================================================
//----===@ get_gtpStat
// Parameters  :
// Description :
//==============================================================================
  int timingDriver::get_gtpStat()
  {
    int l_fd = gtp.fd;
    uint rdData;

    ifRet(l_fd < 0);

    ip_rd(l_fd, A_sttGtp_ip, &rdData);  
    gtp.reset                    = ((rdData >>  0) & 0x1) ;

    gtp.stat[0].cpllfbclklost    = ((rdData >>  8) & 0x1) ;
    gtp.stat[0].cplllock         = ((rdData >>  9) & 0x1) ;
    gtp.stat[0].txresetdone      = ((rdData >> 12) & 0x1) ;
    gtp.stat[0].txfsmresetdone   = ((rdData >> 13) & 0x1) ;
    gtp.stat[0].rxresetdone      = ((rdData >> 14) & 0x1) ;
    gtp.stat[0].rxfsmresetdone   = ((rdData >> 15) & 0x1) ;
    gtp.stat[0].rxDataError      = ((rdData >> 16) & 0x1) ;
    gtp.stat[0].trackOK          = ((rdData >> 18) & 0x1) ;
    gtp.stat[0].track_data       = ((rdData >> 19) & 0x1) ;

    gtp.stat[1].cpllfbclklost    = ((rdData >> 20) & 0x1) ;
    gtp.stat[1].cplllock         = ((rdData >> 21) & 0x1) ;
    gtp.stat[1].txresetdone      = ((rdData >> 24) & 0x1) ;
    gtp.stat[1].txfsmresetdone   = ((rdData >> 25) & 0x1) ;
    gtp.stat[1].rxresetdone      = ((rdData >> 26) & 0x1) ;
    gtp.stat[1].rxfsmresetdone   = ((rdData >> 27) & 0x1) ;
    gtp.stat[1].rxDataError      = ((rdData >> 28) & 0x1) ;
    gtp.stat[1].trackOK          = ((rdData >> 30) & 0x1) ;
    gtp.stat[1].track_data       = ((rdData >> 31) & 0x1) ;

    ip_rd(l_fd, A_sttGtp_trackLoss, &rdData); 
    gtp.stat[0].trackLossCnt     = ((rdData >>  0) & 0xffff)    ;
    gtp.stat[1].trackLossCnt     = ((rdData >> 16) & 0xffff)    ;

    ip_rd(l_fd, A_sttGtp_gt0clk_cntr, &rdData); 
    gtp.stat[0].txClk_cntr       = ((rdData >>  0) & 0xffff)      ;
    gtp.stat[0].rxClk_cntr       = ((rdData >> 16) & 0xffff)      ;

    ip_rd(l_fd, A_sttGtp_gt1clk_cntr, &rdData); 
    gtp.stat[1].txClk_cntr       = ((rdData >>  0) & 0xffff)      ;
    gtp.stat[1].rxClk_cntr       = ((rdData >> 16) & 0xffff)      ;

    ip_rd(l_fd, A_sttGtp_chkDelayCnt, &rdData); 
    gtp.chkDelayCnt              = rdData      ;

    return RET_OK;
  }

//==============================================================================
//----===@ get_sysStat
// Parameters  :
// Description :
//==============================================================================
  int timingDriver::get_sysStat()
  {
    int l_fd = ev.fd;
    uint rdData;

    ifRet(l_fd < 0);

    //system status
    ip_rd(l_fd, A_ifanStt4_CntrFront, &rdData);
    ev.fanA = ((rdData >> 16) & 0xffff);
    ev.fanB = ((rdData >>  0) & 0xffff);

    ip_rd(l_fd, A_ifanStt4_CntrRear, &rdData);
    ev.fanC = ((rdData >> 16) & 0xffff);
    ev.fanD = ((rdData >>  0) & 0xffff);

    ip_rd(l_fd, A_boardStat, &rdData);
    ev.isMaster       = (((~rdData) >> 31) & 0x1   );
    ev.PRSNT_M2C_L    = (((~rdData) >> 28) & 0x1)   ;
    ev.f_SFP_LossA    = ((rdData >>  1) & 0x1 )     ;
    ev.f_SFP_LossB    = ((rdData >>  0) & 0x1 )     ;
    ev.f_SFP_prsntA   = (((~rdData) >>  4) & 0x1 )  ;
    ev.f_SFP_prsntB   = (((~rdData) >>  5) & 0x1 )  ;

    return RET_OK;
  }


//==============================================================================
//----===@ get_fifoStat
// Parameters  :
// Description :
//==============================================================================
  int timingDriver::get_fifoStat(int l_fd, uint addr, struct s_fifoStat *fStat)
  {
    uint rdData;
    ifRet(l_fd < 0);

    ip_rd(l_fd, addr, &rdData);
    fStat->WRCOUNT     = ((rdData >> 16) & 0x3ff );
    fStat->RDCOUNT     = ((rdData >>  6) & 0x3ff );
    fStat->RDERR       = ((rdData >>  5) & 0x1 );
    fStat->WRERR       = ((rdData >>  4) & 0x1 );
    fStat->EMPTY       = ((rdData >>  3) & 0x1 );
    fStat->ALMOSTEMPTY = ((rdData >>  2) & 0x1 );
    fStat->FULL        = ((rdData >>  1) & 0x1 );
    fStat->ALMOSTFULL  = ((rdData >>  0) & 0x1 );

    return RET_OK;
  }

//==============================================================================
//----===@ get_evgStat
// Parameters  :
// Description :
//==============================================================================
  int timingDriver::get_evgStat()
  {
    int l_fd = ev.fd;
    uint rdData;

    ifRet(l_fd < 0);

    //evg status
    ip_rd(l_fd, A_seqTrg_runState, &rdData);
    ev.idec_status    = ((rdData >> 30) & 0x1   );
    ev.seqA_isRunning = ((rdData >>  0) & 0x3   );
    ev.seqB_isRunning = ((rdData >>  4) & 0x3   );

    get_fifoStat(l_fd, A_fifo_evCodeA_STAT, &ev.fStat_evCodeA);
    get_fifoStat(l_fd, A_fifo_seqA_STAT, &ev.fStat_seqA);
    get_fifoStat(l_fd, A_fifo_seqB_STAT, &ev.fStat_seqB);
    get_fifoStat(l_fd, A_fifo_evLogT0_STAT, &ev.fStat_evLogT0);
    get_fifoStat(l_fd, A_fifo_evLogT1_STAT, &ev.fStat_evLogT1);
    get_fifoStat(l_fd, A_fifo_evLogEv_STAT, &ev.fStat_evLogEv);
    get_fifoStat(l_fd, A_fifo_evLogPdp_STAT, &ev.fStat_evLogPdp);
    get_fifoStat(l_fd, A_fifo_evUser_STAT, &ev.fStat_evUser);

    ip_rd(l_fd, A_tx_evCode_cntr, &rdData);
    ev.tx_evCodeA_cntr = ((rdData >> 16) & 0xffff);
    ev.tx_evCodeB_cntr = ((rdData >>  0) & 0xffff);

    ip_rd(l_fd, A_evrW_evgW_set_terr, &rdData);
    ev.evrW_set_terr_cnt = ((rdData >> 16) & 0xffff);
    ev.evgW_set_terr_cnt = ((rdData >>  0) & 0xffff);

    return RET_OK;
  }


//==============================================================================
//----===@ get_evTime
// Parameters  :
// Description :
//==============================================================================
  int timingDriver::get_evTime()
  {
    int l_fd = ev.fd;
    uint rdData;

    ifRet(l_fd < 0);
    set_evg(SET_getTime, 0);

    ip_rd(l_fd, A_evgW_time0, &rdData);
    ev.evgTime.year  = ((rdData & 0x007E0000) >> 17); // 17
    ev.evgTime.day   = ((rdData & 0xFF800000) >> 27); // 27 bit
    ev.evgTime.hour  = ((rdData & 0x0001F000) >> 12); // 12
    ev.evgTime.min   = ((rdData & 0x00000FC0) >>  6); // 6
    ev.evgTime.sec   = ((rdData & 0x0000003F) >>  0); // 0

    ip_rd(l_fd, A_evgW_time1, &rdData);
    ev.evgTime.year  = ev.evgTime.year + ((rdData & 0x80000000) >> 25);
    ev.evgTime.ms    = ((rdData & 0x3FF00000) >> 20); // 0
    ev.evgTime.tick  = ((rdData & 0x000FFFFF) >>  0); // 0

    ip_rd(l_fd, A_evrW_time0           , &rdData);
    ev.evrTime.year  = ((rdData & 0x007E0000) >> 17); // 17
    ev.evrTime.day   = ((rdData & 0xFF800000) >> 27); // 27 bit
    ev.evrTime.hour  = ((rdData & 0x0001F000) >> 12); // 12
    ev.evrTime.min   = ((rdData & 0x00000FC0) >>  6); // 6
    ev.evrTime.sec   = ((rdData & 0x0000003F) >>  0); // 0

    ip_rd(l_fd, A_evrW_time1           , &rdData);
    ev.evrTime.year  = ev.evrTime.year + ((rdData & 0x80000000) >> 25);
    ev.evrTime.ms    = ((rdData & 0x3FF00000) >> 20); // 0
    ev.evrTime.tick  = ((rdData & 0x000FFFFF) >>  0); // 0

    return RET_OK;
  }


//==============================================================================
//----===@ get_evrStat
// Parameters  :
// Description :
//==============================================================================
  int timingDriver::get_evrStat()
  {
    int l_fd = ev.fd;
    uint rdData;
    int i;

    ifRet(l_fd < 0);
    set_evr(SET_evr_getStatReg, 1);

    //evr status
    ip_rd(l_fd, A_rx_evCode_cntr       , &rdData);
    ev.rx_evCodeA_cntr = ((rdData >> 16) & 0xffff);
    ev.rx_evCodeB_cntr = ((rdData >>  0) & 0xffff);

    ip_rd(l_fd, A_streamRx_evCode_cntr , &rdData);
    ev.streamRx_evCodeA_cntr = ((rdData >> 16) & 0xffff);
    ev.streamRx_evCodeB_cntr = ((rdData >>  0) & 0xffff);

    ip_rd(l_fd, A_emergencyStopCntr    , &rdData);
    ev.emergencyStopCntr     = ((rdData >>  0) & 0xffff);

    ip_rd(l_fd, A_pdpRunStt, &rdData);
    for(i=0;i<32;i++)
      ev.pdp[i].isRunning = (rdData >> i) & 0x00000001;

    for(i=0;i<32;i++)
    {
      ip_rd(l_fd, MemC_EVR_pdpStat + i*4, &rdData);
      ev.pdp[i].runCnt = ((rdData >> 16) & 0xffff);
      ev.pdp[i].errCnt = ((rdData >> 0 ) & 0xffff);
    }

    ip_rd(l_fd, A_evrRx_linkErr, &rdData);
    ev.evrRx_linkErr = rdData;

    ip_rd(l_fd, A_evrRx_linkErrCntr, &rdData);
    ev.evrRx_linkErrCntr = rdData;

    return RET_OK;
  }

//==============================================================================
//----===@ get_evLog
// Parameters  :
// Description :
//==============================================================================
  int timingDriver::prn_evLog()
  {
    int l_fd = ev.fd;
    uint rdData;
    int i;
    s_evTime logTime;
    uint rdEv;
    uint rdPdp;
    struct s_fifoStat fStat;

    ifRet(l_fd < 0);

    prnM2("======================================================\r\n");
    prnM2("++ prn_ipInfo ++\r\n");

    for(i=0;i<1024;i++)
    {
      get_fifoStat(l_fd, A_fifo_evLogPdp_STAT, &fStat);
      //prnM2("%-22s: %d\r\n","fStat.EMPTY",fStat.EMPTY);
      if(fStat.EMPTY == 0)
      {
        //evr status
        ip_rd(l_fd, A_get_evLogT0       , &rdData);
        logTime.year  = ((rdData & 0x007E0000) >> 17); // 17
        logTime.day   = ((rdData & 0xFF800000) >> 27); // 27 bit
        logTime.hour  = ((rdData & 0x0001F000) >> 12); // 12
        logTime.min   = ((rdData & 0x00000FC0) >>  6); // 6
        logTime.sec   = ((rdData & 0x0000003F) >>  0); // 0

        ip_rd(l_fd, A_get_evLogT1       , &rdData);
        logTime.year  = logTime.year + ((rdData & 0x80000000) >> 25);
        logTime.ms    = ((rdData & 0x3FF00000) >> 20); // 0
        logTime.tick  = ((rdData & 0x000FFFFF) >>  0); // 0


        ip_rd(l_fd, A_get_evLogEv       , &rdEv);
        ip_rd(l_fd, A_get_evLogPdp      , &rdPdp);

        prnM2("%-22s: %02d.%03d/%02d:%02d:%02d.%03d/%05d [0x%08x] [0x%08x]\r\n",\
                    "evLog"                 ,\
                    logTime.year          ,\
                    logTime.day           ,\
                    logTime.hour          ,\
                    logTime.min           ,\
                    logTime.sec           ,\
                    logTime.ms           ,\
                    logTime.tick         ,\
                    rdEv           ,\
                    rdPdp           );
      }
      else
      {
        break;
      }
    }



    return RET_OK;
  }






















//==============================================================================
//----===@ prn_ipInfo
// Parameters  :
// Description :
//==============================================================================
int timingDriver::prn_ipInfo(int l_fd, struct s_ipInfo *pInfo)
{
  ifRet(pInfo == NULL);

  get_ipInfo(l_fd, pInfo);

  prnM2("======================================================\r\n");
  prnM2("++ prn_ipInfo ++\r\n");
  prnM2("%-22s: %08x\r\n","vendor",pInfo->vendor);
  prnM2("%-22s: %s\r\n","ipID.name",pInfo->name);

  prnM2("%-22s: %02x.%02x\r\n"      ,\
              "ipID.sub"            ,\
              pInfo->ipID        ,\
              pInfo->ipID_sub );

  prnM2("%-22s: %02x.%02x%01x\r\n"   ,\
              "ipVer"                ,\
              pInfo->ipVer_major  ,\
              pInfo->ipVer_miner  ,\
              pInfo->ipVer_rev);
  
  prn_time(pInfo->buildTime);

  return RET_OK;
}

//==============================================================================
//----===@ prn_ip
// Parameters  :
// Description :
//==============================================================================
int timingDriver::prn_ip(int pLevel)
{
  ifRet(ev.fd  < 0);
  ifRet(gtp.fd < 0);
  if(pLevel > 2)
  {
    prn_ipInfo(ev.fd , &ev.info);
    prn_ipInfo(gtp.fd, &gtp.info);
  }

  prnM2("\r\n");
  return RET_OK;
}


//==============================================================================
//----===@ prn_time
// Parameters  :
// Description :
//==============================================================================
  int timingDriver::prn_time(struct s_evTime evTime)
  {
    prnM2("%-22s: %02d.", evTime.name, evTime.year);

    switch(evTime.type)
    {
      case 0 : prnM2("%02d.%02d-", evTime.month , evTime.day);  break;
      case 1 : prnM2("%03d-" , evTime.day);                     break;
      default : break;
    }

    prnM2("%02d:%02d:%02d.%03d-%05d\r\n",\
            evTime.hour          ,\
            evTime.min           ,\
            evTime.sec           ,\
            evTime.ms           ,\
            evTime.tick);
    return RET_OK;
  }

#if 0
// //==============================================================================
// //----===@ cnv_time
// // Parameters  :
// // Description :
// //==============================================================================
//   int timingDriver::cnvStr_time(struct s_evTime evTime, int type, char *value)
//   {
//     switch(type) // time type, 0:yyyy.mm.dd.hh.ss, 1:yyyy.mmm.hh.ss, 2:yyyy.mm.dd.hh.ss.ns, 3:yyyy.mm.dd.hh.ss.ms
//     {
//       case 0 : prnM2("%02d.%02d-", evTime.month , evTime.day);  break;
//       case 1 : prnM2("%03d-" , evTime.day);                     break;
//       default : break;
//     }

// 		sprintf(timebuf,"20%d/%d/%d %d:%d:%d"	,	ptiming->ev.info.buildTime.year  
// 												,	ptiming->ev.info.buildTime.month 
// 												,	ptiming->ev.info.buildTime.day   
// 												,	ptiming->ev.info.buildTime.hour  
// 												,	ptiming->ev.info.buildTime.min   
// 												,	ptiming->ev.info.buildTime.sec   );
// 		strcpy(value, (char*)timebuf);

//     return RET_OK;
//   }
#endif


//==============================================================================
//----===@ prn_sys
// Parameters  :
// Description :
//==============================================================================
int timingDriver::prn_sys(int pLevel)
{
  ifRet(ev.fd  < 0);
  get_sysStat();
  prnM2("======================================================\r\n");
  prnM2("++ Status : SYSTEM ++\r\n");
  prnM2("======================================================\r\n");

  prnM2_fmtDec("isInit", isInit      , " ");
  prnM2_fmtDec("evMode", evMode      , " ");
  
  prnM2_fmtDec("fan A", ev.fanA      , " ");
  prnM2_fmtDec("fan B", ev.fanB      , " ");
  prnM2_fmtDec("fan C", ev.fanC      , " ");
  prnM2_fmtDec("fan D", ev.fanD      , " ");

  prnM2_fmtDec("isMaster       ", ev.isMaster    , "[0:master (1st floor), 1:2nd floor]");
  prnM2_fmtDec("FMC PRSNT_M2C  ", ev.PRSNT_M2C_L , "[0:Loss]");
  prnM2_fmtDec("f_SFP_Loss A   ", ev.f_SFP_LossA , "[1:Loss]");
  prnM2_fmtDec("f_SFP_Loss B   ", ev.f_SFP_LossB , "[1:Loss]");
  prnM2_fmtDec("f_SFP_present A", ev.f_SFP_prsntA, "[1:Present]");
  prnM2_fmtDec("f_SFP_present B", ev.f_SFP_prsntB, "[1:Present]");

  prnM2_fmtDec("sh ID", sh.id      , "[1:lemo IO, 2:fanout]");
  prnM2_fmtDec("sl ID", sl.id      , "[1:lemo IO, 2:fanout]");

  prnM2("%-22s: 0x%04x    , %s\r\n", "sh io", ev.shio, "[high=input, low=output]");
  prnM2("%-22s: 0x%04x    , %s\r\n", "sl io", ev.slio, "[high=input, low=output]");
  
  prn_time(ev.evgTime);
  prn_time(ev.evrTime);
  
  prnM2("\r\n");
  return RET_OK;
}


//==============================================================================
//----===@ prn_evg
// Parameters  :
// Description :
//==============================================================================
int timingDriver::prn_fifoStat(struct s_fifoStat fStat, int prnLabel)
{
	if(prnLabel == 1)
	{
		prnM2("======================================================\r\n");
		prnM2("++ Status : FIFO ++\r\n");
		prnM2("======================================================\r\n");
		prnM2(" %-6s | %-5s | %-5s | %-4s | %-4s | %-5s | %-6s | %-4s | %-6s\r\n",\
				"name","wrcnt","rdcnt","werr","rerr","empty","Aempty","full","Afull");
		prnM2("---------------------------------------------------------------------\r\n");
	}

	prnM2(" %-6s | %-5d | %-5d | %-4d | %-4d | %-5d | %-6d | %-4d | %-6d\r\n",\
			fStat.name        ,\
			fStat.WRCOUNT     ,\
			fStat.RDCOUNT     ,\
			fStat.WRERR       ,\
			fStat.RDERR       ,\
			fStat.EMPTY       ,\
			fStat.ALMOSTEMPTY ,\
			fStat.FULL        ,\
			fStat.ALMOSTFULL  );
	return 0;
}

//==============================================================================
//----===@ prn_evg
// Parameters  :
// Description :
//==============================================================================
int timingDriver::prn_evg(int pLevel)
{
  ifRet(ev.fd  < 0);
  uint i,j;
  uint tmpData;
  uint cmbTmp  = 0 ;
  uint cmbTmp2 = 0 ;

  get_evgStat();

  prnM2("======================================================\r\n");
  prnM2("++ Status : EVG ++\r\n");
  prnM2("======================================================\r\n");

  prnM2_fmtDec("idec_status      ", ev.idec_status      , "[0:Loss]");
  prnM2_fmtDec("seqA_isRunning   ", ev.seqA_isRunning   , "[0:stop, 1:idle, 2:run, 3:pending]");
  prnM2_fmtDec("seqB_isRunning   ", ev.seqB_isRunning   , "[0:stop, 1:idle, 2:run, 3:pending]");
  prnM2_fmtDec("tx_evCodeA_cntr  ", ev.tx_evCodeA_cntr  , "");
  prnM2_fmtDec("tx_evCodeB_cntr  ", ev.tx_evCodeB_cntr  , "");
  prnM2_fmtDec("evrW_set_terr_cnt", ev.evrW_set_terr_cnt, "counts");
  prnM2_fmtDec("evgW_set_terr_cnt", ev.evgW_set_terr_cnt, "counts");

  prnM2_fmtDec("txEn_DBus        ", ev.evg_txEn_DBus     , "[1:Enable]");
  prnM2_fmtDec("txEn_evCodeA     ", ev.evg_txEn_evCodeA  , "[1:Enable]");
  prnM2_fmtDec("txEn_evCodeB     ", ev.evg_txEn_evCodeB  , "[1:Enable]");
  prnM2_fmtDec("txEn_mevCodeA    ", ev.evg_txEn_mevCodeA , "[1:Enable]");
  prnM2_fmtDec("seqTrgA_en       ", ev.evg_seqTrgA_en    , "[1:Enable]");
  prnM2_fmtDec("seqTrgB_en       ", ev.evg_seqTrgB_en    , "[1:Enable]");
  prnM2_fmtDec("mxc14Enable      ", ev.evg_mxc14Enable   , "[1:Enable]");
  
  prnM2("------------------------------------------------------\r\n");
  for(i=0;i<MAX_MXC_NUM;i++)
    prnM2("[%02d]%-18s: %8.3lf\r\n", i, "mxcFreq_Hz", ev.mxcFreq_Hz[i]);
  
  prnM2("------------------------------------------------------\r\n");
  for(i=0;i<8;i++)
    prnM2("[%02d]%-18s: %s\r\n", i, "tx_DBusSel", evSrcName[ev.tx_DBusSel[i]]);
  
  prnM2("------------------------------------------------------\r\n");
  for(i=1;i<16;i++)
    prnM2("[%02d]%-18s: %s\r\n", i, "evTrg", evSrcName[ev.evTrg[i]]);

  prnM2("------------------------------------------------------\r\n");
  for(j=0;j<2;j++)
  {
    prnM2("%-16s[%c]      : %d\r\n", "seqTrgCmbSrcSel", 0x41+j, ev.seqTrgCmbSrcSel[j]);
    switch(ev.seqTrgCmbSrcSel[j])
    {
      case seqS_CMB_OR0 :
      case seqS_CMB_OR1 :         cmbTmp  = ev.seqTrgCmbSrcSel[j];
                                  cmbTmp2 = 0;
                                  break;
      case seqS_CMB_OR2 : 
      case seqS_CMB_OR3 :         cmbTmp  = ev.seqTrgCmbSrcSel[j]-3;
                                  cmbTmp2 = 0;
                                  break;
      case seqS_CMB_OR0_and_OR1 : cmbTmp  = 0;
                                  cmbTmp2 = 1;
                                  break;
      case seqS_CMB_OR2_and_OR3 : cmbTmp  = 2;
                                  cmbTmp2 = 3;
                                  break;
      default :                   break;
    }
    
    prnM2("       (");
    tmpData = 1;
    for(i=0;i<31;i++){
      if((tmpData & ev.seqSrcOR[cmbTmp]) > 0) prnM2(" %s |", evSrcName[i]);
      tmpData = tmpData << 1;
    }

    if(cmbTmp2>0) {
      prnM2(")\r\n     & (");
      tmpData = 1;
      for(i=0;i<31;i++){
        if((tmpData & ev.seqSrcOR[cmbTmp2]) > 0) prnM2(" %s |", evSrcName[i]);
        tmpData = tmpData << 1;
      }
    }
    prnM2(")\r\n");
  }
    prnM2("\r\n");



    // Parameters  : seqX_TrgCmbSrc : 0~7 
//                                0 : evg_seq disable
//                                1 : seqSrcOR[0]
//                                2 : seqSrcOR[1]
//                                3 : seqSrcOR[0] & seqSrcOR[1]
//                                4 : evg_seq disable
//                                5 : seqSrcOR[2]
//                                6 : seqSrcOR[3]
//                                7 : seqSrcOR[2] & seqSrcOR[3]
  
  if(pLevel > 1)
  {
    prn_fifoStat(ev.fStat_evCodeA, 1);
    prn_fifoStat(ev.fStat_seqA, 0);
    prn_fifoStat(ev.fStat_seqB, 0);
    prn_fifoStat(ev.fStat_evUser, 0);
    prn_fifoStat(ev.fStat_evLogT0, 0);
    prn_fifoStat(ev.fStat_evLogT1, 0);
    prn_fifoStat(ev.fStat_evLogEv, 0);
    prn_fifoStat(ev.fStat_evLogPdp, 0);
  }

  prnM2("\r\n");
  return RET_OK;
}

//==============================================================================
//----===@ prn_evr
// Parameters  :
// Description :
//==============================================================================
int timingDriver::prn_evr(int pLevel)
{
  ifRet(ev.fd  < 0);

  int i = 0;
  get_evrStat();

  prnM2("======================================================\r\n");
  prnM2("++ Status : EVR ++\r\n");
  prnM2("======================================================\r\n");
  prnM2_fmtDec("evrRx_linkErr", ev.evrRx_linkErr, "[1:Error]");
  prnM2_fmtDec("evrRx_linkErrCntr", ev.evrRx_linkErrCntr, "counts");
  
  prnM2_fmtDec("evCodeA_cnt", ev.rx_evCodeA_cntr, "counts");
  prnM2_fmtDec("evCodeB_cnt", ev.rx_evCodeB_cntr, "counts");

  prnM2_fmtDec("stream_evCodeA_cnt", ev.streamRx_evCodeA_cntr, "counts");
  prnM2_fmtDec("stream_evCodeB_cnt", ev.streamRx_evCodeB_cntr, "counts");
  
  prnM2_fmtDec("emergencyStopCntr", ev.emergencyStopCntr, "counts");

  prnM2_fmtDec("rxEn_DBus      "   , ev.evr_rxEn_DBus       , "[1:Enable]");
  prnM2_fmtDec("rxEn_evCodeA   "   , ev.evr_rxEn_evCodeA    , "[1:Enable]");
  prnM2_fmtDec("rxEn_evCodeB   "   , ev.evr_rxEn_evCodeB    , "[1:Enable]");
  prnM2_fmtDec("rxEn_streamData"   , ev.evr_rxEn_streamData , "[1:Enable]");
  prnM2_fmtDec("pdpEn          "   , ev.evr_pdpEn           , "[1:Enable]");
  prnM2_fmtDec("evLogFifo_En_user" , ev.evLogFifo_En_user   , "[1:Enable]");
  
  prnM2_fmtDec("extOutSwap"   , ev.extOutSwap   , "1: sl<->sh swap");
  prnM2_fmtDec("exOut31_1pps" , ev.exOut31_1pps , "1: ext out 31 to 1pps");
  prnM2_fmtDec("pdpTrgMode"   , ev.pdpTrgMode   , "1: pdp 16~31 trigger from rx_evCodeA ");
  
  if(pLevel > 0)
  {
    for(i=0;i<32;i++)
    {
      prnM2("%-10s_%02d : %dS/ %5dR/ %5dE/ %5dD/ %5dW/ %dP\r\n",\
                  "pdp",\
                  i,
                  ev.pdp[i].isRunning  ,\
                  ev.pdp[i].runCnt     ,\
                  ev.pdp[i].errCnt     ,\
                  ev.pdp[i].delay      ,\
                  ev.pdp[i].width      ,\
                  ev.pdp[i].polarity    \
                  );
    }
  }

  for(i=0;i<32;i++)
    prnM2("%-10s_%02d : %s.\r\n", "evOut", i, evOutSrcName[ev.evOut[i]]);

  prnM2("\r\n");
  return RET_OK;
}

//==============================================================================
//----===@ prn_gtp
// Parameters  :
// Description :
//==============================================================================
int timingDriver::prn_gtp(int pLevel)
{
  ifRet(gtp.fd < 0);
  
    get_gtpStat();

    prnM2("======================================================\r\n");
    prnM2("++ Status : gtp ++\r\n");
    prnM2("======================================================\r\n");

  if(pLevel > 0)
  {
    prnM2_fmtDec("gtp_soft_reset    ", gtp.reset     , " ");
    prnM2("------------------------------------------------------\r\n");
    prnM2_fmtDec("gt0_cplllock      ", gtp.stat[0].cplllock       , " ");
    prnM2_fmtDec("gt0_txfsmresetdone", gtp.stat[0].txfsmresetdone , " ");
    prnM2_fmtDec("gt0_rxfsmresetdone", gtp.stat[0].rxfsmresetdone , " ");
    prnM2("------------------------------------------------------\r\n");
    prnM2_fmtDec("gt1_cplllock      ", gtp.stat[1].cplllock       , " ");
    prnM2_fmtDec("gt1_txfsmresetdone", gtp.stat[1].txfsmresetdone , " ");
    prnM2_fmtDec("gt1_rxfsmresetdone", gtp.stat[1].rxfsmresetdone , " ");
    prnM2("------------------------------------------------------\r\n");
    prnM2_fmtDec("gt0_trackLossCnt  ", gtp.stat[0].trackLossCnt   , " ");
    prnM2_fmtDec("gt1_trackLossCnt  ", gtp.stat[1].trackLossCnt   , " ");
    prnM2("------------------------------------------------------\r\n");
    prnM2_fmtDec("gt0_txClk_cntr  ", gtp.stat[0].txClk_cntr   , " ");
    prnM2_fmtDec("gt0_rxClk_cntr  ", gtp.stat[0].rxClk_cntr   , " ");
    prnM2_fmtDec("gt1_txClk_cntr  ", gtp.stat[1].txClk_cntr   , " ");
    prnM2_fmtDec("gt1_rxClk_cntr  ", gtp.stat[1].rxClk_cntr   , " ");
    prnM2("------------------------------------------------------\r\n");
  }
  prnM2_fmtDec("gt0_trackOK"       , gtp.stat[0].trackOK     , "<------");
  prnM2_fmtDec("gt1_trackOK"       , gtp.stat[1].trackOK     , "<------");
  prnM2_fmtDec("chkDelayCnt"       , gtp.chkDelayCnt     , "");

  prnM2("\r\n");
  return RET_OK;
}


//==============================================================================
//----===@ prn_all
// Parameters  :
// Description :
//==============================================================================
int timingDriver::prn_all(int pLevel)
{
  ifRet(ev.fd  < 0);
  ifRet(gtp.fd < 0);
  
  prn_ip(pLevel);
  prn_sys(pLevel);
  prn_gtp(pLevel);
  prn_evg(pLevel);
  prn_evr(pLevel);

  prnM2("\r\n");
  prnM2("\r\n");

  return RET_OK;
}



//==============================================================================
//----===@ max7313_setOutput
// Parameters  :
// Description :
//==============================================================================
  int timingDriver::max7313_setOutput(int busId, int slvId, char PF8, char P70 )
  {
    unsigned char txBuf;
    uint txReg;
    int bytesWrite;

    txReg = 0x02; txBuf = P70;
    bytesWrite = iic_wr(busId, slvId, txReg, 1, (char*)&txBuf, 1 );
    if(bytesWrite != 1)prnErrRet();

    txReg = 0x03; txBuf = PF8;
    bytesWrite = iic_wr(busId, slvId, txReg, 1, (char*)&txBuf, 1 );
    if(bytesWrite != 1)prnErrRet();

    prnM1("max7313_setOutput busId %d, slvID %d, PF8 %02x, P70 %02x.\r\n",busId, slvId, PF8, P70);
    return RET_OK;
  }

//==============================================================================
//----===@ max7313_configIO
// Parameters  :
// Description : max7313 port I/O config, 1:input, 0:output 
//==============================================================================
  int timingDriver::max7313_configIO(int busId, int slvId, char PF8, char P70 )
  {
    unsigned char txBuf;
    uint txReg;
    int bytesWrite;

    txReg = 0x06; txBuf = P70;
    bytesWrite = iic_wr(busId, slvId, txReg, 1, (char*)&txBuf, 1 );
    if(bytesWrite != 1)prnErrRet();

    txReg = 0x07; txBuf = PF8;
    bytesWrite = iic_wr(busId, slvId, txReg, 1, (char*)&txBuf, 1 );
    if(bytesWrite != 1)prnErrRet();

    prnM1("max7313_configIO  busId %d, slvID %d, PF8 %02x, P70 %02x.\r\n",busId, slvId, PF8, P70);
    return RET_OK;
  };



//==============================================================================
//----===@ 
// Parameters  :
// Description :
//==============================================================================
  int timingDriver::init_clk()
  {
    uint ret;

    int i;
    Reg_Data rd;
    unsigned char reg_val, test_data;
    Reg_Data* pRegData;

    // Check chip by reading Revision reg 0x00
    prnM2("init_clk...\r\n");
    test_data = si5338_read(0);
    if(test_data != 1)
    {
      prnM1("[ERROR]si5338_init test_data read\r\n");
      return RET_ERR;
    };

    // I2C Programming Procedure
    prnM1("si5338_init I2C Programming Procedure...\r\n");
    si5338_write(246, 0x01);          //Hard reset
    // Disable Outputs
    prnM1("si5338_init Disable Outputs...\r\n");
    si5338_write_mask(230, EOB_ALL, EOB_ALL); // EOB_ALL = 1
    // Pause LOL
    prnM1("si5338_init Pause LOL...\r\n");
    si5338_write_mask(241, DIS_LOL, DIS_LOL); // DIS_LOL = 1
    // Write new configuration to device accounting for the write-allowed mask
    prnM1("si5338_init Write new configuration...\r\n");

    if(evMode == RAON_EVG)
      pRegData = Reg_extClk;
    else  
      pRegData = Reg_ob25MHz;

    for(i=0; i<NUM_REGS_MAX; i++)
    {
      rd = pRegData[i];
      ret = si5338_write_mask(rd.Reg_Addr, rd.Reg_Val, rd.Reg_Mask);
      if(ret != 1)
        return RET_ERR;
    }
    // Validate clock input status
    i=0;
    prnM1("si5338_init Validate clock input status...\r\n");
    reg_val = si5338_read(218) & LOS_MASK;
    while(reg_val != 0){
      i++;
      if(i>10000)
      {
        prnM1("[ERROR]si5338_init Validate clock input status...\r\n");
        return RET_ERR;
      }
      reg_val = si5338_read(218) & LOS_MASK;
    }
    // Configure PLL for locking
    prnM1("si5338_init Configure PLL for locking...\r\n");
    si5338_write_mask(49, 0, FCAL_OVRD_EN); //FCAL_OVRD_EN = 0
    // Initiate Locking of PLL
    prnM1("si5338_init Initiate Locking of PLL...\r\n");
    si5338_write(246, SOFT_RESET);      //SOFT_RESET = 1
    usleep(25000);                // Wait 25 ms
    // Restart LOL
    prnM1("si5338_init Restart LOL...\r\n");
    si5338_write_mask(241, 0, DIS_LOL);   // DIS_LOL = 0
    si5338_write(241, 0x65);          // Set reg 241 = 0x65

    // Confirm PLL lock status
    prnM1("si5338_init Confirm PLL lock status...\r\n");
    i=0;
    reg_val = si5338_read(218) & LOCK_MASK;
    while(reg_val != 0){
      i++;
      if(i>10000)
      {
        prnM1("[ERROR]si5338_init Confirm PLL lock status...\r\n");
        return RET_ERR;
      }
      reg_val = si5338_read(218) & LOCK_MASK;
    }
    //copy FCAL values to active registers
    prnM1("si5338_init copy FCAL values to active registers...\r\n");
    si5338_write_mask( 47, si5338_read( 237), 0x03);  // 237[1:0] to 47[1:0]
    si5338_write(46, si5338_read( 236));  // 236[7:0] to 46[7:0]
    si5338_write(45, si5338_read( 235));  // 235[7:0] to 45[7:0]
    si5338_write_mask( 47, 0x14, 0xFC);   // Set 47[7:2] = 000101b
    // Set PLL to use FCAL values
    prnM1("si5338_init Set PLL to use FCAL values...\r\n");
    si5338_write_mask( 49, FCAL_OVRD_EN, FCAL_OVRD_EN); //FCAL_OVRD_EN = 1
    // Enable Outputs
    prnM1("si5338_init Enable Outputs...\r\n");
    si5338_write(230, 0x00);          //EOB_ALL = 0
    prnM2("init_clk ok..\r\n");
    return RET_OK;
  };


//==============================================================================
//----===@ 
// Parameters  :
// Description :
//==============================================================================
  unsigned char timingDriver::si5338_read(unsigned char r_addr)
  {
    unsigned char rxData = 0;
    iic_rd(IIC_BUS_ob, SLV_SI5338A, r_addr, sizeof(r_addr), (char*)&rxData, 1 );
    return (unsigned char)rxData;
  };

//==============================================================================
//----===@ 
// Parameters  :
// Description :
//==============================================================================
  uint timingDriver::si5338_write(unsigned char w_addr, unsigned char w_data)
  {
    int ret = iic_wr(IIC_BUS_ob, SLV_SI5338A, w_addr, 1, (char*)&w_data, 1 );
    return ret;
  };

//==============================================================================
//----===@ 
// Parameters  :
// Description :
//==============================================================================
  uint timingDriver::si5338_write_mask(unsigned char Addr, unsigned char Data, unsigned char Mask)
  {
    uint ret;
    unsigned char reg_val;

    if(Mask == 0xFF)
    { // All bits
      ret = si5338_write(Addr, Data);
      if(ret != 1)
        return ret;
    }
    else
    {           // Write by mask
      reg_val = si5338_read(Addr);
      reg_val &= ~Mask; // Clear bits to write
      reg_val |= Data & Mask; // Set bits by mask

      ret = si5338_write(Addr, reg_val);
      if(ret != 1)
        return ret;
    }
    return 1;
  };





























//==============================================================================================================


//==============================================================================
//----===@ ip_wr
// Parameters  :
// Description :
//==============================================================================
  int timingDriver::ip_wr(int fd, uint offset, uint wrData)
  {
    if (fd == -1) 
      prnErrRet();

    struct s_regRW reg;
    reg.offset = offset;
    reg.val    = wrData;

    if (ioctl(fd, IOCTL_W, &reg) < 0) 
      prnErrRet();

    return RET_OK;
  }

//==============================================================================
//----===@ ip_rd
// Parameters  :
// Description :
//==============================================================================
  int timingDriver::ip_rd(int fd, uint offset, uint *rdData)
  {
    if(fd == -1) prnErrRet();

    struct s_regRW reg;
    reg.offset = offset;
    reg.val    = 0;

    //prnM1("ip_rd offset : 0x%08x\r\n",offset);

    if (ioctl(fd, IOCTL_R, &reg) < 0) 
      prnErrRet();

    *rdData = reg.val;
    return RET_OK;
  };



//==============================================================================
//----===@ sys_intrEn
// Parameters  :
// Description :
//==============================================================================
  int timingDriver::sys_intrEn(int l_fd, uint wrData)
  {
    ifRet(l_fd < 0);
    ip_wr(l_fd, A_ipSys_config, wrData & 1);
    return RET_OK;
  }

//==============================================================================
//----===@ sys_intrClear
// Parameters  :
// Description :
//==============================================================================
  int timingDriver::sys_intrClear(int l_fd)
  {
    ifRet(l_fd < 0);
    ip_wr(l_fd, A_ipSys_setting, 1);
    taskDelay(0);
    ip_wr(l_fd, A_ipSys_setting, 0);
    return RET_OK;
  }

//==============================================================================
//----===@ sys_intrMask
// Parameters  :
// Description :
//==============================================================================
  int timingDriver::sys_intrMask(int l_fd, uint wrData)
  {
    ifRet(l_fd < 0);
    ip_wr(l_fd, A_ipSys_intrMask, wrData);
    return RET_OK;
  }


//==============================================================================
//----===@ sys_getIntrVal
// Parameters  :
// Description :
//==============================================================================
  int timingDriver::sys_getIntrVal(int l_fd, uint* rdData)
  {
    ifRet(l_fd < 0);
    ip_rd(l_fd, A_ipSys_intrReg, rdData);
    return RET_OK;
  }


//==============================================================================
//----===@ 
// Parameters  :
// Description :
//==============================================================================
  unsigned char timingDriver::lcdWrite(unsigned char value) //write a byte to serial port (UART/I2C/SPI)
  {
    unsigned char ret =0;
  
    write(  lcd_fd, (const char*)&value, 1);
    //ret = (unsigned char)CSerialManager::Instance()->SendCOM((const char*)&value, 1);
    return ret;
  }

//==============================================================================
//----===@ 
// Parameters  :
// Description :
//==============================================================================
  void timingDriver::lcdWriteStr(const char *s)
  {
    uint writeCnt = 0;
    while (*s != 0)
    {
      lcdWrite(*s);
      s++;
      if(writeCnt++ > 512)break;
    }
  }

//==============================================================================
//----===@ 
// Parameters  :
// Description :
//==============================================================================
  void timingDriver::lcdWrite2B(uint v)
  {
    if (v < 255)
      lcdWrite(v);
    else {
      lcdWrite(255);
      lcdWrite(v - 255);
    }
  }

//==============================================================================
//----===@ 
// Parameters  :
// Description :
//==============================================================================
  void timingDriver::lcdDrawStr(uint x, uint y, const char *s) 
  {
    lcdWriteStr("TP");
    lcdWrite2B(x);
    lcdWrite2B(y);
    lcdWriteStr("TT");
    lcdWriteStr(s);
    lcdWrite((unsigned char) 0);
  }

//==============================================================================
//----===@ 
// Parameters  :
// Description :
//==============================================================================
  void timingDriver::lcdClear(void) 
  {
    lcdWriteStr("CL");
  }

//==============================================================================
//----===@ 
// Parameters  :
// Description :
//==============================================================================
  void timingDriver::lcdSetColor(unsigned char color) 
  {
      lcdWriteStr("SC");
      lcdWrite(color);
  }

  void timingDriver::lcdSetColRow(unsigned char col, unsigned char row) 
  {
      lcdWriteStr("STCR");
      lcdWrite(col);
      lcdWrite(row);
      lcdWriteStr("\x80\xC0\x94\xD4");
  }

  void timingDriver::lcdSetBgColor(unsigned char color) //set current color as background
  {
  #if Ver>32
      lcdWriteStr("BGC");
      lcdWrite(color);
  #else
      lcdWriteStr("SC");
      lcdWrite(color);
      lcdWriteStr("FTOB");
  #endif
  }

  void timingDriver::lcdSetFont(unsigned char font) 
  {
      lcdWriteStr("SF");
      lcdWrite(font);
  }

//==============================================================================
//----===@ 
// Parameters  :
// Description :
//==============================================================================
  int timingDriver::iic_setMux(int fd, s_ts2iic* piic)
  {
    int status;
    unsigned char WriteBuffer[2];
    unsigned char BytesWritten;

    unsigned char ReadBuffer = 0x0; /* Buffer to hold data read.*/
    unsigned short int BytesToRead;

    if(piic == NULL)prnErrRet();

    status = ioctl(fd, IIC_SLAVE_FORCE, piic->muxAddress);
    if(status < 0 )prnErrRet();

    WriteBuffer[0] = piic->muxNum;
    BytesWritten = write(fd, WriteBuffer, 1);
    if(BytesWritten != 1)prnErrRet();

    BytesToRead = read(fd, &ReadBuffer, 1);
    if(BytesToRead != 1 )prnErrRet();

    return RET_OK;
  };


//==============================================================================
//----===@ iic_wr
// Parameters  :
// Description :
//==============================================================================
  int timingDriver::iic_wr(int busId, int slvId, uint offset, uint offset_size, char* pData, int size)
  {
    int i;
    int fd;
    int found = 0;
    int Status = 0;
    unsigned char WriteBuffer[MAX_IIC_RW_BUF_SIZE + 2];
    unsigned char BytesWritten;
    s_ts2iic* piic;

    if(busId >= IIC_maxDrv ) prnErrRet();
    if(pData == NULL       ) prnErrRet();
    if(size == 0           ) prnErrRet();

    /* check is valid slave.. */
    for(i = 0; i < iic_slvNum[busId]; i++)
    {
      piic = iic_getInfo(busId, i);
      if(piic->name == (unsigned)slvId)
      {
        //iic_prnInfo(busId, piic, 0);
        fd = iic_fd[busId];
        found = 1;
        break;
      }   
    }

    /* if found wirte data to iic */
    if(!found)
      prnErrRet();

    if(piic->isMux)
    {
      /* do iic mux control */  
      if(!iic_setMux(fd, piic))
        prnErrRet();
    }

    if(size > MAX_IIC_RW_BUF_SIZE )prnErrRet();

    Status = ioctl(fd, IIC_SLAVE_FORCE, piic->addr);
    if(Status < 0)prnErrRet();

    if( offset_size == 1)
    {
      WriteBuffer[0] = (unsigned char)(offset);
    }
    else if( offset_size == 2)
    {
      WriteBuffer[0] = (unsigned char)(offset>>8);
      WriteBuffer[1] = (unsigned char)(offset);
    }
    else
      prnErrRet();

    memcpy(&WriteBuffer[offset_size], pData, size); 
    BytesWritten = write(fd, WriteBuffer, size + offset_size);

    return BytesWritten-offset_size;
  };


//==============================================================================
//----===@ iic_rd
// Parameters  :
// Description :
//==============================================================================
  int timingDriver::iic_rd(int busId, int slvId, uint offset, uint offset_size, char* pData, int size)
  {
    int i;
    int fd;
    int found = 0;
    int Status = 0;
    unsigned char WriteBuffer[2];
    unsigned char BytesWritten;
    unsigned char BytesRead=0;
    s_ts2iic* piic;

    if(busId >= IIC_maxDrv ) prnErrRet();
    if(pData == NULL       ) prnErrRet();
    if(size == 0           ) prnErrRet();

    /* check is valid slave.. */
    for(i = 0; i < iic_slvNum[busId]; i++)
    {
      piic = iic_getInfo(busId, i);
      if(piic->name == (unsigned)slvId)
      {
        //iic_prnInfo(busId, piic, 0);
        fd = iic_fd[busId];
        found = 1;
        break;
      }   
    }

    /* if found wirte data to iic */
    if(!found)prnErrRet();

    if(piic->isMux)
    {
      /* do iic mux control */  
      if(!iic_setMux(fd, piic ))prnErrRet();
    }

    if(size > MAX_IIC_RW_BUF_SIZE )prnErrRet();

    Status = ioctl(fd, IIC_SLAVE_FORCE, piic->addr);
    if(Status < 0)prnErrRet();

    if( offset_size == 1)
    {
      WriteBuffer[0] = (unsigned char)(offset);
    }
    else if( offset_size == 2)
    {
      WriteBuffer[0] = (unsigned char)(offset>>8);
      WriteBuffer[1] = (unsigned char)(offset);
    }
    else
      prnErrRet();

    BytesWritten = write(fd, WriteBuffer, offset_size);
    if(BytesWritten != offset_size)prnErrRet();

    BytesRead = read(fd, pData, size);
    return BytesRead;
  };

} //name space end




