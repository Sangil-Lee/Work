//Wrapping class for system calls of nidaq device driver.

#include <stdlib.h>
#include <termios.h>

#include "lvfpgaDriver.h"


namespace lvfpga
{

	//==============================================================================
	//----===@ lvfpgaDriver
	// Parameters  :
	// Description :
	//==============================================================================
	lvfpgaDriver::lvfpgaDriver(const char* bitfile, const char* signature, const char* resource)
	{
		NiFpga_Status status = NiFpga_Initialize();

		if (NiFpga_IsNotError(status))
		{
			printf("- FPGA Initialize OK! \n");

			//opens a session, downloads the bitstream, and runs the FPGA
			//status = NiFpga_MergeStatus(&status, NiFpga_Open(NiFpga_FPGA_1_Bitfile, NiFpga_FPGA_1_Signature, "RIO0", 0/*attribute*/, &gSession));
			NiFpga_MergeStatus(&status, NiFpga_Open(bitfile, signature, resource, 0/*attribute*/, &gSession));
			printf("Bit filepath:%s\n", bitfile);
			//NiFpga_MergeStatus(&status, NiFpga_Open(NiFpga_FPGA_1_Bitfile,NiFpga_FPGA_1_Signature, "RIO0", 0/*attribute*/, &gSession));
			if (NiFpga_IsNotError(status))
			{
				printf("- NiFpga Open OK!, Session(%d) \n", gSession);
				NiFpga_IrqContext irqContext;
				NiFpga_MergeStatus(&status, NiFpga_ReserveIrqContext(gSession, &irqContext));
				if (NiFpga_IsNotError(status))
				{
#if 0
					drv_rio.status = status;
					drv_rio.session = &gSession;
					drv_rio.irqContext = &irqContext;
					drv_rio.run = 1;
					drv_rio.isWrite = 0;
					drv_rio.convTime = 0;
					epicsThreadCreate(thName,
							epicsThreadPriorityHigh,
							epicsThreadGetStackSize(epicsThreadStackSmall),
							(EPICSTHREADFUNC)GetDataThread, &drv_rio);
#endif
				} 
				else 
				{
					printf("- FPGA can't reserve IRQ! ... (errorcode %d)\n\n", status);
					NiFpga_MergeStatus(&status, NiFpga_Close(gSession, 0));
					NiFpga_MergeStatus(&status, NiFpga_Finalize());
				};
			}
			else
			{
				printf("- NiFpga Open Fail!, Why...(%d) \n", status);
			};
		};
	};

	//==============================================================================
	//----===@ ~lvfpgaDriver
	// Parameters  :
	// Description :
	//==============================================================================
	lvfpgaDriver::~lvfpgaDriver()
	{
		NiFpga_Status status = NiFpga_Finalize();
		NiFpga_MergeStatus(&status, NiFpga_Close(gSession, 0));
		NiFpga_MergeStatus(&status, NiFpga_Finalize());
	}

	lvfpgaDriver* lvfpgaDriver::GetInstance()
	{
		return this;
	};

	NiFpga_Status lvfpgaDriver::ReadBool(uint32_t indicator, NiFpga_Bool* value)
	{
		return (NiFpga_ReadBool(gSession, indicator, value));
	};
	NiFpga_Status lvfpgaDriver::ReadI8(uint32_t   indicator, int8_t* value)
	{
		return (NiFpga_ReadI8(gSession, indicator, value));
	};
	NiFpga_Status lvfpgaDriver::ReadU8(uint32_t   indicator, uint8_t* value)
	{
		return (NiFpga_ReadU8(gSession, indicator, value));
	};
	NiFpga_Status lvfpgaDriver::ReadI16(uint32_t  indicator, int16_t* value)
	{
		return (NiFpga_ReadI16(gSession, indicator, value));
	};
	NiFpga_Status lvfpgaDriver::ReadU16(uint32_t	indicator, uint16_t* value)
	{
		return (NiFpga_ReadU16(gSession, indicator, value));
	};
	NiFpga_Status lvfpgaDriver::ReadI32(uint32_t	indicator, int32_t* value)
	{
		return (NiFpga_ReadI32(gSession, indicator, value));
	};
	NiFpga_Status lvfpgaDriver::ReadU32(uint32_t	indicator, uint32_t* value)
	{
		return (NiFpga_ReadU32(gSession, indicator, value));
	};
	NiFpga_Status lvfpgaDriver::ReadI64(uint32_t	indicator, int64_t* value)
	{
		return (NiFpga_ReadI64(gSession, indicator, value));
	};
	NiFpga_Status lvfpgaDriver::ReadU64(uint32_t	indicator, uint64_t* value)
	{
		return (NiFpga_ReadU64(gSession, indicator, value));
	};
	NiFpga_Status lvfpgaDriver::ReadSgl(uint32_t	indicator, float* value)
	{
		return (NiFpga_ReadSgl(gSession, indicator, value));
	};
	NiFpga_Status lvfpgaDriver::ReadDbl(uint32_t	indicator, double* value)
	{
		return (NiFpga_ReadDbl(gSession, indicator, value));
	};

