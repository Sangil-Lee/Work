#!/usr/bin/python

import sys
import argparse
import csv
import re
from epics import PV
from collections import Counter

class AlarmSetting:
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
                    if not row:
                        continue
                    rlines = " ".join(row)
                    if rlines[0] == "#" :
                        continue
                    rlines = re.sub(r"\s*([=])\s*","=", rlines)
                    rlines = re.split(r'[\s,]+', rlines)

                    if len(rlines) > 1:
                        key = rlines[0]
                        values = rlines[1:]
                        if not values:
                            continue
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
            return

        try:
            for key_pvname, alarm_setpoints in self.alarm_dic.items():
                print(f"{key_pvname}:{alarm_setpoints}")
                continue
                for setpoint in alarm_setpoints:
                    setpoint = re.split(r'[=:]+', setpoint)
                    print(setpoint)
        except Exception as e:
            print(f"Check Alarm List File!!: {str(e)}")
            return 
                
    def __getData(self):
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

    def __getDicDataByKey(self, key):
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

    def __compare_list(self, list1, list2):
        main_iter = iter(list1)
        return all(item in main_iter for item in list2)

    def __verifyAlarmRange(self, key):
        org_dict = self.__getDicDataByKey(key)
        excludeKey = ['HYST', 'AFTC']
        filt_key = [key for key in org_dict.keys() if key not in excludeKey]
        org_dict = {key: org_dict[key] for key in filt_key} 
        filt_dict = {key: value for key, value in sorted(org_dict.items(), key=lambda item: item[1] if item[1] != 0 else float('inf'))}
        filt_dict_key_list = [key for key, value in filt_dict.items() if value != 0]
        #print(filt_dict_key_list)

        #values = [value for value in filt_dict.values() if value != 0.0]
        #valcount = Counter(values)
        #print("Values:", values, "ValCount:", valcount)

        #for value, count in valcount.items():
            #if count > 1 :
                #return False

        filt_values = [value for value in filt_dict.values() if value != 0 ]
        if len(filt_values) != len(set(filt_values)): 
                return False

        return self.__compare_list(self.alarm_field_order, filt_dict_key_list)

    def VerifyAlarmRange(self):
        if self.alarm_dic is None:
            return None
        dic_data = self.__getData()
        checkRange = True
        for row, (key, value) in enumerate(dic_data.items()):
            checkRange = self.__verifyAlarmRange(key)
            if checkRange == False:
                print(f"\nLine[{row}] in {self.file_path}: \nAlarm Range Invalid: Confirm PV[{key}] \n ==>> {value}\n")
                break
        return checkRange

    def __getKeys(self):
        if self.alarm_dic is None:
            return None
        return list(self.alarm_dic.keys())

    def GetKeys(self):
        return self.__getKeys()

    def AlarmGettings(self):
        keys = self.__getKeys()
        if keys is None:
            return None
        for index, key in enumerate(keys):
            self.alarmGetting(key)

    def AlarmSettings(self):
        keys = self.__getKeys()
        if keys is None:
            return None
        for index, key in enumerate(keys):
            self.alarmSetting(key)

    def alarmGetting(self, pv_name):
        alarmdict = self.__getDicDataByKey(pv_name)
        alarmfields = self.alarm_fields
        #valuelist = list(alarmdict.values())

        try:
            alarmval = {}
            for index, field in enumerate(alarmfields):
                alarm_name = pv_name+'.'+field
                fieldpv = PV(alarm_name)
                if fieldpv.get() == None:
                    raise Exception(f"Not connected: {alarm_name}")
                fieldval = fieldpv.get()
                alarmval[field]=fieldval
            print(f"{pv_name}:{alarmval}")
        except Exception as e:
            print(f"EPICS Channel Access Connection: {str(e)}")
            sys.exit(1)

    def alarmSetting(self, pv_name):
        alarmdict = self.__getDicDataByKey(pv_name)
        alarmfields = self.alarm_fields
        valuelist = list(alarmdict.values())
        #print(valuelist)

        print(f"{pv_name}:")
        print(f"SetValue: {alarmdict}")
        #print(f"SetValue:", self.__print_dict_fixed(alarmdict, 12, 2))
        try:
            beforedict = {}
            for index, field in enumerate(alarmfields):
                alarm_name = pv_name+'.'+field
                fieldpv = PV(alarm_name)
                if fieldpv.get() == None:
                    raise Exception(f"Not connected: {alarm_name}")
                fieldval = fieldpv.get()
                beforedict[field]=fieldval

            print(f"Before  : {beforedict}")
            afterdict = {}
            for index, field in enumerate(alarmfields):
                alarm_name = pv_name+'.'+field
                self.__alarmAndServerity(pv_name, field, valuelist[index])
                fieldpv = PV(alarm_name)
                appliedval = fieldpv.get()
                afterdict[field]=appliedval
                #if fieldpv.put(valuelist[index],wait=True) == None:
                #    raise Exception(f"Not connected: {alarm_name}")
                #appliedval = fieldpv.get()
                #afterdict[field]=appliedval
            print(f"Applied : {afterdict}")

            if set(beforedict.values()) != set(afterdict.values()):
                result = "Complete changed!!"
            else:
                result = "Same with previous value"

            print(f"Result  : {result}\n")
        except Exception as e:
            print(f"EPICS Channel Access Connection: {str(e)}")
            sys.exit(1)

    def __alarmAndServerity(self, pvname, field, setvalue):
        alarm_name = pvname+'.'+field
        sever_name = ""
        if field == "HIHI":
            sever_name = "HHSV"
        elif field == "HIGH":
            sever_name = "HSV"
        elif field == "LOW":
            sever_name = "LSV"
        elif field == "LOLO":
            sever_name = "LLSV"
        try:
            fieldpv = PV(alarm_name)
            if fieldpv.put(setvalue, wait=True) == None :
                raise Exception(f"Not connected: {alarm_name}")
            if sever_name:
                #print("Server:", sever_name)
                severpv = PV(pvname+'.'+sever_name)
                if sever_name == "HHSV" or sever_name == "LLSV": 
                    if severpv.put("MAJOR", wait=True) == None :
                        raise Exception(f"Not connected: {alarm_name}")
                elif sever_name == "HSV" or sever_name == "LSV": 
                    severpv = PV(pvname+'.'+sever_name)
                    if severpv.put("MINOR", wait=True) == None :
                        raise Exception(f"Not connected: {alarm_name}")
        except Exception as e:
            print(f"EPICS Channel Access Connection: {str(e)}")
            sys.exit(1)

        return True


    def __print_dict_fixed(self, dict_, width, prec):
        for key, value in dict_.items():
            print(f"{key}= {value:{width}.{prec}f}")

def str2bool(s):
    return s.lower() in ('yes', 'true', 't', '1')

if __name__ == "__main__":

    parser = argparse.ArgumentParser(description='Management Alarm Setpoint Configure')
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
            alarmfile = AlarmSetting(args.file)
    except Exception as e:
            print(f"Read error while reading file: {str(e)}")
            sys.exit(1)
    try:
        if args.print is not None:
            alarmfile.Print() 

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
            print(alarmfile.GetKeys())

    except Exception as e:
        print(f"Main Exception: {str(e)}")

