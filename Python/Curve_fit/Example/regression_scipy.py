import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

from scipy.optimize import curve_fit
from sklearn.metrics import r2_score


#url = 'http://apmonitor.com/che263/uploads/Main/heart_rate.txt'
#x = pd.read_csv(url)

file = 'heart_rate.csv'
x = pd.read_csv(file)


print('Data')
print(x.head())

t = x['Time (sec)'].values
ym = x['Heart Rate (BPM)'].values

def bpm(t, c0, c1,c2,c3):
	return c0+c1*t-c2*np.exp(-c3*t)

p0=[100,0.01,100,0.01]
c,cov=curve_fit(bpm, t, ym, p0)

print('Optimal parameters')
print(c)

yp = bpm(t,c[0],c[1],c[2],c[3])
print('R^2: ' + str(r2_score(ym,yp)))

plt.figure()
plt.title('Heart Rate Regresssion')
plt.plot(t/60.0, ym,'r--',label='Measured')
plt.plot(t/60.0, yp,'b--',label='Predicted')
plt.ylabel('Rate (BPM)')
plt.xlabel('Time (min)')
plt.legend(loc='best')

plt.show()


