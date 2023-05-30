#include <stdio.h>
#include <stdlib.h>
#include <NIDAQmx.h>

int main() {
  // Initialize DAQmx
  int status = DAQmxInitialize();
  if (status != DAQmxSuccess) {
    printf("Error initializing DAQmx: %s\n", DAQmxGetErrorString(status));
    return -1;
  }

  // Create a task
  TaskHandle taskHandle;
  status = DAQmxCreateTask("MyTask", &taskHandle);
  if (status != DAQmxSuccess) {
    printf("Error creating task: %s\n", DAQmxGetErrorString(status));
    return -1;
  }

  // Configure the task
  // Channel 0 of the PXI-6284 is configured to acquire analog input data at a rate of 1000 samples per second.
  status = DAQmxCreateAnalogInputChannel(taskHandle, "Dev1/ai0", "", DAQmx_Val_Volts, 1000.0);
  if (status != DAQmxSuccess) {
    printf("Error configuring channel: %s\n", DAQmxGetErrorString(status));
    return -1;
  }

  // Start the task
  status = DAQmxStartTask(taskHandle);
  if (status != DAQmxSuccess) {
    printf("Error starting task: %s\n", DAQmxGetErrorString(status));
    return -1;
  }

  // Acquire 100 samples
  int numSamples = 100;
  float *data = (float *)malloc(sizeof(float) * numSamples);
  status = DAQmxReadAnalogInput(taskHandle, data, numSamples, DAQmx_Val_Auto, DAQmx_Val_NoConversion, &numSamplesRead);
  if (status != DAQmxSuccess) {
    printf("Error acquiring data: %s\n", DAQmxGetErrorString(status));
    return -1;
  }

  // Print the data
  for (int i = 0; i < numSamples; i++) {
    printf("%f\n", data[i]);
  }

  // Stop the task
  status = DAQmxStopTask(taskHandle);
  if (status != DAQmxSuccess) {
    printf("Error stopping task: %s\n", DAQmxGetErrorString(status));
    return -1;
  }

  // Close the task
  status = DAQmxCloseTask(taskHandle);
  if (status != DAQmxSuccess) {
    printf("Error closing task: %s\n", DAQmxGetErrorString(status));
    return -1;
  }

  // Finalize DAQmx
  status = DAQmxTerminate();
  if (status != DAQmxSuccess) {
    printf("Error finalizing DAQmx: %s\n", DAQmxGetErrorString(status));
    return -1;
  }

  return 0;
}
///This code will acquire 100 samples of analog input data from channel 0 of the PXI-6284 at a rate of 1000 samples per second.
//The data will be printed to the console.
