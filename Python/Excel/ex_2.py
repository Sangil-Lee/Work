from openpyxl import Workbook
import datetime as dt
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.dates as mdates


data = pd.read_excel('CM_Level_Temp.xlsx')
time = data.iloc[:,0]

date = mdates.datestr2num(time)

col1 = data.iloc[:,1]
col2 = data.iloc[:,2]
col3 = data.iloc[:,3]

t1 = np.arange(0, 180, 1)

plt.figure(1)
plt.subplot(311)
plt.plot_date(date,col1,'bo')

plt.subplot(312)
plt.plot_date(date, col2, 'rx')

plt.subplot(313)
#plt.plot(t1, col3, 'g+')
plt.plot_date(date, col3, 'g+')
plt.show()

#print(column2)
