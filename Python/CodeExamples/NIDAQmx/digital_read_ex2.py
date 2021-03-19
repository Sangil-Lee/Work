import nidaqmx

from nidaqmx.constants import (
    LineGrouping)

with nidaqmx.Task() as task:
    task.di_channels.add_di_chan("Dev1/port0/line0:2",  line_grouping=LineGrouping.CHAN_PER_LINE)

    task.start
    data = task.read()
    print(data)
    task.stop