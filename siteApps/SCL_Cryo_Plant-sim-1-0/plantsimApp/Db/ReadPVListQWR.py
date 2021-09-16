import pandas as pd
import sys

#pvnames = pd.read_excel('./SCL3_QWR_IOC_PVlist.xlsx', sheet_name='SCL31_QWR_CM', usecols='A:F', index_col=None, header=None)
pvnames = pd.read_excel('./SCL3_QWR_IOC_PVlist.xlsx', sheet_name='SCL31_QWR_VBx', usecols='A:F', index_col=None, header=None)
pv_dict = pvnames.to_dict(orient='records')
#print(pv_dict)

argcnt = len(sys.argv)
recType = sys.argv[1]

if(argcnt != 2):
    raise SystemExit('Missed Arguments')


key_value = list(filter(lambda elem: elem[5] == recType.upper(), pv_dict))
#key_value = list(pv_dict)
#print(key_value)

#fil_list = [i, pv_val[0]+pv_val[1]+pv_val[2]+pv_val[3]+pv_val[4] for i, pv_val in enumerate(key_value)]
#fil_list = list(pv_val[0] for pv_val in key_value)
#print(len(fil_list))
#for i, pv_val in enumerate(key_value):
for i in range(2, 23, 2):
    for pv_val in key_value:
        strIndex = '{:02d}'.format(i)
        new_pv_val1 = pv_val[1].replace('XX', strIndex)
        new_pv_val2 = pv_val[2].replace('XX', strIndex)
        fil_list = pv_val[0]+new_pv_val1+new_pv_val2+pv_val[3]+pv_val[4]
        print(fil_list)

#for pv_name in fil_list:
#    print(pv_name)

#fil_dict = {pv_val[0] for pv_val in key_value}
#fil_dict = dict(pv_val[0] for pv_val in key_value)
#print(fil_dict)
