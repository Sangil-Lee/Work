#!../../bin/linux-x86_64/liquifier

## You may have to change liquifier to something else
## everywhere it appears in this file

< envPaths

cd "${TOP}"

## Register all support components
dbLoadDatabase "dbd/liquifier.dbd"
liquifier_registerRecordDeviceDriver pdbbase

#Siemens PLC
drvAsynIPPortConfigure("liquifier", "10.1.61.115:503", 0, 0, 1)
modbusInterposeConfig("liquifier",0,5000,0)

#drvAsynIPPortConfigure("Liquifier981", "192.168.1.1:502", 0, 0, 1)
#drvAsynIPPortConfigure("Liquifier982", "192.168.1.1:503", 0, 0, 1)
#drvAsynIPPortConfigure("Liquifier983", "192.168.1.1:504", 0, 0, 1)
#modbusInterposeConfig("Liquifier981",0,5000,0)
#modbusInterposeConfig("Liquifier982",0,5000,0)
#modbusInterposeConfig("Liquifier983",0,5000,0)

# Word access at Modbus address 100
# Access 60 words as inputs.  
# Function code=3
# default data type unsigned integer.
# drvModbusAsynConfigure("portName", "tcpPortName", slaveAddress, modbusFunction, modbusStartAddress, modbusLength, dataType, pollMsec, "plcType")
# drvModbusAsynConfigure("A0_In_Word", "sim1", 0, 3, 100, 60, 0, 100, "Simulator")

##Siemens ModbusTCP Address Map according to modbus function code.
drvModbusAsynConfigure("DB981",  "liquifier", 1, 3,   0,  100,  7, 100, "Modicon")

#drvModbusAsynConfigure("DB982",   "Liquifier982", 0, 16,  0,  530,  0, 100, "Modicon")
#drvModbusAsynConfigure("DB983",   "Liquifier983", 0, 3,   0,  530,  0, 100, "Modicon")

##Liquifier DB 
dbLoadTemplate("${TOP}/iocBoot/${IOC}/liquifier.sub")
#dbLoadTemplate("${TOP}/iocBoot/${IOC}/liquifier_db982.sub")
#dbLoadTemplate("${TOP}/iocBoot/${IOC}/liquifier_db983.sub")

cd "${TOP}/iocBoot/${IOC}"
iocInit

## Start any sequence programs
#seq sncxxx,"user=ctrluserHost"
