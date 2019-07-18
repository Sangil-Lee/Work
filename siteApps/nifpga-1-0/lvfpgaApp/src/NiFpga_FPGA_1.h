/*
 * Generated with the FPGA Interface C API Generator 19.0
 * for NI-RIO 19.0 or later.
 */
#ifndef __NiFpga_FPGA_1_h__
#define __NiFpga_FPGA_1_h__

#ifndef NiFpga_Version
   #define NiFpga_Version 190
#endif

#include "NiFpga.h"

/**
 * The filename of the FPGA bitfile.
 *
 * This is a #define to allow for string literal concatenation. For example:
 *
 *    static const char* const Bitfile = "C:\\" NiFpga_FPGA_1_Bitfile;
 */
#define NiFpga_FPGA_1_Bitfile "NiFpga_FPGA_1.lvbitx"

/**
 * The signature of the FPGA bitfile.
 */
static const char* const NiFpga_FPGA_1_Signature = "3A16C00E5E8474D6D115D2C599A31F07";

#if NiFpga_Cpp
extern "C"
{
#endif

typedef enum
{
   NiFpga_FPGA_1_IndicatorBool_Mod8DI0 = 0x18016,
} NiFpga_FPGA_1_IndicatorBool;

typedef enum
{
   NiFpga_FPGA_1_IndicatorU16_CountRegister = 0x1800A,
} NiFpga_FPGA_1_IndicatorU16;

typedef enum
{
   NiFpga_FPGA_1_IndicatorU32_TickCurrent = 0x1800C,
} NiFpga_FPGA_1_IndicatorU32;

typedef enum
{
   NiFpga_FPGA_1_ControlU32_Threshold = 0x18010,
} NiFpga_FPGA_1_ControlU32;


#if NiFpga_Cpp
}
#endif

#endif
