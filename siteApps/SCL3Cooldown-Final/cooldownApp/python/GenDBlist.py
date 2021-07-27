#!/usr/bin/python2.7

from datetime import datetime, timedelta
import pprint
import time
from influxdb import InfluxDBClient
from copy import deepcopy
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
    tablename = 'DBTable'
    fieldname = 'Signal'

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

    dblist = open('dblist', 'r')
    for idx, line in enumerate(dblist):
        pvname = line.rstrip('\n')
        if(pvname[0] =='#'): continue
        #dt = datetime.now() - timedelta(hours=-9)
        dt = datetime.utcnow()
        np = deepcopy(signal)
        np['fields'][fieldname] = pvname
        np['time'] = dt
        json_body.append(np)

    dblist.close()
    ifdb.write_points(json_body)

    #result = ifdb.query('select * from %s' % tablename)
    #result = ifdb.query('select * from %s where Signal =~ /SCL31*/' % tablename)
    #pprint.pprint(result.raw)

def Read_DB(ifdb, qword):
    tablename = 'DBTable'
    fieldname = 'Signal'

    filterword = qword

    #result = ifdb.query('select * from %s' % tablename)
    queryword = 'select Signal from {tname} where Signal =~ /{fword} */'.format(tname=tablename, fword=filterword)
    #rs = ifdb.query('select Signal from %s where Signal =~ /%s */' % tablename % filterword)
    rs = ifdb.query(queryword)
    #rs = ifdb.query('select Signal from %s' % tablename)
    #rs = ifdb.query('select Signal from %s where Signal !~ /SCL31 */' % tablename)
    #result = ifdb.query('select * from %s where {fieldname} =~ /SCL31*/' % tablename)
    #pprint.pprint(rs.raw)
    #dblist = list(rs.get_points(measurement=tablename))
    dblist = list(rs.get_points(measurement=tablename))
    for idx, pvlist in enumerate(dblist):
        pvname = str(pvlist)
        pvname = pvname.split(',')[0]
        pvname = pvname.split('\'')[3]
        print(pvname)

        #pvname = str(pvlist).split(',')[0]
        #print(pvname)

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