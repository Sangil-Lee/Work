#!../../bin/linux-x86_64/cryo

## You may have to change cryo to something else
## everywhere it appears in this file

< envPaths
epicsEnvSet "STREAM_PROTOCOL_PATH" "../../proto"

cd "${TOP}"

## Register all support components
dbLoadDatabase "dbd/cryo.dbd"
cryo_registerRecordDeviceDriver pdbbase

#Asyn Connection

#Heater Controller
#drvAsynIPPortConfigure("L7", "192.168.1.50:4001", 0, 0, 0)
#drvAsynIPPortConfigure("L8", "192.168.1.50:4002", 0, 0, 0)

#Vacuum Controller
drvAsynIPPortConfigure("L8", "192.168.1.50:4005", 0, 0, 0)
#drvAsynIPPortConfigure("L9", "192.168.1.50:4006", 0, 0, 0)

#Siemens PLC
drvAsynIPPortConfigure("L1", "192.168.1.1:502", 0, 0, 1)
modbusInterposeConfig("L1",0,5000,0)
#Add analog output 
#drvAsynIPPortConfigure("L2", "192.168.1.1:502", 0, 0, 1)
#modbusInterposeConfig("L2",0,5000,0)
#Add Temperature
#drvAsynIPPortConfigure("L3", "192.168.1.1:502", 0, 0, 1)
#modbusInterposeConfig("L3",0,5000,0)

## Load record instances
#dbLoadRecords("db/xxx.db","user=cryomoduleHost")

#Heater controller
#HWRA 
#dbLoadRecords("db/heater.db", "SYS=SRF1,SUBSYS=HWRA01,DEV=CM,SUBDEV=Heater") 
#QWR 
#dbLoadRecords("db/heater_qwr.db", "SYS=SRF1,SUBSYS=QWR01,DEV=CM,SUBDEV=Heater") 

#Vacuum controller
#dbLoadRecords("db/tpg256A.db", "SYS=SRF01,SUBSYS=Bunker2,DEV=QWR01,SUBDEV=Vacu") 
#QWR
#dbLoadRecords("db/mks937b_qwr.db", "SYS=SRF01,SUBSYS=Bunker2,DEV=QWR01,SUBDEV=Vacu") 
#HWRA
#dbLoadRecords("db/mks937b_qwr.db", "SYS=SRF01,SUBSYS=Bunker2,DEV=HWRA01,SUBDEV=Vacu") 
#HWRB
dbLoadRecords("db/mks937b_qwr.db", "SYS=SRF01,SUBSYS=Bunker2,DEV=HWRB01,SUBDEV=Vacu") 

# Access 60 words as outputs.  
# Either function code=6 (single register) or 16 (multiple registers) can be used, but 16
# is better because it is "atomic" when writing values longer than 16-bits.
# Default data type unsigned integer.
# drvModbusAsynConfigure("portName", "tcpPortName", slaveAddress, modbusFunction, modbusStartAddress, modbusLength, dataType, pollMsec, "plcType")
# drvModbusAsynConfigure("A0_Out_Word", "sim1", 0, 16, 100, 60, 0, 1, "Simulator")

# Word access at Modbus address 100
# Access 60 words as inputs.  
# Function code=3
# default data type unsigned integer.
# drvModbusAsynConfigure("portName", "tcpPortName", slaveAddress, modbusFunction, modbusStartAddress, modbusLength, dataType, pollMsec, "plcType")
# drvModbusAsynConfigure("A0_In_Word", "sim1", 0, 3, 100, 60, 0, 100, "Simulator")

##Siemens ModbusTCP Address Map according to modbus function code.
drvModbusAsynConfigure("MB_DIn",  	"L1", 0, 3,  900,    6,  0, 100, "Modicon")
drvModbusAsynConfigure("MB_DOut",  	"L1", 0, 6,  910,    4,  0, 100, "Modicon")
drvModbusAsynConfigure("MB_AIn",   	"L1", 0, 3,  0,     68,  7, 100, "Modicon")
#drvModbusAsynConfigure("MB_AIn",   	"L1", 0, 3,  0,     50,  7, 100, "Modicon")
drvModbusAsynConfigure("MB_AOut",  	"L1", 0, 16, 300,   94,  7, 100, "Modicon")
#drvModbusAsynConfigure("MB_AIn_Temp", 	"L1", 0, 3,  150,  112,  7, 100, "Modicon")
drvModbusAsynConfigure("MB_AIn_Temp", 	"L1", 0, 3,  150,  124,  7, 100, "Modicon")

##ModbusTCP DB
#HWRA
#dbLoadTemplate("${TOP}/iocBoot/${IOC}/modbustcp_hwra.sub")
#QWR
#dbLoadTemplate("${TOP}/iocBoot/${IOC}/modbustcp_qwr.sub")
#HWRB
dbLoadTemplate("${TOP}/iocBoot/${IOC}/modbustcp_hwrb.sub")
dbLoadRecords("db/Helium_HeatLoad.vdb", "SYS=SRF01,SUBSYS=Bunker2,DEV=QWR01")

##HWRB Cooldown Logic
dbLoadTemplate("db/Additional.sub")
dbLoadRecords("db/RefValue.vdb",  "SYS=SRF01,SUBSYS=-Bunker2:,DEV=HWRB01,SUBDEV=-Temp:")
#dbLoadRecords("db/PhaseEnd.vdb",  "SYS=SRF01,SUBSYS=-Bunker2:,DEV=HWRB01,SUBDEV=-Level:")
dbLoadTemplate("db/SRF_HWR_Cooldown.sub", "SYS=SRF01, SUBSYS=-Bunker2:, DEV=HWRB01, SUBDEV=-Valve:, SIGNAL=Valve")

cd "${TOP}/iocBoot/${IOC}"
iocInit

## Start any sequence programs
#seq sncModbusTCPtoODA
#seq seqHeater "SYS=SRF1,SUBSYS=QWR01,DEV=CM,SUBDEV=Heater"
seq sncSRFCooldown, "SYS=SRF01,SUBSYS=-Bunker2:, DEV=HWRB01, SUBDEV=-Logic:"
