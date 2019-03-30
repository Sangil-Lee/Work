import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

from gekko import GEKKO
from sklearn.metrics import r2_score


#url = 'http://apmonitor.com/che263/uploads/Main/heart_rate.txt'
#x = pd.read_csv(url)

file = 'heart_rate.csv'
x = pd.read_csv(file)


print('Data')
print(x.head())

t = np.array(x['Time (sec)'])
ym = np.array(x['Heart Rate (BPM)'])


## GEKKO Model
m = GEKKO()

#Parameters

tm = m.Param(value=t)
c0=m.FV(value=100)
c1=m.FV(value=0.01)
c2=m.FV(value=100)
c3=m.FV(value=0.01)
c0.STATUS=1
c1.STATUS=1
c2.STATUS=1
c3.STATUS=1

bpm = m.CV(value=ym)
bpm.FSTATUS=1

#regression equation
m.Equation(bpm==c0+c1*tm-c2*m.exp(-c3*tm))
#regression mode
m.options.IMODE=2

#Optimize
m.solve()

print('Optimal parameters')
print(c0.value[0])
print(c1.value[0])
print(c2.value[0])
print(c3.value[0])

print('R^2: ' + str(r2_score(ym,bpm)))

#Plot data and prediction
plt.figure()
plt.title('Heart Rate Regresssion')
plt.plot(t/60.0, ym,'r--',label='Measured')
plt.plot(t/60.0, bpm,'b--',label='Predicted')
plt.ylabel('Rate (BPM)')
plt.xlabel('Time (min)')
plt.legend(loc='best')

plt.show()
