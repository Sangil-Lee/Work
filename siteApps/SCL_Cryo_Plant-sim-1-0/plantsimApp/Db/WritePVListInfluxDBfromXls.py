from datetime import datetime, timedelta
from influxdb import InfluxDBClient
from copy import deepcopy
import pprint
import time
import pandas as pd
import sys

def get_ifdb(db, host='localhost', port=8086, user='root', passwd='root'):

    client = InfluxDBClient(host, port, user, passwd, db)

    try:
        client.create_database(db)

        print('Connection Successful')

    except:

        print('Connection Failed')
        pass
    return client

def Write_DB(ifdb):
    json_body = []
    tablename = 'DBTable2'
    fieldname = 'Signal2'

    signal = {
        "measurement": tablename,
        "tags": {
            "signal":"EPICS PV Name"
        },
        "fields":{
            fieldname:0
        },
        "time":None,
    }

    pvnames = pd.read_excel('./SRF_HWRB_PVList.xlsx', sheet_name='HWRB', usecols='A:B', index_col=None, header=None)
    pv_dict = pvnames.to_dict(orient='records')
    key_value = list(pv_dict)
    fil_list = list(pv_val[0] for pv_val in key_value)

    for pv_name in fil_list:
        dt = datetime.utcnow()
        np = deepcopy(signal)
        np['fields'][fieldname] = pv_name
        np['time'] = dt
        json_body.append(np)

    ifdb.write_points(json_body)

def Read_DB(ifdb, qword):
    tablename = 'DBTable2'
    fieldname = 'Signal2'

    filterword = qword

    queryword = 'select Signal2 from {tname} where Signal2 =~ /{fword} */'.format(tname=tablename, fword=filterword)
    rs = ifdb.query(queryword)
    dblist = list(rs.get_points(measurement=tablename))

    #for idx, pvlist in enumerate(dblist):
        #pvname = str(pvlist)
        #pvname = pvname.split('\'')[7]

        #pvname = pvname.split(',')[1]
        #pvname = pvname.split(' ')[2]
        #pvname = pvname.split('\'')[1]
        #or
        #pvname = pvname.split(',')[1].split(' ')[2].split('\'')[1]

        #print(str(pvname))

    #Special Search, next
    #pvname = next( item['time'] for item in dblist if item['Signal2']=='SRF01-Bunker2:HWRB01-Temp:Cavity6Bottom')
    #print (pvname)

    pvlist = [item['Signal2'] for item in dblist]
    #pvlist = list(filter(lambda elem: elem['Signal2'], dblist))
    #pvlist = {item[1] for item in dblist}
    #print(pvlist)
    for pvname in pvlist:
        print(pvname)

def Insert_db():
    mydb = get_ifdb(db='SignalDB')
    Write_DB(mydb)

def Query_db(qword):
    mydb = get_ifdb(db='SignalDB')
    Read_DB(mydb, qword)

if __name__ == '__main__':

    argcnt = len(sys.argv)
    if(argcnt == 1):
        Insert_db()
    else:
        Query_db((str(sys.argv[1])))
