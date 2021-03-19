import nidaqmx

from nidaqmx.constants import (
    TerminalConfiguration)

with nidaqmx.Task() as task:
    task.ai_channels.add_ai_voltage_chan("Dev1/ai1", terminal_config=TerminalConfiguration.DIFFERENTIAL)

    value = task.read()
    print(value)
    task.stop