#!/usr/bin/python

import sys
from epics import PV
import argparse
import csv
import re
from collections import Counter


class AlarmFileReader:
    def __init__(self, file_path):
        try:
            self.alarm_dic = self.read_file(file_path)
            if self.alarm_dic is None:
                raise Exception("Check Alarm List File!!")
        except Exception as e:
            print(f"File Reading Excption: {str(e)}")

        self.alarm_field_order = ['LOLO', 'LOW', 'HIGH', 'HIHI']
        self.alarm_fields = ['LOLO', 'LOW', 'HIGH', 'HIHI', 'HYST', 'AFTC']
        self.data = {}

    def checkValidKey(self, key):
        return key.upper() in ('HIHI', 'HIGH', 'LOW', 'LOLO', 'HYST', 'AFTC')

    def read_file(self, file_path):
        alarm_dic = {}
        self.file_path = file_path
        try:
            with open(self.file_path, 'r', encoding='utf-8') as file:
                csv_reader = csv.reader(file)
                header = next(csv_reader)
                for row in csv_reader:
                    # Remove white space for front, backend of special character(=:)
                    rlines = " ".join(row)
                    if rlines[0] == "#" :
                        continue
                    rlines = re.sub(r"\s*([=])\s*","=", rlines)
                    rlines = re.split(r'[\s,]+', rlines)

                    if len(rlines) > 1:
                        key = rlines[0]
                        values = rlines[1:]
                        default_alarm_fields = {'LOLO':'0.0', 'LOW':'0.0', 'HIGH':'0.0', 'HIHI':'0.0', 'HYST':'0.0', 'AFTC':'0.0'}
                        alarmvalues = list(default_alarm_fields.items())
                        #print(values)

                        for fieldval in values:
                            fieldval = re.split(r'[\s=]+',fieldval)
                            field = fieldval[0].upper()
                            if self.checkValidKey(field) == False:
                                print(f"Invalid Alarm Field in [{key}]")
                                raise Exception(f"Invalid Alarm Field in [{key}]")
                            value = fieldval[1]
                            default_alarm_fields[field]=value
                        
                        alarmkeys   = list(default_alarm_fields.keys())
                        alarmvalues = list(default_alarm_fields.values())
                        alarmfieldvalues = [key+"="+value for key, value in zip(alarmkeys, alarmvalues)]
                        #print(alarmfieldvalues)
                        if alarm_dic.get(key) is not None:
                            print(f"Conflict PV Key ==>>> {key}")
                            raise Exception("Conflict PV Key") 

                        alarm_dic[key]=alarmfieldvalues

                        #alarm_dic[key]=values
            file.close()
        except FileNotFoundError:
            print(f"File Not Found: {self.file_path}")
            return None
        except Exception as e:
            print(f"Read error while reading file: {str(e)}")
            return None
        return alarm_dic

    def Print(self):
        print("Finished read file")
        if self.alarm_dic is None:
            print(self.alarm_dic)
            return

        try:
            for key_pvname, alarm_setpoints in self.alarm_dic.items():
                print(f"\n{key_pvname}==>")

                for setpoint in alarm_setpoints:
                    setpoint = re.split(r'[=:]+', setpoint)
                    print(setpoint)
        except Exception as e:
            print(f"Check Alarm List File!!: {str(e)}")
            return 
                
    def GetData(self):
        if self.alarm_dic:
            return self.alarm_dic
        return None

    def GetListDataByKey(self, key):
        if self.alarm_dic is None:
            return None
        values = self.alarm_dic.get(key)
        if values is None:
            return None
        return values

    def getDicDataByKey(self, key):
        if self.alarm_dic is None:
            return None
        values = self.alarm_dic.get(key)
        if values is None:
            return None
        dic_vals = {}
        for i, vals in enumerate(values):
            vals = re.split(r'[=:]+', vals)
            validkey = self.checkValidKey(vals[0])
            if validkey == False:
                print(f"Invalid Alarm Field in [{key}]")
                return None
            dic_vals[vals[0]] = float(vals[1])
        return dic_vals

    def veryfyAlarmRange(self, key):
        org_dict = self.getDicDataByKey(key)
        excludeKey = ['HYST', 'AFTC']
        filt_key = [key for key in org_dict.keys() if key not in excludeKey]
        org_dict = {key: org_dict[key] for key in filt_key} 
        filt_dict = {key: value for key, value in sorted(org_dict.items(), key=lambda item: item[1])}
        filt_dict_key_list = list(filt_dict.keys())

        values = [value for value in filt_dict.values() if value != 0.0]
        valcount = Counter(values)
        #print("Values:", values, "ValCount:", valcount)

        for value, count in valcount.items():
            if count > 1 :
                return False

        return (self.alarm_field_order == filt_dict_key_list)

    def VerifyAlarmRange(self):
        if self.alarm_dic is None:
            return None
        dic_data = self.GetData()
        checkRange = True
        for row, (key, value) in enumerate(dic_data.items()):
            checkRange = self.veryfyAlarmRange(key)
            if checkRange == False:
                print(f"\nLine[{row}] in {self.file_path}: \nAlarm Range Invalid: Confirm PV[{key}] \n ==>> {value}\n")
                break
        return checkRange

    def getKeys(self):
        if self.alarm_dic is None:
            return None

        return list(self.alarm_dic.keys())

    def AlarmGettings(self):
        keys = self.getKeys()
        if keys is None:
            return None
        for index, key in enumerate(keys):
            self.alarmGetting(key)

    def AlarmSettings(self):
        keys = self.getKeys()
        if keys is None:
            return None
        for index, key in enumerate(keys):
            self.alarmSetting(key)

    def alarmGetting(self, pv_name):
        alarmdict = self.getDicDataByKey(pv_name)
        alarmfields = self.alarm_fields
        valuelist = list(alarmdict.values())

        print(f"{pv_name}:===>Getting")
        try:
            for index, field in enumerate(alarmfields):
                alarm_name = pv_name+'.'+field
                fieldpv = PV(alarm_name)
                if fieldpv.get() == None:
                    raise Exception(f"Not connected: {alarm_name}")
                fieldval = fieldpv.get()
                print(f"{field}:", fieldval)
        except Exception as e:
            print(f"EPICS Channel Access Connection: {str(e)}")
            sys.exit(1)

    def alarmSetting(self, pv_name):
        alarmdict = self.getDicDataByKey(pv_name)
        alarmfields = self.alarm_fields
        valuelist = list(alarmdict.values())

        print(f"{pv_name}:===>Set")
        try:
            for index, field in enumerate(alarmfields):
                alarm_name = pv_name+'.'+field
                fieldpv = PV(alarm_name)
                if fieldpv.put(valuelist[index],wait=True) == None:
                    raise Exception(f"Not connected: {alarm_name}")
                fieldval = fieldpv.get()
                print(f"{field}:", fieldval)
        except Exception as e:
            print(f"EPICS Channel Access Connection: {str(e)}")
            sys.exit(1)

