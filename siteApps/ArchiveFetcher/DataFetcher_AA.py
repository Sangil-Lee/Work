# -*- coding: utf-8 -*-
"""
Created on Tue Apr 18 23:58:05 2023

@author: SILEE
"""

import numpy as np
import pandas as pd
from aa.utils import utc_datetime
from aa.js import JsonFetcher
from datetime import datetime, timedelta, timezone


timezone_tw = timezone(timedelta(hours=8))
#datetime_taipei = datetime(2021, 6, 27, 9, 20, 13, 223000, tzinfo=timezone_tw)
#print("datetime_taipei:", datetime_taipei)

timezone_kst = timezone(timedelta(hours=9))
datetime_kst = datetime(2021, 6, 27, 9, 20, 13, 223000, tzinfo=timezone_kst)
#print("datetime_kst:", datetime_kst)

prefileIdx  = ['1', '2', '3', '4', '5']
postfileIdx = ['0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '10']

preFile = '_MDEL_GF'

preName = "ctrlslab-mdelsmooSim:"
postName = ":NoiseAI0Filter"

jf = JsonFetcher('192.168.75.70', 17668)

####Vs=10
#dirName = "Data/0419/"
#from_date = datetime(2023,4,19,12,00,00, tzinfo=timezone_kst)
#to_date   = datetime(2023,4,19,15,00,00, tzinfo=timezone_kst)

####Vs=80
#dirName = "Data/0420/"
#from_date = datetime(2023,4,19,18,00,00, tzinfo=timezone_kst)
#to_date   = datetime(2023,4,19,21,00,00, tzinfo=timezone_kst)

####Vs=100, Need test again
#dirName = "Data/0421/"
#from_date = datetime(2023,4,20,20,00,00, tzinfo=timezone_kst)
#to_date   = datetime(2023,4,20,23,00,00, tzinfo=timezone_kst)

####Vs=1000
#dirName = "Data/0422/"
#from_date = datetime(2023,4,20,9,00,00, tzinfo=timezone_kst)
#to_date   = datetime(2023,4,20,12,00,00, tzinfo=timezone_kst)

####Vs=10000
dirName = "Data/0423/"
from_date = datetime(2023,4,20,16,00,00, tzinfo=timezone_kst)
to_date   = datetime(2023,4,20,19,00,00, tzinfo=timezone_kst)

#to_date = datetime.now(timezone_kst)

only_data = 0

for index, preidx in enumerate(prefileIdx):
    filename = dirName+preidx+'_MDEL_G'+'.csv'
    dataname = preName+preidx+':NoiseAI0'
    data = jf.get_values(dataname, from_date, to_date)
    df1 = pd.DataFrame(data.values)
    
    if only_data == 1:
        df1.to_csv(filename, index=False, header=False, float_format='%.6f', sep='\t')
    else:
        datatimes = [datatime.replace(tzinfo=None) for datatime in data.datetimes(None)]
        #[print(datatime) for datatime in datatimes]
        df2 = pd.DataFrame(datatimes)
        df3 = pd.concat([df2, df1], axis=1, join='inner')
        df3.to_csv(filename, index=False, header=False, float_format='%.6f', sep='\t')

    for postidx in postfileIdx:
        #if(index == 1):
        #    break
        filename = dirName+preidx+preFile+postidx+'.csv'
        dataname = preName+preidx+postName+postidx
        print(filename, dataname)
        data = jf.get_values(dataname, from_date, to_date)
        print(len(data))
        #data.datetimes(timezone_kst)
        
        df1 = pd.DataFrame(data.values)
        if only_data == 1:
            df1.to_csv(filename, index=False, header=False, float_format='%.6f', sep='\t')
        else:
            datatimes = [datatime.replace(tzinfo=None) for datatime in data.datetimes(None)]
            #[print(datatime) for datatime in datatimes]
            df2 = pd.DataFrame(datatimes)
            df3 = pd.concat([df2, df1], axis=1, join='inner')
            df3.to_csv(filename, index=False, header=False, float_format='%.6f', sep='\t')

#####end

# ### Following test code
# event = jf.get_event_at('ctrlslab-mdelsmooSim:4:NoiseAI0Filter0', datetime.now())
# print(event.value)
# print(event.utc_datetime)

# #data = jf.get_values('ctrlslab-mdelsmooSim:4:NoiseAI0Filter0', utc_datetime(2023,4,18,11,20,00), utc_datetime(2023,4,18,11,25,00))

# print("from:", datetime(2023,4,18,23,00,00), datetime.now())
# #data = jf.get_values('ctrlslab-mdelsmooSim:5:NoiseAI0Filter0', datetime(2023,4,18,23,00,00), datetime.now())
# data = jf.get_values('ctrlslab-mdelsmooSim:4:NoiseAI0Filter0', datetime(2023,4,19,00,00,00, tzinfo=timezone_kst), datetime(2023,4,19,3,00,00, tzinfo=timezone_kst))

# df = pd.DataFrame(data.values)

# #df.to_csv("test.csv", index=False, header=False, float_format='%.6f')
# print(data.values)
# print(type(data.values))
# print(data.datetimes(timezone_kst))
# print(len(data))