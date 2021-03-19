import nidaqmx
import time
import datetime as dt
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation

from nidaqmx.constants import (
    TerminalConfiguration)


# Create figure for plotting
fig = plt.figure()
ax = fig.add_subplot(1, 1, 1)
xs = []
ys = []

# Initialize DAQ device
task = nidaqmx.Task()
task.ai_channels.add_ai_thrmcpl_chan("TC01/ai0")
task.start()


# This function is called periodically from FuncAnimation
def readdaq(i, xs, ys):

    #Read Value from DAQ device
    temp = task.read()

    temp = round(temp, 1)
    print("Celsius Value:", temp)

    # Add x and y to lists
    xs.append(dt.datetime.now().strftime('%H:%M:%S.%f'))
    ys.append(temp)

    # Limit x and y lists to 20 items
    xs = xs[-20:]
    ys = ys[-20:]

    # Draw x and y lists
    ax.clear()
    ax.plot(xs, ys)

    # Format plot
    plt.xticks(rotation=45, ha='right')
    plt.subplots_adjust(bottom=0.30)
    plt.title('Temperature Data')
    plt.ylabel('Temperature (deg C)')


# Set up plot to call readdaq() function periodically
ani = animation.FuncAnimation(fig, readdaq, fargs=(xs, ys), interval=1000)
plt.show()
task.stop()
task.close()