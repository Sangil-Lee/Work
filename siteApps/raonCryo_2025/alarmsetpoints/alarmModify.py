#!/usr/bin/python

import sys
import argparse
import csv
import re
#from lxml import etree                     # Need a lxml package(recommend)
import lxml.etree as et                     # Need a lxml package(recommend)
from prompt_toolkit import prompt
from prompt_toolkit.key_binding import KeyBindings
from prompt_toolkit.keys import Keys
from prompt_toolkit.completion import WordCompleter
from prompt_toolkit.validation import Validator, ValidationError

class AlarmInitialConfig:
    def __init__(self, modify=None, file_path=None):
        try:
            print("***[ Type Follow for PV Alarm Property Setting. to exit program:ctrl+c ]***")
            if modify == None:
                self.__interactiveAlarmConfig()
            else:
                self.__interactiveAlarmConfigModify(file_path)
        except Exception as e:
            print(f"File Reading Excption: {str(e)}")

    def __getSafe(self, slist, index, default=""):
        if 0 <= index < len(slist):
            return slist[index]
        else:
            return default

    def __interactiveAlarmConfigModify(self, file_path):

        try:
            with open(file_path, "r"):
                print("File founded: ", file_path)
        except FileNotFoundError:
            print("File not founded: ", file_path)

        if self.__is_xml_file_contents(file_path) == False or self.__is_xml_file(file_path) == False:
            print("File Error!!: may be not xml file ", file_path)
            return

        eparser = et.XMLParser(remove_blank_text=True)
        tree = et.parse(file_path, eparser)
        root = tree.getroot()

        print("***[ Type Follow to Modify PV Alarm Property ]***")

        index = 1
        spacer = "  " 
        comp_name=""
        prompt_comp_name = "1.{}{}:".format(spacer, "Type Component[Group:Alarm Area Panel] name")
        prompt_pv_name = "{}{}:".format(spacer, "Type PV name")
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

        prompt_xml_file_name = "{}{}:".format(spacer, "Save new xml file name ?")


        while True:
            comp_name = prompt(prompt_comp_name)
            targetcomp = root.xpath(f"//component[@name='{comp_name}']")
            if not targetcomp :
                print(f"Can't not find component name: {comp_name}, to exit program(ctrl+c)")
                continue
            break

        pv_name=""
        while True:
            pv_name = prompt("1.{}.{}".format(index,  prompt_pv_name))
            if not pv_name:
                exit_name = prompt("Are you sure exit? ", validator=YesNoValidator(), default='yes')
                if exit_name == 'yes' :
                    break
                else:
                    continue
            targetpv = root.xpath(f"//pv[@name='{pv_name}']")
            if not targetpv :
                print(f"Can't not find pv node name: {pv_name} in {comp_name}")
                continue

            for pv_node in root.xpath(f"//component[@name='{comp_name}']") and root.xpath(f"//pv[@name='{pv_name}']"):

                #for pv_field in pv_node.getchildren():
                    #print("Tag Name:", pv_field.tag, "Tag Text:", pv_field.text)
                    #if pv_field.tag == "guidance" or pv_field.tag == "display" or pv_field.tag == "command" :
                        #for sub_field in pv_field.getchildren():
                            #print("Sub_Tag Name:", sub_field.tag, "Sub_Tag Text:", sub_field.text)
                #guide_list = self.__getAttrtext(pv_node, "guidance")
                #print(guide_list)
                #return

                #for pv_field in pv_node.getchildren():
                    #pv_node.remove(pv_field)

                field_index = 1
                descr, strDefault = self.__getAttrtext(pv_node,"description")
                descr_name = prompt("1.{}.{}.{}".format(index, field_index, prompt_desc_name), default=strDefault)
                if len(strDefault) == 0:
                    descr = et.SubElement(pv_node, "description")

                if len(descr_name) == 0:
                    descr_name = pv_name
                descr.text = descr_name
                print(descr.text)


                field_index += 1
                enabled, strDefault = self.__getAttrtext(pv_node,"enabled")
                enabled_name = prompt("1.{}.{}.{}".format(index, field_index, prompt_enabled_name), validator=TrueFalseValidator(), default=strDefault)

                if len(strDefault) == 0:
                    enabled = et.SubElement(pv_node, "enabled")

                enabled.text = enabled_name
                print(enabled.text)

                field_index += 1
                latching, strDefault = self.__getAttrtext(pv_node,"latching")
                latching_name = prompt("1.{}.{}.{}".format(index, field_index, prompt_latch_name), validator=TrueFalseValidator(), default=strDefault)

                if len(strDefault) == 0:
                    latching = et.SubElement(pv_node, "latching")

                latching.text = latching_name
                print(latching.text)

                field_index += 1
                annunciating, strDefault = self.__getAttrtext(pv_node,"annunciating")
                annunciating_name = prompt("1.{}.{}.{}".format(index, field_index, prompt_annunciate_name), validator=TrueFalseValidator(), default=strDefault)

                if len(strDefault) == 0:
                    annunciating = et.SubElement(pv_node, "annunciating")

                annunciating.text = annunciating_name
                print(annunciating.text)

                field_index += 1
                delay, strDefault = self.__getAttrtext(pv_node,"delay")
                delay_name = prompt("1.{}.{}.{}".format(index, field_index, prompt_delay_name), validator=NumberValidator(), default=strDefault)

                if len(strDefault) == 0:
                    delay = et.SubElement(pv_node, "delay")

                delay.text = delay_name
                print(delay.text)

                field_index += 1
                count, strDefault = self.__getAttrtext(pv_node,"count")
                count_name = prompt("1.{}.{}.{}".format(index, field_index, prompt_count_name), validator=NumberValidator(), default=strDefault)
                
                if len(strDefault) == 0:
                    count = et.SubElement(pv_node, "count")

                count.text = count_name
                print(count.text)

                field_index += 1
                guide_index = 1
                guide_list = self.__getAttrtext(pv_node, "guidance")
                print(guide_list)
                guide_keys = list(guide_list.keys())
                guide_values = list(guide_list.values())

                try:
                    for guidance_title_name in iter(lambda: prompt("1.{}.{}.{}.{}".format(index, field_index, guide_index, prompt_guidance_name), default=self.__getSafe(guide_keys, guide_index-1)), ''):
                        guide_detail_index = 0

                        if len(guidance_title_name) != 0 and len(guide_keys) < guide_index :
                            guidance = et.SubElement(pv_node, "guidance")
                            guidance_title = et.SubElement(guidance, "title")
                            guidance_title.text = guidance_title_name

                        else:
                            subelement = self.__getSubelement(pv_node, "guidance", self.__getSafe(guide_keys, guide_index-1))
                            subelement.text = guidance_title_name

                        guide_detail_index += 1
                        guidance_details_name = prompt("1.{}.{}.{}.{}.{}".format(index, field_index, guide_index, guide_detail_index, prompt_guidance_detail_name), default=self.__getSafe(guide_values, guide_index-1) )
                        if len(guidance_details_name) != 0 and len(guide_values) < guide_index :
                            guidance_details = et.SubElement(guidance, "details")
                            guidance_details.text = guidance_details_name
                        else:
                            subelement = self.__getSubelement(pv_node, "guidance", self.__getSafe(guide_values, guide_index-1))
                            subelement.text = guidance_details_name

                        guide_index += 1
                except IndexError:
                    print("out of list index")


                field_index += 1
                display_index = 1
                display_list = self.__getAttrtext(pv_node, "display")
                display_keys = list(display_list.keys())
                display_values = list(display_list.values())

                for display_title_name in iter(lambda: prompt("1.{}.{}.{}.{}".format(index, field_index, display_index, prompt_display_name), default=self.__getSafe(display_keys, display_index-1)), ''):
                    display_detail_index = 0
                    if len(display_title_name) != 0 and len(display_keys) < display_index :
                        display = et.SubElement(pv_node, "display")
                        display_title = et.SubElement(display, "title")
                        display_title.text = display_title_name
                    else:
                        subelement = self.__getSubelement(pv_node, "display", self.__getSafe(display_keys, display_index-1))
                        subelement.text = display_title_name


                    display_detail_index += 1
                    display_details_name = prompt("1.{}.{}.{}.{}.{}".format(index, field_index, display_index, display_detail_index, prompt_display_detail_name), default=self.__getSafe(display_values, display_index-1) )
                    if len(display_details_name) != 0 and len(display_values) < display_index:
                        display_details = et.SubElement(display, "details")
                        display_details.text = display_details_name
                    else:
                        subelement = self.__getSubelement(pv_node, "display", self.__getSafe(display_values, display_index-1))
                        subelement.text = display_details_name

                    display_index += 1

                field_index += 1
                command_index = 1
                command_list = self.__getAttrtext(pv_node, "command")
                command_keys = list(command_list.keys())
                command_values = list(command_list.values())
                for command_title_name in iter(lambda: prompt("1.{}.{}.{}.{}".format(index, field_index, command_index, prompt_command_name), default=self.__getSafe(command_keys, command_index-1)), ''):
                    command_detail_index = 0
                    if len(command_title_name) != 0 and len(command_keys) < command_index:
                        command = et.SubElement(pv_node, "command")
                        command_title = et.SubElement(command, "title")
                        command_title.text = command_title_name
                    else:
                        subelement = self.__getSubelement(pv_node, "command", self.__getSafe(command_keys, command_index-1))
                        subelement.text = command_title_name

                    command_detail_index +=1
                    command_details_name = prompt("1.{}.{}.{}.{}.{}".format(index, field_index, command_index, command_detail_index, prompt_command_detail_name), default=self.__getSafe(command_values, command_index-1) )
                    if len(command_details_name) != 0 and len(command_values) < command_index:
                        command_details = et.SubElement(command, "details")
                        command_details.text = command_details_name
                    else:
                        subelement = self.__getSubelement(pv_node, "command", self.__getSafe(command_values, command_index-1))
                        subelement.text = command_details_name
                    command_index += 1

                prefixes = ['mailto:', 'cmd:', 'sevrpv:']
                field_index += 1
                auto_command_index = 1
                auto_command_list = self.__getAttrtext(pv_node, "automated_action")
                auto_command_keys = list(auto_command_list.keys())
                auto_command_values = list(auto_command_list.values())
                for automated_action_title_name in iter(lambda: prompt("1.{}.{}.{}.{}".format(index, field_index, auto_command_index, prompt_auto_command_name), default=self.__getSafe(auto_command_keys, auto_command_index-1)), ''):
                    auto_command_detail_index = 0
                    if len(automated_action_title_name) != 0 and len(auto_command_keys) < auto_command_index :
                        automated_action = et.SubElement(pv_node, "automated_action")
                        automated_action_title = et.SubElement(automated_action, "title")
                        automated_action_title.text = automated_action_title_name
                    else:
                        subelement = self.__getSubelement(pv_node, "automated_action", self.__getSafe(auto_command_keys, auto_command_index-1))
                        subelement.text = command_title_name
                        

                    auto_command_detail_index += 1
                    #automated_action_details_name = prompt("1.{}.{}.{}.{}.{}".format(index, field_index, auto_command_index, auto_command_detail_index, prompt_auto_command_detail_name), validator=NotificationValidator(prefixes), default='cmd:')
                    automated_action_details_name = prompt("1.{}.{}.{}.{}.{}".format(index, field_index, auto_command_index, auto_command_detail_index, prompt_auto_command_detail_name), default=self.__getSafe(auto_command_values, auto_command_index-1), validator=NotificationValidator(prefixes))

                    if len(automated_action_details_name) != 0 and len(auto_command_values) < auto_command_index :
                        automated_action_details = et.SubElement(automated_action, "details")
                        automated_action_details.text = automated_action_details_name
                    else:
                        subelement = self.__getSubelement(pv_node, "automated_action", self.__getSafe(auto_command_values, auto_command_index-1))
                        subelement.text = command_details_name

                    auto_command_detail_index += 1
                    delay_name = prompt("1.{}.{}.{}.{}.{}".format(index, field_index, auto_command_index, auto_command_detail_index, prompt_auto_command_delay_name), validator=NumberValidator())
                    if len(delay_name) != 0:
                        delay = et.SubElement(automated_action, "delay")
                        delay.text = delay_name
                    auto_command_index += 1
                index += 1
            continue

        xml_file_name = prompt(prompt_xml_file_name, default="default_alarm_config.xml")
        if xml_file_name == "":
            return
        tree.write(xml_file_name, encoding="UTF-8", xml_declaration=True, pretty_print=True)

    def __getAttrtext(self, node, tagname):
        subfields = {} 
        for field in node.getchildren():
            if field.tag.lower() in ["guidance", "display", "command", "automated_action"] :
                if field.tag == tagname :
                    for index, sub_field in enumerate(field.getchildren()):
                        if index % 2 == 0:
                            key = sub_field.text
                        else:
                            value = sub_field.text
                    subfields[key]=value
            elif field.tag == tagname:
                return field, field.text

        return subfields

    def __getSubelement(self, node, tagname, subtag_text):
        for field in node.getchildren():
            if field.tag.lower() in ["guidance", "display", "command", "automated_action"] :
                if field.tag == tagname :
                    for index, sub_field in enumerate(field.getchildren()):
                        if sub_field.text == subtag_text :
                            return sub_field
        return None

    def __interactiveAlarmConfig(self):
        bindings = KeyBindings()
        @bindings.add(Keys.ControlQ)
        def _(event):
            event.app.exit()
            

        print("***[ Type Following ]***")

        spacer = "  " 
        prompt_config_name = "1.{}{}:".format(spacer, "Config name")
        config_name = prompt(prompt_config_name, default='Accelerator')
        config = et.Element("config")
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
            component = et.SubElement(config, "component")
            component.attrib["name"] = component_name
            #for index, pv_name in enumerate(iter(lambda: prompt("   [pv name]: ",), '')):
            for pv_name in iter(lambda: prompt("1.{}.{}.{}".format(com_index, pv_index, prompt_pvname_name),), ''):
                #test = "{}:{}".format(index, pv_name)
                pv = et.SubElement(component, "pv")
                pv.attrib["name"] = pv_name

                field_index = 1
                descr_name = prompt("1.{}.{}-{}.{}".format(com_index, pv_index, field_index, prompt_desc_name),)
                if len(descr_name) == 0:
                    descr_name = pv_name
                descr = et.SubElement(pv, "description")
                descr.text = descr_name

                field_index += 1
                try:
                    enabled_name = prompt("1.{}.{}-{}.{}".format(com_index, pv_index, field_index, prompt_enabled_name), validator=TrueFalseValidator(), default='true')
                    enabled = et.SubElement(pv, "enabled")
                    enabled.text = enabled_name

                    field_index += 1
                    latching_name = prompt("1.{}.{}-{}.{}".format(com_index, pv_index, field_index, prompt_latch_name), validator=TrueFalseValidator(), default='false')
                    latching = et.SubElement(pv, "latching")
                    latching.text = latching_name

                    field_index += 1
                    annunciate_name = prompt("1.{}.{}-{}.{}".format(com_index, pv_index, field_index, prompt_annunciate_name), validator=TrueFalseValidator(), default='false')
                    annunciate = et.SubElement(pv, "annunciating")
                    annunciate.text = annunciate_name

                    field_index += 1
                    delay_name = prompt("1.{}.{}-{}.{}".format(com_index, pv_index, field_index, prompt_delay_name), validator=NumberValidator())

                    if len(delay_name) != 0:
                        delay = et.SubElement(pv, "delay")
                        delay.text = delay_name

                    field_index += 1
                    count_name = prompt("1.{}.{}-{}.{}".format(com_index, pv_index, field_index, prompt_count_name), validator=NumberValidator())
                    if len(count_name) != 0:
                        count = et.SubElement(pv, "count")
                        count.text = count_name

                    field_index += 1
                    guide_index = 1
                    for guidance_title_name in iter(lambda: prompt("1.{}.{}-{}.{}.{}".format(com_index, pv_index, field_index, guide_index, prompt_guidance_name),), ''):
                        guide_detail_index = 0
                        if len(guidance_title_name) != 0:
                            guidance = et.SubElement(pv, "guidance")
                            guidance_title = et.SubElement(guidance, "title")
                            guidance_title.text = guidance_title_name

                        guide_detail_index += 1
                        guidance_details_name = prompt("1.{}.{}-{}.{}.{}.{}".format(com_index, pv_index, field_index, guide_index, guide_detail_index, prompt_guidance_detail_name),)
                        if len(guidance_details_name) != 0:
                            guidance_details = et.SubElement(guidance, "details")
                            guidance_details.text = guidance_details_name
                        guide_index += 1

                    field_index += 1
                    display_index = 1
                    for display_title_name in iter(lambda: prompt("1.{}.{}-{}.{}.{}".format(com_index, pv_index, field_index, display_index, prompt_display_name),), ''):
                        display_detail_index = 0
                        if len(display_title_name) != 0:
                            display = et.SubElement(pv, "display")
                            display_title = et.SubElement(display, "title")
                            display_title.text = display_title_name

                        display_detail_index += 1
                        display_details_name = prompt("1.{}.{}-{}.{}.{}.{}".format(com_index, pv_index, field_index, display_index, display_detail_index, prompt_display_detail_name),)
                        if len(display_details_name) != 0:
                            display_details = et.SubElement(display, "details")
                            display_details.text = display_details_name
                        display_index += 1

                    field_index += 1
                    command_index = 1
                    for command_title_name in iter(lambda: prompt("1.{}.{}-{}.{}.{}".format(com_index, pv_index, field_index, command_index, prompt_command_name),), ''):
                        command_detail_index = 0
                        if len(command_title_name) != 0:
                            command = et.SubElement(pv, "command")
                            command_title = et.SubElement(command, "title")
                            command_title.text = command_title_name

                        command_detail_index +=1
                        command_details_name = prompt("1.{}.{}-{}.{}.{}.{}".format(com_index, pv_index, field_index, command_index, command_detail_index, prompt_command_detail_name),)
                        if len(command_details_name) != 0:
                            command_details = et.SubElement(command, "details")
                            command_details.text = command_details_name
                        command_index += 1

                    prefixes = ['mailto:', 'cmd:', 'sevrpv:']
                    field_index += 1
                    auto_command_index = 1
                    for automated_action_title_name in iter(lambda: prompt("1.{}.{}-{}.{}.{}".format(com_index, pv_index, field_index, auto_command_index, prompt_auto_command_name),), ''):
                        auto_command_detail_index = 0
                        if len(automated_action_title_name) != 0:
                            automated_action = et.SubElement(pv, "automated_action")
                            automated_action_title = et.SubElement(automated_action, "title")
                            automated_action_title.text = automated_action_title_name

                        auto_command_detail_index += 1
                        automated_action_details_name = prompt("1.{}.{}-{}.{}.{}.{}".format(com_index, pv_index, field_index, auto_command_index, auto_command_detail_index, prompt_auto_command_detail_name), validator=NotificationValidator(prefixes), default='cmd:')

                        if len(automated_action_details_name) != 0:
                            automated_action_details = et.SubElement(automated_action, "details")
                            automated_action_details.text = automated_action_details_name

                        auto_command_detail_index += 1
                        delay_name = prompt("1.{}.{}-{}.{}.{}.{}".format(com_index, pv_index, field_index, auto_command_index, auto_command_detail_index, prompt_auto_command_delay_name), validator=NumberValidator())
                        if len(delay_name) != 0:
                            delay = et.SubElement(automated_action, "delay")
                            delay.text = delay_name
                        auto_command_index += 1
                    pv_index += 1
                    continue
                except ValidationError as e:
                    print(e)

            com_index += 1
        xtree = et.ElementTree(config)
        xml_file_name = prompt(prompt_xml_file_name, default="default_alarm_config.xml")
        xtree.write(xml_file_name, encoding="UTF-8", xml_declaration=True, pretty_print=True)
        #prompt("   Exit Ctrl+Q: ", key_bindings=bindings)

    @staticmethod
    def GenXMLAlarmConfigByGroup(file_path, copy):
        data_dict = {}
        current_key = None
        current_value = []
        try:
            file_name = file_path
            if re.search(r"\.csv$",file_name, re.IGNORECASE) is None:
                raise Exception("File extension is not csv!!")
            with open(file_path, 'r', encoding='utf-8') as file:
                csv_reader = csv.reader(file)
                header = next(csv_reader)
                pv_list = []
                for row in csv_reader:
                    # Remove white space for front, backend of special character(=:)
                    if not row:
                        continue
                    rlines = " ".join(row)
                    if rlines[0] == "#" :
                        rlines = rlines.replace("#","")
                        rlines = re.sub(r"\s*([=])\s*","=", rlines)
                        rlines = re.split(r'[\s=\[\]]+', rlines)
                        rlines = [s for s in rlines if s]
                        if rlines[0].lower() != "component" :
                            continue
                        key=rlines[1]
                        #print(rlines)
                        if current_key is not None:
                            data_dict[current_key] = ';'.join(current_value)
                        current_key = key
                        current_value = []
                    elif current_key is not None:
                        rlines = re.split(r'[\s,]+', rlines)
                        current_value.append(rlines[0])

                if current_key is not None:
                    data_dict[current_key] = ';'.join(current_value)

            file.close()
            #print(data_dict)

            config = et.Element("config")
            config.attrib["name"] = "Accelerator"
            for key, values in data_dict.items():
                #print(key,"===>>")
                component = et.SubElement(config, "component")
                component.attrib["name"] = key
                values = re.split(r'[\s;]+', values)
                for value in values:
                    #print(value)
                    pv = et.SubElement(component, "pv")
                    pv.attrib["name"] = value
                    descr = et.SubElement(pv, "descirption")
                    descr.text = value
                    enabled = et.SubElement(pv, "enabled")
                    enabled.text = "true"
                    latching = et.SubElement(pv, "latching")
                    latching.text = "false"

            xtree = et.ElementTree(config)
            config_file = 'generated_alarm_config.xml'
            if copy is True:
                config_file = "../site-config/alarm_config.xml"

            xtree.write(config_file, encoding="UTF-8", xml_declaration=True, pretty_print=True)
            print(f"Successful generation alarm config: {config_file}")
        except FileNotFoundError:
            print(f"File Not Found: {file_path}")
            return None
        except Exception as e:
            print(f"Read error while reading file: {str(e)}")
            return None

    def __is_xml_file(self, file_path):
        return file_path.lower().endswith('.xml')

    def __is_xml_file_contents(self, file_path):
        try:
            et.parse(file_path)
            return True
        except et.XMLSyntaxError:
            return False

    @staticmethod
    def GenXMLAlarmConfig(file_path, copy):
        try:
            file_name = file_path
            if re.search(r"\.csv$",file_name, re.IGNORECASE) is None:
                raise Exception("File extension is not csv!!")

            with open(file_path, 'r', encoding='utf-8') as file:
                csv_reader = csv.reader(file)
                header = next(csv_reader)
                config = et.Element("config")
                config.attrib["name"] = "Accelerator"
                component = et.SubElement(config, "component")
                component.attrib["name"] = "Group"
                prev_val = 0
                for line_num, row in enumerate(csv_reader):
                    if not row:
                        continue
                    rlines = " ".join(row)
                    if rlines[0] == "#" :
                        continue
                    pv_name = re.split(r'[\s,]+', rlines)
                    #print(pv_name)

                    if len(pv_name) > 1 :
                        raise Exception (f"Only need a pv_name information:{file_name}")

                    val, reminder = divmod(line_num, 10)
                    if prev_val is not val:
                        component = et.SubElement(config, "component")
                        component.attrib["name"] = "Group_"+str(val)

                    pv = et.SubElement(component, "pv")
                    pv.attrib["name"] = pv_name[0]
                    descr = et.SubElement(pv, "descirption")
                    descr.text = pv_name[0]
                    enabled = et.SubElement(pv, "enabled")
                    enabled.text = "true"
                    latching = et.SubElement(pv, "latching")
                    latching.text = "false"
                    annunciating = et.SubElement(pv, "annunciating")
                    annunciating.text = "false"
                    prev_val = val

            xtree = et.ElementTree(config)
            config_file = 'generated_alarm_config.xml'
            if copy is True:
                config_file = "../site-config/alarm_config.xml"
            xtree.write(config_file, encoding="UTF-8", xml_declaration=True, pretty_print=True)
            print(f"Successful generation alarm config: {config_file}")
            file.close()

        except FileNotFoundError:
            print(f"File Not Found: {file_path}")
            return None
        except Exception as e:
            print(f"Read error while reading file: {str(e)}")
            return None

