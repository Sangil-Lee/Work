import serial
import threading
import csv
import time
import queue
import struct

class PLCDataStore:
    """ PLC 데이터를 저장하고 관리하는 클래스 """
    def __init__(self, csv_filename):
        self.plc_db = self.load_plc_db(csv_filename)
        self.data_store = {}  # 시리얼 데이터를 저장할 공간
        self.valid_index = {}  # 유효한 인덱스 저장
        self.valid_index_list = set()  # 모든 PLC DB의 indexNum 저장

        # 초기화: 모든 PLC 주소에 대해 기본값 설정
        for address, info in self.plc_db.items():
            self.data_store[address] = 0  
            self.valid_index[address] = 0
            if info["index"] is not None:
                self.valid_index_list.add(info["index"])  # 모든 indexNum 저장
                
        print(f"🔍 저장된 인덱스 리스트: {self.valid_index_list}")  # ✅ 추가된 print문

    def load_plc_db(self, csv_filename):
        """ PLC DB 파일을 읽어서 index와 offset을 저장 """
        plc_db = {}
        
        try:
            with open(csv_filename, mode="r", encoding="ANSI") as file:
                reader = csv.reader(file)
                next(reader)  # 헤더 건너뛰기
                for row in reader:
                    name = row[0]  # 변수 이름
                    address = row[6]  # PLC 메모리 주소 (예: %MW6.120)
                    
                    parts = address.split(".")
                    if len(parts) > 1:
                        index = int(parts[1]) // 110
                        offset = int(parts[1]) % 110
                    else:
                        index, offset = None, None
                    
                    plc_db[address] = {"name": name, "index": index, "offset": offset}
        except UnicodeDecodeError as e:
            print(f"❌ 파일을 읽는 중 오류 발생: {e}")

        return plc_db

    def data_cnv(self, inBuf):
        """ 데이터 변환 """
        odBuf = [0] * 110
        for i in range(110):
            header_offset = 4 + inBuf[1] - 222
            if i * 2 + header_offset + 1 < len(inBuf):  # 배열 크기 초과 방지
                msb = (inBuf[i * 2 + (header_offset + 1)] << 8) & 0xFF00
                lsb = inBuf[i * 2 + header_offset] & 0xFF
                odBuf[i] = msb | lsb
        return odBuf

    def update_data(self, payload, index_number):
        """ 수신된 데이터를 저장 """
        converted_data = self.data_cnv(payload)
        
        value = None  # value 기본값 설정
        offset = None  # 기본값 설정
        
        for address, info in self.plc_db.items():
            if info["index"] == index_number:
                offset = info["offset"]
                if offset is None:
                    print(f"⚠️ {address}의 Offset이 None입니다. 패킷 Index: {index_number}")
                    continue  # 해당 데이터는 건너뛰기

                if offset >= len(converted_data):
                    print(f"⚠️ {address}의 Offset({offset})이 변환된 데이터 크기({len(converted_data)})를 초과합니다.")
                    continue  # 리스트 범위를 벗어나면 건너뛰기
                
                value = converted_data[offset]
                self.data_store[address] = value
                self.valid_index[address] = 1
                #print(f"📦 수신된 패킷 Index: {index_number}, Offset: {offset}, Value: {value}")
                
            #if address == "%MW6.0":
                #print(f"🔍 MW6.0 패킷 정보 - Index: {index_number}, Offset: {offset}, Value: {value}")

        if offset is None:
            print(f"⚠️ Offset이 None 상태로 남아있음. Index: {index_number}")

        if value is None:
            print(f"⚠️ Value가 할당되지 않음. Index: {index_number}, Offset: {offset}")

    def get_value(self, address):
        """ 특정 PLC 주소의 값을 반환 """
        return self.data_store.get(address, None)
    
# CRC check 
def crc16_ccitt(data, length):
        crc = 0xFFFF

        for i in range(length):
            crc ^=(data[i]<<8)
            for _ in range(8):
                if crc & 0x80000: 
                    crc = (crc<<1) ^ 0x1021
                else:
                    crc <<=1
                crc &= 0xFFFF 
        return crc 

