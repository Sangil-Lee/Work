import nidaqmx

with nidaqmx.Task() as task:
    task.ao_channels.add_ao_voltage_chan('Dev1/ao0','mychannel',0,5)

    value = 4
    task.start
    task.write(value)
    task.stop