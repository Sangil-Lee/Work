#!../../bin/linux-x86_64/liquifier

## You may have to change liquifier to something else
## everywhere it appears in this file

< envPaths

cd "${TOP}"

## Register all support components
dbLoadDatabase "dbd/liquifier.dbd"
liquifier_registerRecordDeviceDriver pdbbase

#Siemens PLC
#DB981
drvAsynIPPortConfigure("liquifier", "192.168.10.9:502", 0, 0, 1)
modbusInterposeConfig ("liquifier", 0, 5000, 0)

#DB981-2, DB981-3
drvAsynIPPortConfigure("liquifier2", "192.168.10.9:502", 0, 0, 1)
modbusInterposeConfig ("liquifier2", 0, 5000, 0)


#DB982
drvAsynIPPortConfigure("liquifier3", "192.168.10.9:502", 0, 0, 1)
modbusInterposeConfig ("liquifier3", 0, 5000, 0)

#DB983
drvAsynIPPortConfigure("liquifier4", "192.168.10.9:502", 0, 0, 1)
modbusInterposeConfig ("liquifier4", 0, 5000, 0)

# Word access at Modbus address 100
# Access 60 words as inputs.  
# Function code=3
# default data type unsigned integer.
# drvModbusAsynConfigure("portName", "tcpPortName", slaveAddress, modbusFunction, modbusStartAddress, modbusLength, dataType, pollMsec, "plcType")
# drvModbusAsynConfigure("A0_In_Word", "sim1", 0, 3, 100, 60, 0, 100, "Simulator")

##Siemens ModbusTCP Address Map according to modbus function code.
drvModbusAsynConfigure("DB981",    "liquifier",  0, 3,   0,    125,  0, 100, "Modicon")
drvModbusAsynConfigure("DB981-2",  "liquifier2", 0, 3,   132,  40,  0, 100, "Modicon")
drvModbusAsynConfigure("DB981-3",  "liquifier2", 0, 3,   210,  50,  0, 100, "Modicon")
drvModbusAsynConfigure("DB981-4",  "liquifier2", 0, 3,   81,  2,  0, 100, "Modicon")

drvModbusAsynConfigure("DB982",    "liquifier3", 0, 16,   1017,  12,  0, 100, "Modicon")
drvModbusAsynConfigure("DB982-2",  "liquifier3", 0, 16,   1073,  82,  0, 100, "Modicon")
drvModbusAsynConfigure("DB982-3",  "liquifier3", 0, 6,   1157,  10,  0, 100, "Modicon")
drvModbusAsynConfigure("DB982-4",  "liquifier3", 0, 6,   1000,  1,  0, 100, "Modicon")

drvModbusAsynConfigure("DB983",    "liquifier4", 0, 3,   2017,  12,  0, 100, "Modicon")
drvModbusAsynConfigure("DB983-2",  "liquifier4", 0, 3,   2073,  82,  0, 100, "Modicon")
drvModbusAsynConfigure("DB983-3",  "liquifier4", 0, 3,   2157,  10,  0, 100, "Modicon")

##Liquifier DB 
dbLoadTemplate("${TOP}/iocBoot/${IOC}/liquifier.sub")
dbLoadTemplate("${TOP}/iocBoot/${IOC}/liquifier_db982.sub")
dbLoadTemplate("${TOP}/iocBoot/${IOC}/liquifier_db983.sub")

cd "${TOP}/iocBoot/${IOC}"
iocInit

## Start any sequence programs
#seq sncxxx,"user=ctrluserHost"
