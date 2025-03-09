#!/usr/bin/python

import sys
import argparse
import csv
import re
import xml.etree.ElementTree as etree       # Built-in default package
import xml.dom.minidom                      # Built-in default package
from lxml import etree                     # Need a lxml package(recommend)
from epics import PV
from collections import Counter
from prompt_toolkit import prompt
from prompt_toolkit.key_binding import KeyBindings
from prompt_toolkit.keys import Keys
from prompt_toolkit.completion import WordCompleter
from prompt_toolkit.validation import Validator, ValidationError

class AlarmManagement:
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

    def pretty_print_xml(self, element, filename):
        xml_string = etree.tostring(element, encoding='utf-8').decode('utf-8')
        dom = xml.dom.minidom.parseString(xml_string)
        pretty_xml = dom.toprettyxml(indent='  ', encoding='utf-8')
        with open(filename, 'w', encoding='utf-8') as f:
            f.write(pretty_xml)

    def GenXMLAlarmConfig(self, copy):
        dic_data = self.__getData()
        config = etree.Element("config")
        config.attrib["name"] = "Accelerator"
        component = etree.SubElement(config, "component")
        component.attrib["name"] = "Group"
        prev_val = 0
        for row, (key, value) in enumerate(dic_data.items()):
            val, reminder = divmod(row, 10)
            if prev_val is not val:
                component = etree.SubElement(config, "component")
                component.attrib["name"] = "Group_"+str(val)

            pv = etree.SubElement(component, "pv")
            pv.attrib["name"] = key
            descr = etree.SubElement(pv, "descirption")
            descr.text = key
            enabled = etree.SubElement(pv, "enabled")
            enabled.text = "true"
            latching = etree.SubElement(pv, "latching")
            latching.text = "false"
            prev_val = val
            #print(key, value)

        xtree = etree.ElementTree(config)

        config_file = 'generated_alarm_config.xml'
        if copy is True:
            config_file = "../site-config/alarm_config.xml"

        #case 1:
        #xtree.write("gen_alarm_config.xml", encoding="UTF-8", xml_declaration=True) ## Not pretty lines using ElementTree

        #case 2: best but,Need a lxml package(pip3 install --break-system-packages lxml)
        xtree.write(config_file, encoding="UTF-8", xml_declaration=True, pretty_print=True)

        #case 3: minidom, built-in
        #self.pretty_print_xml(config, "test_alarm.xml")
        print(f"Successful generation alarm config: {config_file}")

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
                #print(f"{field}:", fieldval)
            print(f"{pv_name}:{alarmval}")
        except Exception as e:
            print(f"EPICS Channel Access Connection: {str(e)}")
            sys.exit(1)

    def alarmSetting(self, pv_name):
        alarmdict = self.__getDicDataByKey(pv_name)
        alarmfields = self.alarm_fields
        valuelist = list(alarmdict.values())

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
                fieldpv = PV(alarm_name)
                if fieldpv.put(valuelist[index],wait=True) == None:
                    raise Exception(f"Not connected: {alarm_name}")
                appliedval = fieldpv.get()
                afterdict[field]=appliedval
            print(f"Applied : {afterdict}")

            if set(beforedict.values()) != set(afterdict.values()):
                result = "Complete changed!!"
            else:
                result = "Same with previous value"

            print(f"Result  : {result}\n")
        except Exception as e:
            print(f"EPICS Channel Access Connection: {str(e)}")
            sys.exit(1)

    def __print_dict_fixed(self, dict_, width, prec):
        for key, value in dict_.items():
            print(f"{key}= {value:{width}.{prec}f}")

    def InteractiveAlarmConfig(self, interactive):
        bindings = KeyBindings()

        @bindings.add(Keys.ControlQ)
        def _(event):
            event.app.exit()

        config_name = prompt("[config name]: ", default='Accelerator')
        #print(f"name = {name}")

        config = etree.Element("config")
        config.attrib["name"] = config_name

        for component_name in iter(lambda: prompt("[component name]: ", ), ''):
            component = etree.SubElement(config, "component")
            component.attrib["name"] = component_name
            for pv_name in iter(lambda: prompt("[pv name]: ",), ''):
                pv = etree.SubElement(component, "pv")
                pv.attrib["name"] = pv_name

                descr_name = prompt("description: ")
                if len(descr_name) == 0:
                    descr_name = pv_name
                descr = etree.SubElement(pv, "descirption")
                descr.text = descr_name

                try:
                    enabled_name = prompt("enabled[true or false]: ", validator=TrueFalseValidator(), default='true')
                    enabled = etree.SubElement(pv, "enabled")
                    enabled.text = enabled_name
                    latching_name = prompt("latching[true or false]: ", validator=TrueFalseValidator(), default='false')
                    latching = etree.SubElement(pv, "latching")
                    latching.text = latching_name
                    annunciate_name = prompt("annunciating[true or false]: ", validator=TrueFalseValidator(), default='false')
                    annunciate = etree.SubElement(pv, "annunciating")
                    annunciate.text = annunciate_name

                    delay_name = prompt("delay[number]: ", validator=NumberValidator())

                    if len(delay_name) != 0:
                        delay = etree.SubElement(pv, "delay")
                        delay.text = delay_name

                    count_name = prompt("count[number]: ", validator=NumberValidator())
                    if len(count_name) != 0:
                        count = etree.SubElement(pv, "count")
                        count.text = count_name

                    for guidance_title_name in iter(lambda: prompt("guidance title: ",), ''):
                        if len(guidance_title_name) != 0:
                            guidance = etree.SubElement(pv, "guidance")
                            guidance_title = etree.SubElement(guidance, "title")
                            guidance_title.text = guidance_title_name

                        guidance_details_name = prompt("guidance details: ")
                        if len(guidance_details_name) != 0:
                            guidance_details = etree.SubElement(guidance, "details")
                            guidance_details.text = guidance_details_name

                    for display_title_name in iter(lambda: prompt("display title: ",), ''):
                        if len(display_title_name) != 0:
                            display = etree.SubElement(pv, "display")
                            display_title = etree.SubElement(display, "title")
                            display_title.text = display_title_name

                        display_details_name = prompt("display details: ")
                        if len(display_details_name) != 0:
                            display_details = etree.SubElement(display, "details")
                            display_details.text = display_details_name

                    for command_title_name in iter(lambda: prompt("command title: ",), ''):
                        if len(command_title_name) != 0:
                            command = etree.SubElement(pv, "command")
                            command_title = etree.SubElement(command, "title")
                            command_title.text = command_title_name

                        command_details_name = prompt("command details: ")
                        if len(command_details_name) != 0:
                            command_details = etree.SubElement(command, "details")
                            command_details.text = command_details_name


                    prefixes = ['mailto:', 'cmd:', 'sevrpv:']
                    for automated_action_title_name in iter(lambda: prompt("automated_action title: ",), ''):
                        if len(automated_action_title_name) != 0:
                            automated_action = etree.SubElement(pv, "automated_action")
                            automated_action_title = etree.SubElement(automated_action, "title")
                            automated_action_title.text = automated_action_title_name

                        #automated_action_details_name = prompt("automated_action details: ", validator=NotificationValidator(), default='cmd:')
                        #automated_action_details_name = prompt("automated_action details[mailto:, cmd:, sevrpv:]: ", default='cmd:')
                        automated_action_details_name = prompt("automated_action details: ", validator=NotificationValidator(prefixes), default='cmd:')
                        if len(automated_action_details_name) != 0:
                            automated_action_details = etree.SubElement(automated_action, "details")
                            automated_action_details.text = automated_action_details_name

                        delay_name = prompt("delay[number]: ", validator=NumberValidator())
                        if len(delay_name) != 0:
                            delay = etree.SubElement(automated_action, "delay")
                            delay.text = delay_name

                    continue
                except ValidationError as e:
                    print(e)


        xtree = etree.ElementTree(config)
        xml_file_name = prompt("xml filename: ", default='default_alarm_config.xml')
        xtree.write(xml_file_name, encoding="UTF-8", xml_declaration=True, pretty_print=True)

        #prompt("Exit Ctrl+Q: ", key_bindings=bindings)

