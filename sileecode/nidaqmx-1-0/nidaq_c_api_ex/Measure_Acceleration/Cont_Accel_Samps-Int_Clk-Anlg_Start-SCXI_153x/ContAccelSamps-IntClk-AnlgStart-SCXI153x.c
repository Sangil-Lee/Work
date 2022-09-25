/*********************************************************************
*
* ANSI C Example program:
*    ContAccelSamps-IntClk-AnlgStart-SCXI153x.c
*
* Example Category:
*    AI
*
* Description:
*    This example demonstrates how to make continuous, hardware-timed
*    acceleration measurement using a SCXI-153x module.
*
* Instructions for Running:
*    1. Specify the Physical Channel where you have connected the
*       accelerometer.
*    2. Enter the Minimum and Maximum acceleration values, in g, you
*       expect to measure. A smaller range will allow a more accurate
*       measurement.
*    3. Select the number of samples to acquire.
*    4. Set the rate of the acquisition
*    5. Set the source of the start trigger. By default this is
*       APFI0.
*    6. Set the slope and level of desired analog edge condition.
*    7. Set the Hysteresis Level.
*    8. Enter the Sensitivity and the Sensitivity Units for the
*       accelerometer being used.
*    9. Specify the cutoff frequency of the filter and the excitation
*       settings.
*    10. The SCXI-153x can tie the negative terminal to chassis
*        ground by selecting reference single ended(RSE) mode. Select
*        differential if you do not want the chassis ground to be
*        connected
*
* Steps:
*    1. Create a task.
*    2. Create an analog input accelerometer channel.
*    3. Configure the ground referencing and lowpass cutoff frequency
*       of the SCXI-153x module.
*    4. Set the rate for the sample clock. Additionally, define the
*       sample mode to be continuous.
*    5. Call the Start function to start the acquisition.
*    6. Read the waveform data in the EveryNCallback function until
*       the user hits the stop button or an error occurs.
*    7. Call the Clear Task function to clear the Task.
*    8. Display an error if any.
*
* I/O Connections Overview:
*    Connect your accelerometer to the terminals corresponding to the
*    Physical Channel I/O Control value. Also, make sure your analog
*    trigger terminal matches the Trigger Source Control.
*
*********************************************************************/

#include <stdio.h>
#include <NIDAQmx.h>

#define DAQmxErrChk(functionCall) if( DAQmxFailed(error=(functionCall)) ) goto Error; else

int32 CVICALLBACK EveryNCallback(TaskHandle taskHandle, int32 everyNsamplesEventType, uInt32 nSamples, void *callbackData);
int32 CVICALLBACK DoneCallback(TaskHandle taskHandle, int32 status, void *callbackData);

int main(void)
{
	int32       error=0;
	TaskHandle  taskHandle=0;
	char        errBuff[2048]={'\0'};

	/*********************************************/
	// DAQmx Configure Code
	/*********************************************/
	DAQmxErrChk (DAQmxCreateTask("",&taskHandle));
	DAQmxErrChk (DAQmxCreateAIAccelChan(taskHandle,"Dev1/ai0","",DAQmx_Val_Diff,-10.0,10.0,DAQmx_Val_AccelUnit_g,50,DAQmx_Val_mVoltsPerG,DAQmx_Val_Internal,0.004,NULL));
	DAQmxErrChk (DAQmxCfgSampClkTiming(taskHandle,"",10000.0,DAQmx_Val_Rising,DAQmx_Val_ContSamps,1000));
	DAQmxErrChk (DAQmxCfgAnlgEdgeStartTrig(taskHandle,"APFI0",DAQmx_Val_Rising,0.0));
	DAQmxErrChk (DAQmxSetAnlgEdgeStartTrigHyst(taskHandle, 1.0));
	DAQmxErrChk (DAQmxSetAILowpassCutoffFreq(taskHandle, "", 20000));

	DAQmxErrChk (DAQmxRegisterEveryNSamplesEvent(taskHandle,DAQmx_Val_Acquired_Into_Buffer,1000,0,EveryNCallback,NULL));
	DAQmxErrChk (DAQmxRegisterDoneEvent(taskHandle,0,DoneCallback,NULL));

	/*********************************************/
	// DAQmx Start Code
	/*********************************************/
	DAQmxErrChk (DAQmxStartTask(taskHandle));

	printf("Acquiring samples continuously. Press Enter to interrupt\n");
	getchar();

Error:
	if( DAQmxFailed(error) )
		DAQmxGetExtendedErrorInfo(errBuff,2048);
	if( taskHandle!=0 ) {
		/*********************************************/
		// DAQmx Stop Code
		/*********************************************/
		DAQmxStopTask(taskHandle);
		DAQmxClearTask(taskHandle);
	}
	if( DAQmxFailed(error) )
		printf("DAQmx Error: %s\n",errBuff);
	printf("End of program, press Enter key to quit\n");
	getchar();
	return 0;
}

int32 CVICALLBACK EveryNCallback(TaskHandle taskHandle, int32 everyNsamplesEventType, uInt32 nSamples, void *callbackData)
{
	int32       error=0;
	char        errBuff[2048]={'\0'};
	static int  totalRead=0;
	int32       read=0;
	float64     data[1000];

	/*********************************************/
	// DAQmx Read Code
	/*********************************************/
	DAQmxErrChk (DAQmxReadAnalogF64(taskHandle,1000,10.0,DAQmx_Val_GroupByScanNumber,data,1000,&read,NULL));
	if( read>0 ) {
		printf("Acquired %d samples. Total %d\r",(int)read,(int)(totalRead+=read));
		fflush(stdout);
	}

Error:
	if( DAQmxFailed(error) ) {
		DAQmxGetExtendedErrorInfo(errBuff,2048);
		/*********************************************/
		// DAQmx Stop Code
		/*********************************************/
		DAQmxStopTask(taskHandle);
		DAQmxClearTask(taskHandle);
		printf("DAQmx Error: %s\n",errBuff);
	}
	return 0;
}

int32 CVICALLBACK DoneCallback(TaskHandle taskHandle, int32 status, void *callbackData)
{
	int32   error=0;
	char    errBuff[2048]={'\0'};

	// Check to see if an error stopped the task.
	DAQmxErrChk (status);

Error:
	if( DAQmxFailed(error) ) {
		DAQmxGetExtendedErrorInfo(errBuff,2048);
		DAQmxClearTask(taskHandle);
		printf("DAQmx Error: %s\n",errBuff);
	}
	return 0;
}
