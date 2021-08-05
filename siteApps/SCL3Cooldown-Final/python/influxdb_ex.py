#!/usr/bin/env python
#coding=utf-8

##########################################################################################
from datetime import datetime, timedelta
import pprint
from influxdb import InfluxDBClient
from copy import deepcopy
import pytz

##########################################################################################
local_tz = pytz.timezone('Asia/Seoul') # use your local timezone name here
##########################################################################################
def utc_to_local(utc_dt):
	local_dt = utc_dt.replace(tzinfo=pytz.utc).astimezone(local_tz)
	return local_tz.normalize(local_dt) # .normalize might be unnecessary

##########################################################################################
def get_ifdb(db, host='localhost', port=8086, user='root', passwd='root'):
	client = InfluxDBClient(host, port, user, passwd, db)
	try:
		client.create_database(db)
	except:
		pass
	return client

##########################################################################################
def my_test(ifdb):
	json_body = [
	]
	tablename = 'table02'
	fieldname = 'fld03'
	point = {
		"measurement": tablename,
		"tags": {
			"host": "server02",
			"region": "us-east"
		},
		"fields": {
			fieldname: 0
		},
		"time": None,
	}
	dt = datetime.utcnow() - timedelta(seconds=6)
	ldt = utc_to_local(dt)
	print "UTC now=<%s> local now=<%s>" % (dt, ldt)
	vals = [ 2.3, 1.7, 1.4, 0.7,1.9 ]
	for v in vals:
		np = deepcopy(point)
		np['fields'][fieldname] = v
		np['time'] = dt
		json_body.append(np)
		dt += timedelta(seconds=1)
	ifdb.write_points(json_body)

	result = ifdb.query('select * from %s' % tablename)
	pprint.pprint(result.raw)

##########################################################################################
def do_test():
	ifdb = get_ifdb(db='tstest')
	my_test(ifdb)

##########################################################################################
if __name__ == '__main__':
	do_test()
