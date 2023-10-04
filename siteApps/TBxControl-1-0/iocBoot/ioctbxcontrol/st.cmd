#!../../bin/linux-x86_64/tbxcontrol

#- You may have to change tbxcontrol to something else
#- everywhere it appears in this file

< envPaths

cd "${TOP}"

## Register all support components
dbLoadDatabase "dbd/tbxcontrol.dbd"
tbxcontrol_registerRecordDeviceDriver pdbbase

#Siemens PLC
drvAsynIPPortConfigure("TBX_PLC", "192.168.140.171:502", 0, 0, 1)
modbusInterposeConfig("TBX_PLC",0,5000,0)


## to 0   up 250 bytes(125 word), data block
drvModbusAsynConfigure("TBXAI",   "TBX_PLC", 0, 3,  0,   125, 7, 100, "Modicon")  
## to 251 up 500 bytes(125 word), data block
drvModbusAsynConfigure("TBXAI_1", "TBX_PLC", 0, 3,  126, 125, 7, 100, "Modicon")  
## to 252 up 750 bytes(125 word), data block
drvModbusAsynConfigure("TBXAI_2", "TBX_PLC", 0, 3,  252, 125, 7, 100, "Modicon")  


#Spare
#drvModbusAsynConfigure("SPARE", "TBX_PLC",   0, 3,  378,  20, 7, 100, "Modicon")  


####400~548
drvModbusAsynConfigure("TBXAO",   "TBX_PLC", 0, 6,  400, 123, 7, 100, "Modicon")
drvModbusAsynConfigure("TBXAO_1", "TBX_PLC", 0, 6,  524, 24,  7, 100, "Modicon")

drvModbusAsynConfigure("TBXDI", "TBX_PLC",   0, 3,  550,  25, 7, 100, "Modicon")
drvModbusAsynConfigure("TBXDO", "TBX_PLC",   0, 6,  576,  32, 7, 100, "Modicon")

drvModbusAsynConfigure("TBXAO_2", "TBX_PLC", 0, 6,  608, 123,  7, 100, "Modicon")
drvModbusAsynConfigure("TBXAO_3", "TBX_PLC", 0, 6,  732, 123,  7, 100, "Modicon")
drvModbusAsynConfigure("TBXAO_4", "TBX_PLC", 0, 6,  856, 123,  7, 100, "Modicon")
drvModbusAsynConfigure("TBXAO_5", "TBX_PLC", 0, 6,  980,  40,  7, 100, "Modicon")


## Load record instances, Before 0818
#dbLoadTemplate("db/TBxcontrol.sub")
#dbLoadTemplate("db/TBxcontrol_test.sub")
#dbLoadTemplate("db/TBxcontrol_2.sub")


##0818
#dbLoadTemplate("db/TBxcontrol_3.sub")


##23_0927
dbLoadTemplate("db/TBxcontrol_4.sub")
dbLoadTemplate("db/GBCPStat_TBx.sub")
dbLoadTemplate("db/GBIntStat_TBx.sub")
dbLoadTemplate("db/GBCURStat_TBx.sub")
dbLoadTemplate("db/SoftPVs_TBx.sub")

##0929, Thermal Load Calc
dbLoadTemplate("db/TBxcontrol_hepak.sub")

cd "${TOP}/iocBoot/${IOC}"
iocInit

## Start any sequence programs
#seq sncxxx,"user=ctrluser"
dbpf Cryo-CDL00:TBx01-TT7581:SetCP			"50"
dbpf Cryo-CDL00:TBx01-TT7581:SetDelayTime	"3" 
dbpf Cryo-CDL00:TBx01-PT7501:SetCP			"21"
dbpf Cryo-CDL00:TBx01-PT7501:SetDelayTime	"3" 
dbpf Cryo-CDL00:TBx01-PT7503:SetCP			"21"
dbpf Cryo-CDL00:TBx01-PT7503:SetDelayTime	"3" 
dbpf Cryo-CDL00:TBx01-PT7504:SetCP			"20.5" 
dbpf Cryo-CDL00:TBx01-PT7504:SetDelayTime	"3" 
dbpf Cryo-CDL00:TBx01-PT7506:SetCP			"20.5" 
dbpf Cryo-CDL00:TBx01-PT7506:SetDelayTime	"3" 
dbpf Cryo-CDL00:TBx01-PT7507:SetCP			"18.5" 
dbpf Cryo-CDL00:TBx01-PT7507:SetDelayTime	"3" 
dbpf Cryo-CDL00:TBx01-PT7401:SetCP			"8" 
dbpf Cryo-CDL00:TBx01-PT7401:SetDelayTime	"3" 
dbpf Cryo-CDL00:TBx01-PT7403:SetCP			"8" 
dbpf Cryo-CDL00:TBx01-PT7403:SetDelayTime	"3" 
dbpf Cryo-CDL00:TBx01-PT7301:SetCP			"1.8" 
dbpf Cryo-CDL00:TBx01-PT7301:SetDelayTime	"3" 
dbpf Cryo-CDL00:TBx01-PT7303:SetCP			"1.8" 
dbpf Cryo-CDL00:TBx01-PT7303:SetDelayTime	"3" 
dbpf Cryo-CDL00:TBx01-PT7305:SetCP			"1.1" 
dbpf Cryo-CDL00:TBx01-PT7305:SetDelayTime	"3" 
dbpf Cryo-CDL00:TBx01-PT7201:SetCP			"1.8" 
dbpf Cryo-CDL00:TBx01-PT7201:SetDelayTime	"3" 
dbpf Cryo-CDL00:TBx01-PT7582:SetCP			"18.5" 
dbpf Cryo-CDL00:TBx01-PT7582:SetDelayTime	"3" 
dbpf Cryo-CDL00:TBx01-PT7483:SetCP			"8" 
dbpf Cryo-CDL00:TBx01-PT7483:SetDelayTime	"3" 
dbpf Cryo-CDL00:TBx01-PT7381:SetCP			"0" 
dbpf Cryo-CDL00:TBx01-PT7381:SetDelayTime	"0" 
dbpf Cryo-CDL00:TBx01-PT7105:SetCP			"0.05" 
dbpf Cryo-CDL00:TBx01-PT7105:SetDelayTime	"3" 
dbpf Cryo-CDL00:TBx01-PT7106:SetCP			"0.05" 
dbpf Cryo-CDL00:TBx01-PT7106:SetDelayTime	"3" 
dbpf Cryo-CDL00:TBx01-PT7145:SetCP			"0.05" 
dbpf Cryo-CDL00:TBx01-PT7145:SetDelayTime	"3" 
dbpf Cryo-CDL00:TBx01-PT7146:SetCP			"0.05" 
dbpf Cryo-CDL00:TBx01-PT7146:SetDelayTime	"3" 
dbpf Cryo-CDL00:TBx01-PT7181:SetCP			"0.05" 
dbpf Cryo-CDL00:TBx01-PT7181:SetDelayTime	"3" 