class TrueFalseValidator(Validator):
    def validate(self, document):
        text = document.text.lower()
        if text not in ['true', 'false']:
            raise ValidationError(message="true or false.", cursor_position=len(text))

class NumberValidator(Validator):
    def validate(self, document):
        text = document.text
        if not text.isdigit() and len(text) != 0:
            raise ValidationError(message="only number.", cursor_position=len(text))

class NotificationValidator(Validator):
    def __init__(self, prefixes):
        self.prefixes = prefixes

    def validate(self, document):
        text = document.text.lower()
        if not any(text.startswith(prefix) for prefix in self.prefixes):
            raise ValidationError(message="start [mailto: or cmd: or sevrpv:]", cursor_position=0)

        #if text not in ['mailto:', 'cmd:', 'sevrpv:']:
        #    raise ValidationError(message="mailto: or cmd: or sevrpv:", cursor_position=len(text))


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
    parser.add_argument('-genxmlconfig',type=bool, help='Generation initial alarm xml config from pv list')
    parser.add_argument('-copy',        type=bool, help='Copy initial alarm xml config to pas-demo/site-config/alarm_config.xml')
    parser.add_argument('-interactive', type=bool, help='Interactive generation alarm config xml')

    args = parser.parse_args()
    #bprint = str2bool('{}'.format('print' in args))
    print('Parsed arguments: {}'.format(args))
    print('-file',      args.file)
    print('-print',     args.print)
    print('-verify',    args.verify)
    print('-getalarms', args.getalarms)
    print('-setalarms', args.setalarms)
    print('-getkeys',   args.getkeys)
    print('-genxmlconfig',   args.genxmlconfig)
    print('-copy',      args.copy)
    print('-interactive',      args.interactive)

    try :
        if args.file is not None:
            alarmfile = AlarmManagement(args.file)
    except Exception as e:
            print(f"Read error while reading file: {str(e)}")
            sys.exit(1)
    try:
        if args.print is not None:
            alarmfile.Print() 
        #print(alarmfile.GetListDataByKey('ctrlslab:A_Alarm_10'))
        #print(alarmfile.getKeys())

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

        if args.genxmlconfig is not None and args.interactive is None:
            if alarmfile.VerifyAlarmRange() == True:
                print("All Alarm Range Valid!!")
                alarmfile.GenXMLAlarmConfig(args.copy)
        elif args.genxmlconfig is not None and args.interactive is not None:
            if alarmfile.VerifyAlarmRange() == True:
                print("All Alarm Range Valid!!")
                alarmfile.InteractiveAlarmConfig(args.interactive)

    except Exception as e:
        print(f"Main Exception: {str(e)}")

