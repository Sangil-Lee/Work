#!../../bin/linux-x86_64/s7Modbus

## You may have to change s7Modbus to something else
## everywhere it appears in this file

< envPaths

cd "${TOP}"

## Register all support components
dbLoadDatabase "dbd/s7Modbus.dbd"
s7Modbus_registerRecordDeviceDriver pdbbase

#drvAsynIPPortConfigure("StepMTst","192.168.0.3:502",0,0,1)
drvAsynIPPortConfigure("StepMTst","10.1.5.206:502",0,0,1)
modbusInterposeConfig("StepMTst",0,5000,0)

# The S7 - 300 - modbusTCP, Function code
# Read 32 bits, Coil Status.    Function code=1.   
# Address = 0XXXX, Response Length = 2000 Coils(2000bits)
#drvModbusAsynConfigure("Step_Motor_CIn_Bit",   "StepMTst", 0, 1, 1700, 100,    0,  100, "Modicon")
drvModbusAsynConfigure("Step_Motor_CIn_Bit",   "StepMTst", 0, 1, 0, 100,    0,  100, "Modicon")

drvModbusAsynConfigure("Step_Motor_DIn_Bit",   "StepMTst", 0, 2,  0, 100,    0,  100, "Modicon")

#drvModbusAsynConfigure("Step_Motor_WHR",     "StepMTst", 0, 3, 0, 100,    0,  100, "Modicon")

drvModbusAsynConfigure("Step_Motor_WIn",  "StepMTst", 0, 4, 0, 100,    0,  100, "Modicon")

# Write 1 bit.  Function code=5.
# Address = 0XXXX, Response Length = 1 Coil(1bits)
drvModbusAsynConfigure("Step_Motor_BOut",  "StepMTst", 0, 5,  0, 100,    0,  1, "Modicon")

# Write 8 words (128 bits).  Function code=6.
# Address = 4XXXX, Response Length = 1 Register (1Register, 8bits)
drvModbusAsynConfigure("Step_Motor_WOut", "StepMTst", 0, 6, 0, 100,    0,  100, "Modicon")

# Access the same 256 bits as array outputs.  Function code=15.
# Address = 0XXXX, ...
drvModbusAsynConfigure("Step_Motor_BAOut",  "StepMTst", 0, 15,  0, 100,   0,   1, "Modicon")

# We access the same 16 words as array outputs (256 bits). Function code=16.
# Address = 4XXXX, ...
drvModbusAsynConfigure("Step_Motor_WAOut", "StepMTst", 0, 16, 0, 100,    0,   1, "Modicon")

drvModbusAsynConfigure("Step_Motor_FIn", "StepMTst", 0, 3, 0, 100,   7,  1, "Modicon")

# Write Analog Out (Float).
drvModbusAsynConfigure("Step_Motor_FOut", "StepMTst", 0, 16, 0, 30,   7,  1, "Modicon")

# Enable ASYN_TRACEIO_HEX on modbus server
#asynSetTraceIOMask("Step_Motor_FIn",0,4)


# Dump up to 512 bytes in asynTrace
#asynSetTraceIOTruncateSize("Step_Motor_FIn",0,512)

dbLoadTemplate("${TOP}/iocBoot/${IOC}/_StepMTst.substitutions")

cd "${TOP}/iocBoot/${IOC}"
iocInit

## Start any sequence programs
#seq sncxxx,"user=ctrluserHost"
