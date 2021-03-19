import nidaqmx
import numpy as np
import time

from nidaqmx.constants import (
    TerminalConfiguration)


Vin = 5
Ro = 10000 # %10k Resistor


with nidaqmx.Task() as task:
    task.ai_channels.add_ai_voltage_chan("Dev1/ai0", terminal_config=TerminalConfiguration.RSE)

    i = 0
    while i < 10: 

        Vout = task.read()

        Rt = (Vout*Ro)/(Vin-Vout) # Voltage Divider Equation
        # Rt=10000; Used for Testing. Setting Rt=10k should give TempC=25

        # Steinhart constants
        A = 0.001129148
        B = 0.000234125
        C = 0.0000000876741

        # Steinhart-Hart Equation
        TempK = 1 / (A + (B * np.log(Rt)) + (C * pow(np.log(Rt),3)))

        # Convert from Kelvin to Celsius
        TempC = TempK - 273.15

        print("Sample:", i)
        print("Voltage Value:", round(Vout,2))
        print("Celsius Value:", round(TempC,1))
        print("\n")
        time.sleep(1)
        i = i+1
    
    task.stop