def str2bool(s):
    return s.lower() in ('yes', 'true', 't', '1')

if __name__ == "__main__":

    parser = argparse.ArgumentParser(description='Alarm Setpoint Configure')
    parser.add_argument('-file',        help='Alarm list for changing alarm setpoints')
    parser.add_argument('-print',       type=bool, help='Alarm display')
    parser.add_argument('-verify',      type=bool, help='Alarm Range Verification')
    parser.add_argument('-getalarms',   type=bool, help='Getting alarm field values for all pv list')
    parser.add_argument('-setalarms',   type=bool, help='Setting alarm field values for all pv list')
    parser.add_argument('-getkeys',     type=bool, help='Get PVname keys from pv list')

    args = parser.parse_args()
    #bprint = str2bool('{}'.format('print' in args))
    print('Parsed arguments: {}'.format(args))
    print('-file',      args.file)
    print('-print',     args.print)
    print('-verify',    args.verify)
    print('-getalarms', args.getalarms)
    print('-setalarms', args.setalarms)
    print('-getkeys',   args.getkeys)

    try :
        if args.file is not None:
            alarmfile = AlarmFileReader(args.file)
    except Exception as e:
            print(f"Read error while reading file: {str(e)}")
            sys.exit(1)
    try:
        if args.print is not None:
            alarmfile.Print() 
        #print(alarmfile.GetListDataByKey('ctrlslab:A_Alarm_10'))
        #print(alarmfile.getDicDataByKey('ctrlslab:A_Alarm_101'))
        #print(alarmfile.getKeys())
        #org_dict = alarmfile.getDicDataByKey('ctrlslab:A_Alarm_10')

        if args.verify is True:
            if alarmfile.VerifyAlarmRange() == True:
                print("All Alarm Range Valid!!")

        if args.getalarms is not None:
            if alarmfile.VerifyAlarmRange() == True:
                print("All Alarm Range Valid!!")
                alarmfile.AlarmGettings()

        if args.setalarms is not None:
            if alarmfile.VerifyAlarmRange() == True:
                print("All Alarm Range Valid!!")
                alarmfile.AlarmSettings()

        if args.getkeys is not None:
            print(alarmfile.getKeys())

    except KeyboardInterrupt:
        print(" Serial Receiver Stopped.")

