import nidaqmx
import time

with nidaqmx.Task() as task:
    task.do_channels.add_do_chan("Dev1/port0/line0")

    value = True
    task.start

    i = 1
    while i < 10: 

        task.write(value)
        time.sleep(1)
        value = not value
        task.write(value)
        i = i+1
    
    task.stop