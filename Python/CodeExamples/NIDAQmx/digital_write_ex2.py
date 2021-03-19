import nidaqmx
from nidaqmx.constants import (
    LineGrouping)

with nidaqmx.Task() as task:
    task.do_channels.add_do_chan("Dev1/port0/line0:2", line_grouping=LineGrouping.CHAN_FOR_ALL_LINES)

    value = [False, True, True]
    task.start
    task.write(value, auto_start=True)
    task.stop