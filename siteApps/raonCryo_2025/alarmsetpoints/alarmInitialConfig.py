#!/usr/bin/python

import sys
import argparse
import csv
import re
from lxml import etree                     # Need a lxml package(recommend)
from prompt_toolkit import prompt
from prompt_toolkit.key_binding import KeyBindings
from prompt_toolkit.keys import Keys
from prompt_toolkit.completion import WordCompleter
from prompt_toolkit.validation import Validator, ValidationError

class AlarmInitialConfig:
    def __init__(self):
        try:
            self.__interactiveAlarmConfig()
        except Exception as e:
            print(f"File Reading Excption: {str(e)}")

    def __interactiveAlarmConfig(self):
        bindings = KeyBindings()
        @bindings.add(Keys.ControlQ)
        def _(event):
            event.app.exit()
            

        print("***[ Type Following ]***")

        spacer = "  " 
        prompt_config_name = "1.{}{}:".format(spacer, "Config name")
        config_name = prompt(prompt_config_name, default='Accelerator')
        config = etree.Element("config")
        config.attrib["name"] = config_name

        prompt_component_name = "{}{}:".format(spacer, "Component name")
        prompt_pvname_name = "{}{}:".format(spacer, "PV name")
        prompt_desc_name = "{}{}:".format(spacer, "Description")
        prompt_enabled_name = "{}{}:".format(spacer, "Enabled")
        prompt_latch_name = "{}{}:".format(spacer, "Latch")
        prompt_annunciate_name = "{}{}:".format(spacer, "Annunciate")
        prompt_delay_name = "{}{}:".format(spacer, "Alarm Delay[seconds]")
        prompt_count_name = "{}{}:".format(spacer, "Alarm Count[with delay]")
        prompt_guidance_name = "{}{}:".format(spacer, "Guidance Title")
        prompt_guidance_detail_name = "{}{}:".format(spacer, "Guidance Detail")
        prompt_display_name = "{}{}:".format(spacer, "Display Title")
        prompt_display_detail_name = "{}{}:".format(spacer, "Display Detail")
        prompt_command_name = "{}{}:".format(spacer, "Command Title")
        prompt_command_detail_name = "{}{}:".format(spacer, "Command Detail")
        prompt_auto_command_name = "{}{}:".format(spacer, "Automated Command Title")
        prompt_auto_command_detail_name = "{}{}:".format(spacer, "Automated Dommand Detail")
        prompt_auto_command_delay_name = "{}{}:".format(spacer, "Automated Command Delay")

        prompt_xml_file_name = "{}{}:".format(spacer, "Save xml file name")

        com_index = 1
        for component_name in iter(lambda: prompt("1.{}.{}".format(com_index, prompt_component_name),), ''):
            pv_index = 1
            component = etree.SubElement(config, "component")
            component.attrib["name"] = component_name
            #for index, pv_name in enumerate(iter(lambda: prompt("   [pv name]: ",), '')):
            for pv_name in iter(lambda: prompt("1.{}.{}.{}".format(com_index, pv_index, prompt_pvname_name),), ''):
                #test = "{}:{}".format(index, pv_name)
                pv = etree.SubElement(component, "pv")
                pv.attrib["name"] = pv_name

                field_index = 1
                descr_name = prompt("1.{}.{}-{}.{}".format(com_index, pv_index, field_index, prompt_desc_name),)
                if len(descr_name) == 0:
                    descr_name = pv_name
                descr = etree.SubElement(pv, "description")
                descr.text = descr_name

                field_index += 1
                try:
                    enabled_name = prompt("1.{}.{}-{}.{}".format(com_index, pv_index, field_index, prompt_enabled_name), validator=TrueFalseValidator(), default='true')
                    enabled = etree.SubElement(pv, "enabled")
                    enabled.text = enabled_name

                    field_index += 1
                    latching_name = prompt("1.{}.{}-{}.{}".format(com_index, pv_index, field_index, prompt_latch_name), validator=TrueFalseValidator(), default='false')
                    latching = etree.SubElement(pv, "latching")
                    latching.text = latching_name

                    field_index += 1
                    annunciate_name = prompt("1.{}.{}-{}.{}".format(com_index, pv_index, field_index, prompt_annunciate_name), validator=TrueFalseValidator(), default='false')
                    annunciate = etree.SubElement(pv, "annunciating")
                    annunciate.text = annunciate_name

                    field_index += 1
                    delay_name = prompt("1.{}.{}-{}.{}".format(com_index, pv_index, field_index, prompt_delay_name), validator=NumberValidator())

                    if len(delay_name) != 0:
                        delay = etree.SubElement(pv, "delay")
                        delay.text = delay_name

                    field_index += 1
                    count_name = prompt("1.{}.{}-{}.{}".format(com_index, pv_index, field_index, prompt_count_name), validator=NumberValidator())
                    if len(count_name) != 0:
                        count = etree.SubElement(pv, "count")
                        count.text = count_name

                    field_index += 1
                    guide_index = 1
                    for guidance_title_name in iter(lambda: prompt("1.{}.{}-{}.{}.{}".format(com_index, pv_index, field_index, guide_index, prompt_guidance_name),), ''):
                        guide_detail_index = 0
                        if len(guidance_title_name) != 0:
                            guidance = etree.SubElement(pv, "guidance")
                            guidance_title = etree.SubElement(guidance, "title")
                            guidance_title.text = guidance_title_name

                        guide_detail_index += 1
                        guidance_details_name = prompt("1.{}.{}-{}.{}.{}.{}".format(com_index, pv_index, field_index, guide_index, guide_detail_index, prompt_guidance_detail_name),)
                        if len(guidance_details_name) != 0:
                            guidance_details = etree.SubElement(guidance, "details")
                            guidance_details.text = guidance_details_name
                        guide_index += 1

                    field_index += 1
                    display_index = 1
                    for display_title_name in iter(lambda: prompt("1.{}.{}-{}.{}.{}".format(com_index, pv_index, field_index, display_index, prompt_display_name),), ''):
                        display_detail_index = 0
                        if len(display_title_name) != 0:
                            display = etree.SubElement(pv, "display")
                            display_title = etree.SubElement(display, "title")
                            display_title.text = display_title_name

                        display_detail_index += 1
                        display_details_name = prompt("1.{}.{}-{}.{}.{}.{}".format(com_index, pv_index, field_index, display_index, display_detail_index, prompt_display_detail_name),)
                        if len(display_details_name) != 0:
                            display_details = etree.SubElement(display, "details")
                            display_details.text = display_details_name
                        display_index += 1

                    field_index += 1
                    command_index = 1
                    for command_title_name in iter(lambda: prompt("1.{}.{}-{}.{}.{}".format(com_index, pv_index, field_index, command_index, prompt_command_name),), ''):
                        command_detail_index = 0
                        if len(command_title_name) != 0:
                            command = etree.SubElement(pv, "command")
                            command_title = etree.SubElement(command, "title")
                            command_title.text = command_title_name

                        command_detail_index +=1
                        command_details_name = prompt("1.{}.{}-{}.{}.{}.{}".format(com_index, pv_index, field_index, command_index, command_detail_index, prompt_command_detail_name),)
                        if len(command_details_name) != 0:
                            command_details = etree.SubElement(command, "details")
                            command_details.text = command_details_name
                        command_index += 1

                    prefixes = ['mailto:', 'cmd:', 'sevrpv:']
                    field_index += 1
                    auto_command_index = 1
                    for automated_action_title_name in iter(lambda: prompt("1.{}.{}-{}.{}.{}".format(com_index, pv_index, field_index, auto_command_index, prompt_auto_command_name),), ''):
                        auto_command_detail_index = 0
                        if len(automated_action_title_name) != 0:
                            automated_action = etree.SubElement(pv, "automated_action")
                            automated_action_title = etree.SubElement(automated_action, "title")
                            automated_action_title.text = automated_action_title_name

                        auto_command_detail_index += 1
                        automated_action_details_name = prompt("1.{}.{}-{}.{}.{}.{}".format(com_index, pv_index, field_index, auto_command_index, auto_command_detail_index, prompt_auto_command_detail_name), validator=NotificationValidator(prefixes), default='cmd:')

                        if len(automated_action_details_name) != 0:
                            automated_action_details = etree.SubElement(automated_action, "details")
                            automated_action_details.text = automated_action_details_name

                        auto_command_detail_index += 1
                        delay_name = prompt("1.{}.{}-{}.{}.{}.{}".format(com_index, pv_index, field_index, auto_command_index, auto_command_detail_index, prompt_auto_command_delay_name), validator=NumberValidator())
                        if len(delay_name) != 0:
                            delay = etree.SubElement(automated_action, "delay")
                            delay.text = delay_name
                        auto_command_index += 1
                    pv_index += 1
                    continue
                except ValidationError as e:
                    print(e)

            com_index += 1
        xtree = etree.ElementTree(config)
        xml_file_name = prompt(prompt_xml_file_name, default="default_alarm_config.xml")
        xtree.write(xml_file_name, encoding="UTF-8", xml_declaration=True, pretty_print=True)
        #prompt("   Exit Ctrl+Q: ", key_bindings=bindings)

    @staticmethod
    def GenXMLAlarmConfigByGroup(file_path, copy):
        data_dict = {}
        current_key = None
        current_value = []
        config_name = ""
        try:
            file_name = file_path
            if re.search(r"\.csv$",file_name, re.IGNORECASE) is None:
                raise Exception("File extension is not csv!!")
            with open(file_path, 'r', encoding='utf-8') as file:
                csv_reader = csv.reader(file)
                header = next(csv_reader)
                pv_list = []
                for row in csv_reader:
                    if not row:
                        continue
                    rlines = " ".join(row)
                    if rlines[0] == "#" :
                        continue
                    rlines = re.sub(r"\s*([=])\s*","=", rlines)
                    rlines = re.split(r'[\s=\[\]]+', rlines)
                    if rlines[0].lower() == "config" :
                        config_name = rlines[1]
                    elif rlines[0].lower() == "component" :
                        key=rlines[1]
                        #print(key)
                        if current_key is not None:
                            data_dict[current_key] = ';'.join(current_value)
                        current_key = key
                        current_value = []
                    elif current_key is not None:
                        #print(rlines)
                        current_value.append(rlines[0])
                    else:
                        raise Exception("Need a definition for component name!!")

                if current_key is not None:
                    data_dict[current_key] = ';'.join(current_value)

            file.close()
            #print(data_dict)

            config = etree.Element("config")
            config.attrib["name"] = config_name
            setkey = set()
            for dicindex, (keys, values) in enumerate(data_dict.items()):
                #print(keys,"===>>")
                keys = re.split(r'[\s:;]+', keys)
                values = re.split(r'[\s;]+', values)
                for index, key in enumerate(keys):
                    if index == 0 :
                        if not key in setkey:
                            component = etree.SubElement(config, "component")
                            component.attrib["name"] = key
                        setkey.add(key)
                        if len(keys) == 1:
                            for value in values:
                                #print(value)
                                pv = etree.SubElement(component, "pv")
                                pv.attrib["name"] = value
                                descr = etree.SubElement(pv, "description")
                                descr.text = value
                                enabled = etree.SubElement(pv, "enabled")
                                enabled.text = "true"
                                latching = etree.SubElement(pv, "latching")
                                latching.text = "false"
                                annunciating = etree.SubElement(pv, "annunciating")
                                annunciating.text = "false"
                            continue
                    else :
                        print(f"Dicindex({dicindex}), Key[{index}]: {key}")
                        subcomp = etree.SubElement(component, "component")
                        subcomp.attrib["name"] = key
                        for value in values:
                            #print(value)
                            pv = etree.SubElement(subcomp, "pv")
                            pv.attrib["name"] = value
                            descr = etree.SubElement(pv, "description")
                            descr.text = value
                            enabled = etree.SubElement(pv, "enabled")
                            enabled.text = "true"
                            latching = etree.SubElement(pv, "latching")
                            latching.text = "false"
                            annunciating = etree.SubElement(pv, "annunciating")
                            annunciating.text = "false"

            xtree = etree.ElementTree(config)
            config_file = 'generated_alarm_config.xml'
            if copy is True:
                config_file = "../site-config/alarm_config.xml"

            xtree.write(config_file, encoding="UTF-8", xml_declaration=True, pretty_print=True)

            config_names = 'config_names'
            file_path = '../site-config/phoebus_settings.ini'

            if AddingConfigName(file_path, config_names, config_name):
                print(f"'{file_path}': Adding '{config_name}' in {config_names}.")

            else:
                print(f"'{file_path}': Already exists '{config_name}' in {config_names}.")

            print(f"Successful generation alarm config: {config_file}")
        except FileNotFoundError:
            print(f"File Not Found: {file_path}")
            return None
        except Exception as e:
            print(f"Read error while reading file: {str(e)}")
            return None

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

