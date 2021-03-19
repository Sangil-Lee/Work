import datetime as dt
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation
# Create figure for plotting
fig = plt.figure()
ax = fig.add_subplot(1, 1, 1)
xs = []
ys = []

xk = 0 # Initial Value for x

# This function is called periodically from FuncAnimation
def simulation(i, xs, ys):

    # Model Parameters
    a = 0.25
    b = 2

    # Simulation Parameters
    Ts = 0.1 # Sampling Time
    uk = 1 # Step Response
    global xk # Since we change x inside a function we define it as a gobal variable

    # Model of Discrete System
    xk1 = (1 - a*Ts) * xk  + Ts * b * uk
    xk = xk1

    # Add x and y to lists
    xs.append(dt.datetime.now().strftime('%H:%M:%S.%f'))
    ys.append(xk1)

    N = 60 # Limit x and y lists to N items
    xs = xs[-N:]
    ys = ys[-N:]

    # Draw x and y lists
    ax.clear()
    ax.plot(xs, ys)

    # Format plot
    plt.xticks(rotation=45, ha='right')
    plt.subplots_adjust(bottom=0.30)
    plt.title('Simulation of dxdt = -ax + bu')
    plt.xlabel('t [s]')
    plt.ylabel('x')
    plt.grid()

# Set up plot to call animate() function periodically
ani = animation.FuncAnimation(fig, simulation, fargs=(xs, ys), interval=100)
plt.show()