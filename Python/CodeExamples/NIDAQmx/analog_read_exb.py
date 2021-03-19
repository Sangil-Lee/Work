import nidaqmx

from nidaqmx.constants import (
    TerminalConfiguration)

task = nidaqmx.Task()

task.ai_channels.add_ai_voltage_chan("Dev1/ai1", terminal_config=TerminalConfiguration.DIFFERENTIAL)

task.start()

value = task.read()
print(value)

task.stop()
task.close()