#include <stdio.h>
#include <NIDAQmx.h>

int main() {
  // Initialize DAQmx
  int status = DAQmxInitialize();
  if (status != DAQmx_Success) {
    printf("Error initializing DAQmx: %s\n", DAQmxGetErrorString(status));
    return 1;
  }

  // Create a task
  TaskHandle taskHandle;
  status = DAQmxCreateTask("MyTask", &taskHandle);
  if (status != DAQmx_Success) {
    printf("Error creating task: %s\n", DAQmxGetErrorString(status));
    return 1;
  }

  // Configure the analog input channels
  int32 numChannels = 1;
  float64 minVal = -10.0;
  float64 maxVal = 10.0;
  status = DAQmxCfgSampClkTiming(taskHandle, "", 1000.0, DAQmx_Val_RisingEdge, DAQmx_Val_ContSamps, numChannels);
  if (status != DAQmx_Success) {
    printf("Error configuring analog input channels: %s\n", DAQmxGetErrorString(status));
    return 1;
  }

  // Start the acquisition
  status = DAQmxStartTask(taskHandle);
  if (status != DAQmx_Success) {
    printf("Error starting acquisition: %s\n", DAQmxGetErrorString(status));
    return 1;
  }

  // Read the data
  float64 *data = (float64 *)malloc(sizeof(float64) * numChannels);
  int32 numRead = 0;
  status = DAQmxReadAnalogF64(taskHandle, numChannels, 1000.0, DAQmx_Val_GroupByChannel, data, sizeof(float64) * numChannels, &numRead, NULL);
  if (status != DAQmx_Success) {
    printf("Error reading data: %s\n", DAQmxGetErrorString(status));
    return 1;
  }

  // Print the data
  for (int i = 0; i < numChannels; i++) {
    printf("Channel %d: %f\n", i, data[i]);
  }

  // Stop the acquisition
  status = DAQmxStopTask(taskHandle);
  if (status != DAQmx_Success) {
    printf("Error stopping acquisition: %s\n", DAQmxGetErrorString(status));
    return 1;
  }

  // Close the task
  status = DAQmxCloseTask(taskHandle);
  if (status != DAQmx_Success) {
    printf("Error closing task: %s\n", DAQmxGetErrorString(status));
    return 1;
  }

  // Finalize DAQmx
  status = DAQmxUninitialize();
  if (status != DAQmx_Success) {
    printf("Error finalizing DAQmx: %s\n", DAQmxGetErrorString(status));
    return 1;
  }

  return 0;
}

