#!/usr/bin/python

from influxdb_client import InfluxDBClient, Point, WriteOptions
from influxdb_client.client.write_api import SYNCHRONOUS
from influxdb_client.rest import ApiException
from cryptography.fernet import Fernet
from epics import PV
import epics
import numpy as np
import sys, os, time, argparse, csv, re, json, datetime
import random, threading



class EncryptEPICSInfluxDB:

    def __init__(self):
        print("EncryptEPICSInfluxDB")

        self.Influxurl = "http://localhost:8086"
        self.Bucket="ctrluser"
        self.Org = "cotlab"
    
        self.Token = self.__influxToken()
        self.influxClient = InfluxDBClient(url=self.Influxurl, token=self.Token, org=self.Org)
        self.Cipher = Fernet(self.__encryptKey())
        print(self.__influxToken())
        print(self.__encryptKey())

    def __influxToken(self):
        #token = "COWyuJkBwwRcfv_GarnCr0KRnuBz4IF3bzAOv7T59VhRF_yPPUQ7jkTfH0NrP1rT7eG__b5bvMH6PYpBekwVYw=="
        #token = "l_HnOIjEGflnDEJDRMjWV_cmMCtm2Yrgf9oh1osPoE04egQorYqlvzCTCSnyqXfzMEjadPaClVcU2MjlIV-tVA=="
        return os.environ.get("INFLUXDB_TOKEN")

    def __encryptKey(self):
        return os.environ.get("ENCRYPT_KEY")

    def __encryptData(self, data: float) -> bytes:
        encodeData = str(data).encode()
        encryptData = self.Cipher.encrypt(encodeData)
        return encryptData

    def encryptDatas(self, datas: list) -> list:
        encryptlist = []
        for data in datas:
            encryptlist.append(self.__encryptData(data))

        return encryptlist

    def __decryptData(self, encData: bytes) -> float:
        decryptData = float(self.Cipher.decrypt(encData))
        return decryptData

    def decryptDatas(self, encDatas: list) -> list:
        decrlist = []
        for encData in encDatas:
            decrlist.append(self.__decryptData(encData))

        return decrlist

    def InfluxDBTest(self):
        influxWrite = self.influxClient.write_api(write_options=SYNCHRONOUS)
        for value in range(100):
          point = ( Point("pv1")
            .tag("tagname1", "tagvalue1")
            .field("field1", value)
          )
          influxWrite.write(bucket=self.Bucket, org=self.Org, record=point)
          time.sleep(0.01) # separate points by 1 second

        query_api = self.influxClient.query_api()
        query = """from(bucket: "ctrluser")
         |> range(start: -10m)
         |> filter(fn: (r) => r._measurement == "pv1")"""
        tables = query_api.query(query, org=self.Org)

        for table in tables:
          for record in table.records:
            print(record)

    def TestEncryptDecrypt(self):
        encDatas = []
        Values = []
        mean, std_dev = 0.0, 1.0
        min_v, max_v = 1.0, 50.0
        for value in range(100):
            fvalue = random.uniform(min_v, max_v)
            #random.gauss(mean, std_dev)
            Values.append(fvalue)
            encData = self.__encryptData(fvalue)
            encDatas.append(encData)

        print(Values)
        print(encDatas)
        print(self.decryptDatas(encDatas))

    def __onConnectionChange(self, pvname=None, conn=None, **kws):
        print(f'PV connection status changed: {pvname} {conn}')

    def __onEventData(self, pvname=None, value=None, host=None, **kws):
        print(f'PV value changed: {pvname} ({host})  {value[:20]}')

    def __onEncryptEvent(self, pvname=None, value=None, host=None, **kws):
        encryptPVDatas = self.encryptDatas(value)
        print(encryptPVDatas)
        #print(self.decryptDatas(encryptPVDatas))

    def SetPVMonitor(self, pvname=None):
        pv = epics.get_pv(pvname, connection_callback=self.__onConnectionChange, callback=self.__onEventData)
        pv.get()
        while True:
            time.sleep(0.1)

    def SetEncryptPVMonitor(self, pvname=None):
        pv = epics.get_pv(pvname, connection_callback=self.__onConnectionChange, callback=self.__onEncryptEvent)
        pv.get()
        while True:
            time.sleep(0.1)

if __name__ == "__main__":

    parser = argparse.ArgumentParser(description='Save and Restroe Encrypted EPICS Data to InfluxDB')
    parser.add_argument('-save', type=bool, help='Testing save data to influx db')
    parser.add_argument('-encrypt', type=bool, help='Testing encrypt test')
    parser.add_argument('-epics', type=bool, help='Testing encrypt test')

    args = parser.parse_args()
    print('Parsed argments: {}'.format(args))
    print('-save', args.save)
    print('-encrypt', args.encrypt)
    print('-epics', args.epics)

    encryptEPICSInfluxDB = EncryptEPICSInfluxDB()

    if args.save is not None:
        encryptEPICSInfluxDB.InfluxDBTest()
    if args.encrypt is not None:
        encryptEPICSInfluxDB.TestEncryptDecrypt()


    if args.epics is not None:
        pv_name = "TEST_sinA_WF"  # 실제 waveform PV 이름으로 변경하세요.
        try:
            #encryptEPICSInfluxDB.SetPVMonitor(pv_name)
            pvdata = encryptEPICSInfluxDB.SetEncryptPVMonitor(pv_name)
            print(pvdata)
        except KeyboardInterrupt:
            print("\nExit...")


"""
from influxdb_client import InfluxDBClient, Point, WriteOptions
import datetime
import time

# InfluxDB 2.x 연결 설정
url = "YOUR_INFLUXDB_URL"  # 예: "http://localhost:8086"
token = "YOUR_API_TOKEN"
org = "YOUR_ORG_ID"
bucket = "YOUR_BUCKET_NAME"

client = InfluxDBClient(url=url, token=token, org=org)
write_api = client.write_api(write_options=WriteOptions(batch_size=1000, flush_interval=10_000))

measurement_name = "continuous_values"
tag_key = "series_id"
tag_value = "series_1"

try:
    start_time = datetime.datetime.utcnow()
    points = []
    for i in range(10000):
        timestamp = start_time + datetime.timedelta(milliseconds=i * 10)  # 각 포인트 간 10ms 간격
        point = Point(measurement_name) \
            .tag(tag_key, tag_value) \
            .field("value", float(i)) \
            .time(timestamp)
        points.append(point)

        # 배치 쓰기 (선택 사항 - 성능 향상)
        if (i + 1) % 1000 == 0:
            write_api.write(bucket=bucket, record=points)
            points = []
            print(f"{i+1}개의 데이터 포인트 저장 완료.")

    # 남은 데이터 쓰기
    if points:
        write_api.write(bucket=bucket, record=points)
        print(f"남은 {len(points)}개의 데이터 포인트 저장 완료.")

    print("총 10000개의 연속적인 value 저장 완료.")

except Exception as e:
    print(f"데이터 저장 중 오류 발생: {e}")

finally:
    client.close()
"""