	NiFpga_Status lvfpgaDriver::WriteBool(uint32_t  control,  NiFpga_Bool value)
	{
		return (NiFpga_WriteBool(gSession, control, value));
	};
	NiFpga_Status lvfpgaDriver::WriteI8(uint32_t    control,  int8_t value)
	{
		return (NiFpga_WriteI8(gSession, control, value));
	};
	NiFpga_Status lvfpgaDriver::WriteU8(uint32_t    control,  uint8_t value)
	{
		return (NiFpga_WriteU8(gSession, control, value));
	};
	NiFpga_Status lvfpgaDriver::WriteI16(uint32_t   control,  int16_t value)
	{
		return (NiFpga_WriteI16(gSession, control, value));
	};
	NiFpga_Status lvfpgaDriver::WriteU16(uint32_t   control,  uint16_t value)
	{
		return (NiFpga_WriteU16(gSession, control, value));
	};
	NiFpga_Status lvfpgaDriver::WriteI32(uint32_t   control,  int32_t value)
	{
		return (NiFpga_WriteI32(gSession, control, value));
	};
	NiFpga_Status lvfpgaDriver::WriteU32(uint32_t  control, uint32_t value)
	{
		return (NiFpga_WriteU32(gSession, control, value));
	};
	NiFpga_Status lvfpgaDriver::WriteI64(uint32_t   control,  int64_t value)
	{
		return (NiFpga_WriteI64(gSession, control, value));
	};
	NiFpga_Status lvfpgaDriver::WriteU64(uint32_t   control,  uint64_t value)
	{
		return (NiFpga_WriteU64(gSession, control, value));
	};
	NiFpga_Status lvfpgaDriver::WriteSgl(uint32_t   control,  float value)
	{
		return (NiFpga_WriteSgl(gSession, control, value));
	};
	NiFpga_Status lvfpgaDriver::WriteDbl(uint32_t   control,  double value)
	{
		return (NiFpga_WriteDbl(gSession, control, value));
	};

	NiFpga_Status lvfpgaDriver::ReadArrayBool(uint32_t indicator, NiFpga_Bool* value, size_t length)
	{
		return (NiFpga_ReadArrayBool(gSession, indicator, value, length));
	};

	NiFpga_Status lvfpgaDriver::ReadArrayI8(uint32_t indicator, int8_t* value, size_t length)
	{
		return (NiFpga_ReadArrayI8(gSession, indicator, value, length));
	};

	NiFpga_Status lvfpgaDriver::ReadArrayU8(uint32_t indicator, uint8_t* value, size_t length)
	{
		return (NiFpga_ReadArrayU8(gSession, indicator, value, length));
	};

	NiFpga_Status lvfpgaDriver::ReadArrayI16(uint32_t indicator, int16_t* value, size_t length)
	{
		return (NiFpga_ReadArrayI16(gSession, indicator, value, length));
	};

	NiFpga_Status lvfpgaDriver::ReadArrayU16(uint32_t indicator, uint16_t* value, size_t length)
	{
		return (NiFpga_ReadArrayU16(gSession, indicator, value, length));
	};

	NiFpga_Status lvfpgaDriver::ReadArrayI32(uint32_t indicator, int32_t* value, size_t length)
	{
		return (NiFpga_ReadArrayI32(gSession, indicator, value, length));
	};

	NiFpga_Status lvfpgaDriver::ReadArrayU32(uint32_t indicator, uint32_t* value, size_t length)
	{
		return (NiFpga_ReadArrayU32(gSession, indicator, value, length));
	};

	NiFpga_Status lvfpgaDriver::ReadArrayI64(uint32_t indicator, int64_t* value, size_t length)
	{
		return (NiFpga_ReadArrayI64(gSession, indicator, value, length));
	};

	NiFpga_Status lvfpgaDriver::ReadArrayU64(uint32_t indicator, uint64_t* value, size_t length)
	{
		return (NiFpga_ReadArrayU64(gSession, indicator, value, length));
	};

	NiFpga_Status lvfpgaDriver::ReadArraySgl(uint32_t indicator, float* value, size_t length)
	{
		return (NiFpga_ReadArraySgl(gSession, indicator, value, length));
	};

	NiFpga_Status lvfpgaDriver::ReadArrayDbl(uint32_t indicator, double* value, size_t length)
	{
		return (NiFpga_ReadArrayDbl(gSession, indicator, value, length));
	};

	NiFpga_Status lvfpgaDriver::WriteArrayBool(uint32_t control, NiFpga_Bool* value, size_t length)
	{
		return (NiFpga_WriteArrayBool(gSession, control, value, length));
	};

