import nidaqmx

with nidaqmx.Task() as task:
    task.di_channels.add_di_chan("Dev1/port0/line0")

    task.start
    value = task.read()
    print(value)
    task.stop