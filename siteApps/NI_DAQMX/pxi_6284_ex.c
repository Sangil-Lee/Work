#include <stdio.h>
#include <NIDAQmx.h>

int main() {
  // Initialize DAQmx
  int32 status;
  status = DAQmxInitialize();
  if (status != 0) {
    printf("DAQmxInitialize failed with error code %d\n", status);
    return -1;
  }

  // Create a task
  TaskHandle taskHandle;
  status = DAQmxCreateTask("MyTask", &taskHandle);
  if (status != 0) {
    printf("DAQmxCreateTask failed with error code %d\n", status);
    return -1;
  }

  // Configure the analog input channels
  int32 numChannels = 2;
  float64 minVal = -10.0;
  float64 maxVal = 10.0;
  status = DAQmxCfgSampClkTiming(taskHandle, "", 10000.0, DAQmx_Val_ContSamps, numChannels, minVal, maxVal);
  if (status != 0) {
    printf("DAQmxCfgSampClkTiming failed with error code %d\n", status);
    return -1;
  }

  // Start the task
  status = DAQmxStartTask(taskHandle);
  if (status != 0) {
    printf("DAQmxStartTask failed with error code %d\n", status);
    return -1;
  }

  // Read the data
  float64 *data = (float64 *)malloc(sizeof(float64) * numChannels);
  int32 numRead = 0;
  status = DAQmxReadAnalogF64(taskHandle, numChannels, data, numChannels, DAQmx_Val_Auto, &numRead, NULL);
  if (status != 0) {
    printf("DAQmxReadAnalogF64 failed with error code %d\n", status);
    return -1;
  }

  // Print the data
  for (int i = 0; i < numChannels; i++) {
    printf("Channel %d: %f\n", i, data[i]);
  }

  // Stop the task
  status = DAQmxStopTask(taskHandle);
  if (status != 0) {
    printf("DAQmxStopTask failed with error code %d\n", status);
    return -1;
  }

  // Close the task
  status = DAQmxClearTask(taskHandle);
  if (status != 0) {
    printf("DAQmxClearTask failed with error code %d\n", status);
    return -1;
  }

  // Finalize DAQmx
  status = DAQmxUninitialize();
  if (status != 0) {
    printf("DAQmxUninitialize failed with error code %d\n", status);
    return -1;
  }

  return 0;
}


//This code will create a task, configure the analog input channels, start the task, read the data, print the data, stop the task, and close the task. 
//You can use this code as a starting point for your own projects.
