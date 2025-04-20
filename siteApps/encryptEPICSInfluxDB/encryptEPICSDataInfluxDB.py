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

    def __decryptData(self, encData: bytes) -> float:
        decryptData = float(self.Cipher.decrypt(encData))
        return decryptData

    def __decryptDatas(self, encDatas: list) -> list:
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
        print(self.__decryptDatas(encDatas))

class WaveformEventReader:
    def __init__(self, pv_name):
        """
        EPICS waveform 데이터를 event 방식으로 읽는 클래스.

        Args:
            pv_name (str): 읽을 waveform PV 이름.
        """
        self.pv_name = pv_name
        self.waveform_data = None
        self._event = threading.Event()
        self._subscription = None
        self._running = False

    def _callback(self, epics_args, user_args=None):
        """
        PV 값 변경 시 호출되는 콜백 함수.

        Args:
            epics_args (dict): EPICS 콜백 인자.
            user_args (any): 사용자 정의 인자 (여기서는 사용하지 않음).
        """
        if epics_args and 'waveform' in epics_args:
            self.waveform_data = epics_args['waveform']
            self._event.set()  # 이벤트 발생

    def start(self):
        """
        waveform PV에 대한 구독을 시작합니다.
        """
        if self._running:
            print(f"이미 {self.pv_name}에 대한 구독이 실행 중입니다.")
            return

        try:
            self._subscription = epics.camonitor(self.pv_name, callback=self._callback)
            self._running = True
            print(f"{self.pv_name}에 대한 waveform 데이터 구독 시작.")
        except Exception as e:
            print(f"{self.pv_name} 구독 시작 실패: {e}")

    def stop(self):
        """
        waveform PV에 대한 구독을 중지합니다.
        """
        if self._running:
            if self._subscription:
                epics.camonitor_clear(self._subscription)
                self._subscription = None
            self._running = False
            print(f"{self.pv_name}에 대한 waveform 데이터 구독 중지.")
        else:
            print(f"{self.pv_name}에 대한 구독이 실행 중이지 않습니다.")

    def get_event(self, timeout=None):
        """
        새로운 waveform 데이터가 도착할 때까지 기다리는 이벤트 객체를 반환합니다.

        Args:
            timeout (float, optional): 이벤트 대기 시간 (초). None이면 무한히 기다립니다.

        Returns:
            threading.Event: 이벤트 객체. 새로운 데이터가 도착하면 set() 됩니다.
        """
        return self._event

    def read_waveform(self, timeout=None):
        """
        새로운 waveform 데이터를 기다렸다가 반환합니다.

        Args:
            timeout (float, optional): 데이터 대기 시간 (초). None이면 무한히 기다립니다.

        Returns:
            numpy.ndarray or None: 수신된 waveform 데이터 (numpy 배열).
                                   타임아웃 발생 시 None을 반환합니다.
        """
        self._event.clear()  # 이벤트 플래그 초기화
        if self._event.wait(timeout):
            return self.waveform_data
        else:
            return None


if __name__ == "__main__":

    parser = argparse.ArgumentParser(description='Save and Restroe Encrypted EPICS Data to InfluxDB')
    parser.add_argument('-save', type=bool, help='Testing save data to influx db')
    parser.add_argument('-encrypt', type=bool, help='Testing encrypt test')

    args = parser.parse_args()
    print('Parsed argments: {}'.format(args))
    print('-save', args.save)
    print('-encrypt', args.encrypt)

    encryptEPICSInfluxDB = EncryptEPICSInfluxDB()

    if args.save is not None:
        encryptEPICSInfluxDB.InfluxDBTest()
    if args.encrypt is not None:
        encryptEPICSInfluxDB.TestEncryptDecrypt()



if __name__ == "__main__":
    pv_name_to_monitor = "your_waveform_pv_name"  # 실제 waveform PV 이름으로 변경하세요.

    reader = WaveformEventReader(pv_name_to_monitor)
    reader.start()

    try:
        while True:
            print("새로운 waveform 데이터를 기다립니다...")
            waveform = reader.read_waveform(timeout=5)  # 5초 동안 대기

            if waveform is not None:
                print("새로운 waveform 데이터 도착:")
                print(f"  길이: {len(waveform)}")
                print(f"  처음 10개 값: {waveform[:10]}")
            else:
                print("5초 동안 새로운 waveform 데이터가 도착하지 않았습니다.")

            time.sleep(1)  # 잠시 대기
    except KeyboardInterrupt:
        print("\n프로그램 종료.")
    finally:
        reader.stop()

