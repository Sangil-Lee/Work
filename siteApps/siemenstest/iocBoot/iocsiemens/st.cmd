#!../../bin/linux-x86_64/siemens

## You may have to change siemens to something else
## everywhere it appears in this file

< envPaths

cd "${TOP}"

## Register all support components
dbLoadDatabase "dbd/siemens.dbd"
siemens_registerRecordDeviceDriver pdbbase

drvAsynIPPortConfigure("siemenstest","10.1.5.206:502",0,0,1)
modbusInterposeConfig("siemenstest",0,5000,0)

drvModbusAsynConfigure("Step_Motor_CIn_Bit",   "siemenstest", 0, 1, 0, 100,    0,  100, "Modicon")
drvModbusAsynConfigure("Step_Motor_DIn_Bit",   "siemenstest", 0, 2, 0, 100,    0,  100, "Modicon")
drvModbusAsynConfigure("Step_Motor_WHR",   "siemenstest", 0, 3, 0, 100,    0,  100, "Modicon")
drvModbusAsynConfigure("Step_Motor_FIn",   "siemenstest", 0, 3, 0, 100,    7,  1, "Modicon")
drvModbusAsynConfigure("Step_Motor_WIn",   "siemenstest", 0, 4, 0, 100,    0,  100, "Modicon")
drvModbusAsynConfigure("Step_Motor_BOut",   "siemenstest", 0, 5, 0, 100,    0,  1, "Modicon")
drvModbusAsynConfigure("Step_Motor_WOut",   "siemenstest", 0, 6, 0, 100,    0,  100, "Modicon")
drvModbusAsynConfigure("Step_Motor_BAOut",   "siemenstest", 0, 15, 0, 100,    0,  1, "Modicon")
drvModbusAsynConfigure("Step_Motor_WAOut",   "siemenstest", 0, 16, 0, 100,    0,  1, "Modicon")
drvModbusAsynConfigure("Step_Motor_FOut",   "siemenstest", 0, 16, 0, 100,    7,  1, "Modicon")

dbLoadTemplate("${TOP}/iocBoot/${IOC}/siemenstest.sub")
cd "${TOP}/iocBoot/${IOC}"
iocInit

## Start any sequence programs
#seq sncxxx,"user=ctrluserHost"
