import pandas as pd

pvnames = pd.read_excel('./SCL3_QWR_IOC_PVlist.xlsx', sheet_name='SCL31_QWR_CM', usecols='A:F', index_col=None, header=None)

pv_dict = pvnames.to_dict(orient='records')

#print(pv_dict)


key_value = list(filter(lambda elem: elem[5] == 'AO', pv_dict))
#key_value = list(pv_dict)
#print(key_value)

#fil_list = [i, pv_val[0]+pv_val[1]+pv_val[2]+pv_val[3]+pv_val[4] for i, pv_val in enumerate(key_value)]
#fil_list = list(pv_val[0] for pv_val in key_value)
#print(len(fil_list))
for i, pv_val in enumerate(key_value):
    strIndex = '%02d'%i
    pv_val[1] = pv_val[1].replace('XX', strIndex)
    pv_val[2] = pv_val[2].replace('XX', strIndex)
    fil_list = pv_val[0]+pv_val[1]+pv_val[2]+pv_val[4]+pv_val[5]
    print(fil_list)

#for pv_name in fil_list:
#    print(pv_name)

#fil_dict = {pv_val[0] for pv_val in key_value}
#fil_dict = dict(pv_val[0] for pv_val in key_value)
#print(fil_dict)
