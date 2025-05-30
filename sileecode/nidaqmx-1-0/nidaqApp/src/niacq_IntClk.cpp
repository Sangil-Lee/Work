/*********************************************************************
*
* ANSI C Example program:
*    Acq-IntClk.c
*
* Example Category:
*    AI
*
* Description:
*    This example demonstrates how to acquire a finite amount of data
*    using the DAQ device's internal clock.
*
* Instructions for Running:
*    1. Select the physical channel to correspond to where your
*       signal is input on the DAQ device.
*    2. Enter the minimum and maximum voltages.
*    Note: For better accuracy try to match the input range to the
*          expected voltage level of the measured signal.
*    3. Select the number of samples to acquire.
*    4. Set the rate of the acquisition.
*    Note: The rate should be AT LEAST twice as fast as the maximum
*          frequency component of the signal being acquired.
*
* Steps:
*    1. Create a task.
*    2. Create an analog input voltage channel.
*    3. Set the rate for the sample clock. Additionally, define the
*       sample mode to be finite and set the number of samples to be
*       acquired per channel.
*    4. Call the Start function to start the acquisition.
*    5. Read all of the waveform data.
*    6. Call the Clear Task function to clear the task.
*    7. Display an error if any.
*
* I/O Connections Overview:
*    Make sure your signal input terminal matches the Physical
*    Channel I/O Control. For further connection information, refer
*    to your hardware reference manual.
*
*********************************************************************/

#include <stdio.h>
#include <NIDAQmx.h>

int main(void)
{
	int32       error=0;
	TaskHandle  taskHandle=0;
	int32       read;
	float64     data[1000];
	char        errBuff[2048]={'\0'};

	/*********************************************/
	// DAQmx Configure Code
	/*********************************************/
	DAQmxCreateTask("",&taskHandle);
	//DAQmxCreateAIVoltageChan(taskHandle,"PXI1Slot5/ai0","",DAQmx_Val_Cfg_Default,-10.0,10.0,DAQmx_Val_Volts,NULL);
	//DAQmxCreateAIVoltageChan(taskHandle,"PXI1Slot5/ai0","",DAQmx_Val_Cfg_Default,0.0,10.0,DAQmx_Val_Volts,NULL);
	//DAQmxCfgSampClkTiming(taskHandle,"",10000.0,DAQmx_Val_Rising,DAQmx_Val_FiniteSamps,1000);
	DAQmxCreateAIVoltageChan(taskHandle,"PXI1Slot5/ai0","",DAQmx_Val_RSE,0.0,10.0,DAQmx_Val_Volts,NULL);
	DAQmxCfgSampClkTiming(taskHandle,"",10000.0,DAQmx_Val_Rising,DAQmx_Val_ContSamps,1000);

	/*********************************************/
	// DAQmx Start Code
	/*********************************************/
	DAQmxStartTask(taskHandle);

	/*********************************************/
	// DAQmx Read Code
	/*********************************************/
	DAQmxReadAnalogF64(taskHandle,1000,10.0,DAQmx_Val_GroupByChannel,data,1000,&read,NULL);

	for ( int index = 0; index < 1000; index++)
		printf("Data[%d] :%f\n",index, data[index]);

	printf("Acquired %d points\n",(int)read);

	if( DAQmxFailed(error) )
		DAQmxGetExtendedErrorInfo(errBuff,2048);
	if( taskHandle!=0 )  {
		/*********************************************/
		// DAQmx Stop Code
		/*********************************************/
		DAQmxStopTask(taskHandle);
		DAQmxClearTask(taskHandle);
	}
	if( DAQmxFailed(error) )
		printf("DAQmx Error: %s\n",errBuff);
	printf("End of program\n");
	return 0;
}
