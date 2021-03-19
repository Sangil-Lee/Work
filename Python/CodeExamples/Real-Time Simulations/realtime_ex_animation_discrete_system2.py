import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation

# Parameters
N = 200
x_len = N         # Number of points to display
y_range = [0, 10]  # Range of possible Y values to display

# Create figure for plotting
fig = plt.figure()
ax = fig.add_subplot(1, 1, 1)
xs = list(range(0, N))
ys = [0] * x_len
ax.set_ylim(y_range)

xk = 0 # Initial Value for x

# Create a blank line. We will update the line in animate
line, = ax.plot(xs, ys)

# Add labels
plt.title('Simulation of dxdt = -ax + bu')
plt.xlabel('Samples')
plt.ylabel('x')

# This function is called periodically from FuncAnimation
def simulation(i, ys):
  
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

    # Add y to list
    ys.append(xk1)

    # Limit y list to set number of items
    ys = ys[-x_len:]

    # Update line with new Y values
    line.set_ydata(ys)

    return line,

# Set up plot to call animate() function periodically
ani = animation.FuncAnimation(fig,
    simulation,
    fargs=(ys,),
    interval=100,
    blit=True)
plt.show()