def AddingConfigName(filepath, search_string, configname):
    try:
        with open(filepath, 'r+', encoding='utf-8') as file:
            lines = file.readlines()
            file.seek(0)
            file.truncate()

            added = False

            for line in lines:
                if search_string in line:
                    if configname not in line:
                        file.write(line.rstrip()+","+ configname + '\n')
                        added = True
                    else:
                        file.write(line)
                        added = False
                else:
                    file.write(line)
            return added

    except FileNotFoundError:
        print(f"Error: File not found: {filepath}")
        return False


def str2bool(s):
    return s.lower() in ('yes', 'true', 't', '1')

def check_all_none(args):
    for arg in vars(args).values():
        if arg is not None:
            return False
    return True

def read_csv_with_next_line(file_path):
    try:
        with open(file_path, 'r', newline='', encoding='utf-8') as csvfile:
            reader = csv.reader(csvfile)
            lines = list(reader)  # 모든 줄을 메모리에 저장

            for i, current_line in enumerate(lines):
                next_line = lines[i + 1] if i + 1 < len(lines) else None
                yield current_line, next_line

    except FileNotFoundError:
        print(f"오류: 파일을 찾을 수 없습니다: {file_path}")
    except Exception as e:
        print(f"오류: 파일 처리 중 오류 발생: {e}")



if __name__ == "__main__":

    parser = argparse.ArgumentParser(description='Initialize Alarm Config XML File')
    parser.add_argument('-file',        help='Generation alarm config xml file from alarm setpoints setting file(csv file, i.e., alarm_config.csv)')
    parser.add_argument('-copy',        type=bool, help='Copy initial alarm xml config to pas-demo/site-config/alarm_config.xml')
    args = parser.parse_args()

    print('Parsed arguments: {}'.format(args))
    print('-file',          args.file)
    print('-copy',          args.copy)


    try :
        if check_all_none(args) :
            config = AlarmInitialConfig()
        elif args.file is not None:
            AlarmInitialConfig.GenXMLAlarmConfigByGroup(args.file, args.copy)

    except KeyboardInterrupt:
        print("Program Exit==> Not generated alarm config xml")
        sys.exit(1)
    except Exception as e:
        print(f"Main Exception: {str(e)}")
        sys.exit(1)
