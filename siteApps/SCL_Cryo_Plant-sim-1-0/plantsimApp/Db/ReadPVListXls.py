import pandas as pd


pvnames = pd.read_excel('./SRF_HWRB_PVList.xlsx', sheet_name='HWRB', usecols='A:B', index_col=None, header=None)


#pv_list = pvnames['PVList'].tolist()
#print(len(pv_list))
#print (pv_list)

#pv_dict = pvnames.to_dict(orient='list')
#pv_dict = pvnames.to_dict(orient='split')
#pv_dict = pvnames.to_dict(orient='dict')
pv_dict = pvnames.to_dict(orient='records')
#type(pv_dict)
#print(pv_dict)

key_value = list(filter(lambda elem: elem[1] == 'AO' or elem[1] == 'AI', pv_dict))
#key_value = list(pv_dict)
#print(key_value[0])

#fil_list = [pv_val[0] for pv_val in key_value]
fil_list = list(pv_val[0] for pv_val in key_value)
#print(len(fil_list))
#print(fil_list)

for pv_name in fil_list:
    print(pv_name)

fil_dict = {pv_val[0] for pv_val in key_value}
#fil_dict = dict(pv_val[0] for pv_val in key_value)
#print(fil_dict)
