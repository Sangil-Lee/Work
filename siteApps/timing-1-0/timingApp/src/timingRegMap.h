#ifndef		_TIMING_REGISTER_MAP_H
#define		_TIMING_REGISTER_MAP_H

#include <stdint.h>
#include <sys/time.h>

typedef struct timingregmap{

		// Constant parameters: set/get by system_open
		uint32_t maxSizeBufferMB;
		uint32_t maxSizeSnapshot;
		uint32_t lowLatencyEnable;
		uint32_t nb_channel;

		// Firmware / software version
		double firmware;
		double software;

		// Parameters: read (write) by system_open, system_set
		int refClockSource;    
		int hardwareTriggerSource;     
		int samplingFrequency; 
		int systemControl;      
		int snapshotMode;
		int triggerSource;      
		int triggerLevel;         
		int channelEn;
		double dwellTimeUs;
		double snapshotFreqHz; 
		double loFrequencyMHz;
		double bandwidthMHz;

		// Status: set by system_get
		double dwellTimeMaxUs;
		double *nb_snapshot_write;
		double *nb_snapshot_read;
		double occupancy_hs_buffer_pc; 
		double occupancy_ext_buffer_pc;
		double pcie_transfer_speed_MB;
		double occupancy_dma_buffer_pc;
		uint32_t system_status;
		uint32_t clock_status;
		uint32_t pcie_status;
		uint32_t buffer_status;
		uint32_t count_drop_event;
		double occupancy_snapshot_buffer_pc;
		uint32_t flag_error_set;
		uint32_t flag_error_init;
		uint32_t initialization_status;

		// Time reference, period updated by system_set, offset user defined
		uint64_t system_time_reference_ns;
		double time_period_ps;   // Period of the FPGA time generator
		uint64_t sample_period_ps; // Period betwween 2 ADC samples

		// User Simulink interface
		int user_address;
		int user_register_write;
		int user_register_read;

		// Snapshot: set by system_read
		uint32_t nb_sample;
		uint32_t nb_sample_write;
		uint32_t channel;
		uint64_t snapshot_time_s;
		uint64_t snapshot_time_ns;
		struct timeval snapshot_time_server;
		uint32_t trigger_count;
		int16_t  *sample;

		// Error message
		char error_open[300];
		char error_close[300];
		char error_set[300];
		char error_get[300];
		char error_user_set[300];
		char error_user_get[300];
		char error_read[300];
		char error_write[300];
		char error_thread[300];

		// Private
		// Max hold status
		uint32_t max_occupancy_hs_buffer;
		uint32_t max_occupancy_ext_buffer;
		double   max_pcie_transfer_speed_MB;
		double   max_occupancy_dma_buffer_pc;

		// mutex
		//pthread_t snapshot_thread;
		//pthread_mutex_t lock_read;
		//pthread_mutex_t lock_write;

		// Status signal updated by system_thread
		int thread_status;

		//struct daq_system_private *p;
}timingregmap;
#endif