class YesNoValidator(Validator):
    def validate(self, document):
        text = document.text.lower()
        if text not in ['yes', 'no']:
            raise ValidationError(message="yes or no.", cursor_position=len(text))

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

def check_all_none(args):
    for arg in vars(args).values():
        if arg is not None:
            return False
    return True


if __name__ == "__main__":

    parser = argparse.ArgumentParser(description='Initialize Alarm Config XML File')
    parser.add_argument('-file',       help='Alarm list for changing alarm setpoints(csv file, i.e., alarm_config.csv)')
    parser.add_argument('-copy',       type=bool, help='Copy initial alarm xml config to pas-demo/site-config/alarm_config.xml')
    parser.add_argument('-component',  type=bool, help='Component name in alarm config xml')
    parser.add_argument('-modify',     type=bool, help='Modify alarm config xml')
    parser.add_argument('-xmlfile',    help='Alarm configuration xml file for alarm-server (xml file, i.e., config.xml)')
    args = parser.parse_args()

    print('Parsed arguments: {}'.format(args))
    print('-file',          args.file)
    print('-copy',          args.copy)
    print('-component',     args.component)
    print('-modify',        args.modify)
    print('-xmlfile',       args.xmlfile)


    try :
        if check_all_none(args) :
            config = AlarmInitialConfig()
        elif args.modify is not None:
            config = AlarmInitialConfig(args.modify, args.xmlfile)
        elif args.file is not None and args.component is None:
            AlarmInitialConfig.GenXMLAlarmConfig(args.file, args.copy)
        elif args.file is not None and args.component is not None:
            AlarmInitialConfig.GenXMLAlarmConfigByGroup(args.file, args.copy)

    except KeyboardInterrupt:
        print("Program Exit==> Not generated alarm config xml")
        sys.exit(1)
    except Exception as e:
        print(f"Main Exception: {str(e)}")
        sys.exit(1)