class SerialReceiver:
    def __init__(self, port, baudrate=12000000, timeout=1, plc_db_file="plc_db.csv"):
        self.serial_port = serial.Serial(
            port=port, 
            baudrate=baudrate,
            parity=serial.PARITY_EVEN,
            bytesize=serial.EIGHTBITS,
            stopbits=serial.STOPBITS_ONE,
            timeout=timeout,)
        self.data_queue = queue.Queue()
        self.running = True
        self.trdB_flag = 0
        self.plc_data_store = PLCDataStore(plc_db_file)
        self.thread_a = threading.Thread(target=self.thread_a_func)
        self.thread_b = threading.Thread(target=self.thread_b_func)
        self.timeout_counter = 0  
        self.ci = 0
    
    def thread_a_func(self):
        while self.running:
            if self.serial_port.in_waiting:
                if self.trdB_flag==0:
                    data = self.serial_port.read(self.serial_port.in_waiting)
                    if data:
                        self.data_queue.put(data)
                        self.trdB_flag = 1
                        self.timeout_counter = 0
            else:
                self.timeout_counter += 1  
            
            if self.timeout_counter >= 10:
                print("⚠️ 데이터 수신 실패! 유효한 인덱스를 선택하거나 COM 포트 설정을 확인하세요.")
                self.timeout_counter = 0
            
            time.sleep(0.05)

    def thread_b_func(self):
        while self.running:
          if self.trdB_flag == 1:
            if not self.data_queue.empty():
                raw_data = self.data_queue.get()
                self.process_packet(raw_data)
                self.trdB_flag = 0
            time.sleep(0.1)

    # Packet Structure 
    # [0] 0x68 - Start Delimiter 
    # [1] 0xE9 - Length (233) --> [4]~[236]
    # [2] 0xE9 - Length 
    # [3] 0x68 - Start Delimiter 
    # [4] 0xff - Destination Address 
    # [5] 0x80 - Source Address 
    # [6] 0x46 - Function Code
    # [7] 0x30 - DSCAP
    # [8] 0x02 - SSAP 
    # [9] Local Address 
    # [10] Sequence Number 
    # [11] Index 
    # [12] 0xC8 - Timeout
    # [13] 0x03 - Reserved 
    # [14] 0x46 - Reserved
    # [15-16]     Data[0]
    # [17-18]     Data[1]
    # .......
    # [231-232]   Data[108]
    # [233-234]   Data[109]
    # [235-236]   CRC     --> [15] -[234]
    # [237]       Checksum
    # [238]       End Delimiter  


    def process_packet(self, packet):
        if len(packet) < 6:
            return
        
        for i in range(len(packet)-6):
            if packet[i] == 0x68 and packet[i+3] == 0x68:
                if packet[i+1] == packet[i+2]:
                    if i + packet[i+1] + 5 < len(packet):
                        #print(f"packet length: {packet[i+1]}")
                        if packet[i + packet[i+1] + 5] == 0x16:
                            index_number = packet[i + packet[i+1] - 222]  # ✅ index 값을 가져옴
                            if self.ci > 100:
                                if index_number in self.plc_data_store.valid_index_list:
                                    payload = packet[i: i + packet[i+1] + 6]
                                    #CRC check 
                                    if self.verify_crc(payload):
                                        self.plc_data_store.update_data(payload, index_number)
                                       # print("CRC OK")
                                    else:
                                        print(f"❌ CRC 오류 - Index {index_number}: 데이터 폐기")
                                break
                            else:
                                self.ci += 1
                            print(f"ci 값: {self.ci}")  

    #CRC check 

    def verify_crc(self, payload):
        if len(payload) < 6:
            return False 
        length = payload[1]
        data_end = length + 4 
        crc_pos = data_end - 2 
       

        if crc_pos >= len(payload) -1: 
            return False 
        
        received_crc = payload[crc_pos+1] & 0xFF00|(payload[crc_pos] << 8) & 0xFF 
        #received_crc = (payload[crc_pos] << 8)|payload[crc_pos+1] # error for purpose on 
        calculated_crc = crc16_ccitt(payload[4:crc_pos], len(payload[4:crc_pos]))

        return received_crc == calculated_crc
        
     
    def start(self):
        self.thread_a.start()
        self.thread_b.start()
    
    def stop(self):
        self.running = False
        self.thread_a.join()
        self.thread_b.join()
        self.serial_port.close()

    def get_plc_value(self, address):
        """ 특정 주소의 값을 가져오기 """
        return self.plc_data_store.get_value(address)

    def start_monitoring(self):
        """ MW6.0 데이터를 1초마다 읽고 출력 """
        try:
            while self.running:
                value = self.get_plc_value("%MW6.0")
                print(f"🔍 MW6.0 값(HeartBeat for MTP): {value}")  # ✅ 데이터 출력
                time.sleep(1)  # ✅ 1초 대기
        except KeyboardInterrupt:
            print("⏹ 데이터 모니터링 중지됨.")
    

if __name__ == "__main__":
    receiver = SerialReceiver(port="COM4", baudrate=12000000, plc_db_file="BP_APP.csv")
    receiver.start()

    try:
        receiver.start_monitoring()  # ✅ MW6.0 값 1초마다 출력
    except KeyboardInterrupt:
        receiver.stop()
        print("⏹ Serial Receiver Stopped.")