	NiFpga_Status lvfpgaDriver::WriteArrayI8(uint32_t control, int8_t* value, size_t length)
	{
		return (NiFpga_WriteArrayI8(gSession, control, value, length));
	};
	NiFpga_Status lvfpgaDriver::WriteArrayU8(uint32_t control, uint8_t* value, size_t length)
	{
		return (NiFpga_WriteArrayU8(gSession, control, value, length));
	};
	NiFpga_Status lvfpgaDriver::WriteArrayI16(uint32_t control, int16_t* value, size_t length)
	{
		return (NiFpga_WriteArrayI16(gSession, control, value, length));
	};
	NiFpga_Status lvfpgaDriver::WriteArrayU16(uint32_t control, uint16_t* value, size_t length)
	{
		return (NiFpga_WriteArrayU16(gSession, control, value, length));
	};
	NiFpga_Status lvfpgaDriver::WriteArrayI32(uint32_t control, int32_t* value, size_t length)
	{
		return (NiFpga_WriteArrayI32(gSession, control, value, length));
	};
	NiFpga_Status lvfpgaDriver::WriteArrayU32(uint32_t control, uint32_t* value, size_t length)
	{
		return (NiFpga_WriteArrayU32(gSession, control, value, length));
	};
	NiFpga_Status lvfpgaDriver::WriteArrayI64(uint32_t control, int64_t* value, size_t length)
	{
		return (NiFpga_WriteArrayI64(gSession, control, value, length));
	};
	NiFpga_Status lvfpgaDriver::WriteArrayU64(uint32_t control, uint64_t* value, size_t length)
	{
		return (NiFpga_WriteArrayU64(gSession, control, value, length));
	};
	NiFpga_Status lvfpgaDriver::WriteArraySgl(uint32_t control, float* value, size_t length)
	{
		return (NiFpga_WriteArraySgl(gSession, control, value, length));
	};
	NiFpga_Status lvfpgaDriver::WriteArrayDbl(uint32_t control, double* value, size_t length)
	{
		return (NiFpga_WriteArrayDbl(gSession, control, value, length));
	};



#if 0
static void GetDataThread(void *lParam)
{
    EPICS_RIO *rio = (EPICS_RIO *)lParam;

    int i;
    int32_t val[18];
    double fval[18];
    double aiVal[18];

    NiFpga_Status status = rio->status;

    NiFpga_Session Session = *rio->session;
    NiFpga_Bool bool_value = NiFpga_True;

    uint32_t irqsAsserted;
    NiFpga_Bool timeOut;
    NiFpga_IrqContext = *rio->irqContext;

    // wait until an IRQ is asserted
    NiFpga_MergeStatus(&status, NiFpga_WaitOnIrgs(Session, irqContext, NiFpga_Irq_0, 1000, &irqsAsserted, &timeOut);

    if (NiFpga_IsNotError(status) && !timeOut)
    {
        if(irqsAsserted && NiFpga_Irq_0)
        {
            printf("- FPGA interrupted! \n");
            NiFpga_MergeStatus(&status, NiFpga_ReadArrayI32(Session, NiFpga_temperature1_IndicatorArrayI32_ConvI32, val, NiFpga_temperature1_IndicatorArrayI32Size_ConvI
            if (NiFpga_IsNotError(status))
            {
                for(i=0; i < 18; i++)
                    fval[i] = FIXED2FLOAT(val[i]);
                aiVal[17] = NI9213_CJC_Calc(fval[17]);
                rio->val[16] = aiVal[17];
                for(i=0; i < 16; i++) {
                    aiVal[i] = (float)CalcTemperature(aiVal[17], fval[i]);
                    rio->val[i] = aiVal[i];
                }

                NiFpga_MergeStatus(&status, NiFpga_AcknowledgeIrqs(Session, irqsAsserted));
                printf("- FPGA interrupt acknowledge completed \n");

                printf("CJC Raw value (%f), Real value (%f)\n", fval[17], aiVal[17]);
                printf("CH1 Raw value (%f), Real value (%f)\n", fval[0], aiVal[0]);

                if(rio->isWrite == 1) {
                    if(rio->convTime == 0) {
                        status = NiFpga_WriteU8(Session, NiFpga_temperature1_ControlU8_ConversionTime, 0);
                    } else {
                        status = NiFpga_WriteU8(Session, NiFpga_temperature1_ControlU8_ConversionTime, 1);
                    }
                    rio->isWrite = 0;
                }
            } else {
                printf("- FPGA read error (errorcode %d)\n\n", status);
                NiFpga_MergeStatus(&status, NiFpga_MergeStatus(&status, NiFpga_UnreserveIrqContext(Session, irqContext));
                if (NiFpga_IsError(status))
                    printf("- FPGA Unreserve IRQ error -- (errorcode %d)\n\n", status);
                NiFpga_MergeStatus(&status, NiFpga_Finalize());
                if (NiFpga_IsError(status))
                    printf("- FPGA Finalize error -- (errorcde %d)\n\n", status);
            }
        }
    }
}
#endif


}; //name space end




