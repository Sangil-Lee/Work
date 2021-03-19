import nidaqmx

with nidaqmx.Task() as task:
    task.do_channels.add_do_chan("Dev1/port0/line0")

    value = True
    task.start
    task.write(value)
    task.stop