# -*- coding: utf-8 -*-
"""
Created on Tue Mar 16 19:55:42 2021

@author: ctrluser
"""

import numpy as np
import matplotlib.pyplot as plt
#Construct a time signal
Fs = 2000 # Sampling freq
tstep = 1/Fs #sample time interval
f0 = 100 

N = int( 10 * Fs / f0) # Number of Samples

t = np.linspace(0, (N-1)*tstep, N)
fstep = Fs / N # freq interval

f = np.linspace(0, (N-1)*tstep, N)

# multiple wave
y = 1 * np.sin(2*np.pi * f0 * t) + \
    4 * np.sin(2*np.pi * 3 * f0 * t) + 2 #DC component


#perform fft
X = np.fft.fft(y)
X_mag = np.abs(X) / N

f_plot = f[0:int(N/2+1)]
X_mag_plot = 2*X_mag[0:int(N/2+1)]

X_mag_plot[0] = X_mag_plot[0]/2 #Note: DC component does noet need to mux by 2


#plot
fig, [ax1, ax2] = plt.subplots(nrows=2, ncols=1)
ax1.plot(t, y, '.-')
#ax2.plot(f, X_mag, '.-')
ax2.plot(f_plot, X_mag_plot, '.-')
ax1.set_xlabel('Time (s)')
ax2.set_xlabel('Freq (Hz)')

ax1.grid()
ax2.grid()

ax1.set_xlim(0, t[-1])
ax2.set(0, f_plot[-1])


plt.show()