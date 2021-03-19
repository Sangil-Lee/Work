import nidaqmx
import time

from nidaqmx.constants import (
    TerminalConfiguration)


with nidaqmx.Task() as task:
    task.ai_channels.add_ai_voltage_chan("Dev1/ai0", terminal_config=TerminalConfiguration.RSE)

    i = 0
    while i < 10: 

        voltage = task.read()

        degreesC = 100*voltage - 50

        print("Sample:", i)
        print("Voltage Value:", round(voltage,2))
        print("Celsius Value:", round(degreesC,1))
        print("\n")
        time.sleep(1)
        i = i+1
    
    task.stop