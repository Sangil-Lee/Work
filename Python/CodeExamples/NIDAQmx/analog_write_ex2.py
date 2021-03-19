import nidaqmx
from nidaqmx.types import CtrTime

with nidaqmx.Task() as task:
    task.ao_channels.add_ao_voltage_chan("Dev1/ao0", "Test", 0, 5) 

    task.write([1.1, 2.2, 3.3, 4.4, 3.5], auto_start=True)
    print('1 Channel 1 Sample Write: ')
    print(task.write(1.0))
    task.stop()