#!../../bin/linux-x86_64/N1914A_PowerMeter

< envPaths
epicsEnvSet("ARCH","linux-x86_64")
epicsEnvSet("IOC","N1914A_PowerMeter")
epicsEnvSet("TOP","/home/jii/epics/R3.14.12.5/siteApps/N1914A_PowerMeter")
epicsEnvSet("STREAM_PROTOCOL_PATH", ".:${TOP}/db")

cd "${TOP}"

## Register all support components

dbLoadDatabase "dbd/N1914A_PowerMeter.dbd"
N1914A_PowerMeter_registerRecordDeviceDriver pdbbase

drvAsynIPPortConfigure("N1914A_PowerMeter_15", "192.168.0.15:5025", 0, 0, 0)
drvAsynIPPortConfigure("N1914A_PowerMeter_05", "192.168.0.5:5025", 0, 0, 0)
drvAsynIPPortConfigure("N1914A_PowerMeter_06", "192.168.0.6:5025", 0, 0, 0)
drvAsynIPPortConfigure("N1914A_PowerMeter_07", "192.168.0.7:5025", 0, 0, 0)
drvAsynIPPortConfigure("N1914A_PowerMeter_08", "192.168.0.8:5025", 0, 0, 0)
drvAsynIPPortConfigure("N1914A_PowerMeter_09", "192.168.0.9:5025", 0, 0, 0)
drvAsynIPPortConfigure("N1914A_PowerMeter_10", "192.168.0.10:5025", 0, 0, 0)


dbLoadRecords("db/N1914A_PowerMeter.db", "SYS=RF, SUBSYS=REF, DEV=PM01, SIG=AvgCH1, PORT=N1914A_PowerMeter_15")
dbLoadRecords("db/N1914A_PowerMeter.db", "SYS=RF, SUBSYS=REF, DEV=PM01, SIG=AvgCH2, PORT=N1914A_PowerMeter_15")
dbLoadRecords("db/N1914A_PowerMeter.db", "SYS=RF, SUBSYS=REF, DEV=PM02, SIG=AvgCH1, PORT=N1914A_PowerMeter_05")
dbLoadRecords("db/N1914A_PowerMeter.db", "SYS=RF, SUBSYS=REF, DEV=PM02, SIG=AvgCH2, PORT=N1914A_PowerMeter_05")
dbLoadRecords("db/N1914A_PowerMeter.db", "SYS=RF, SUBSYS=REF, DEV=PM03, SIG=AvgCH1, PORT=N1914A_PowerMeter_06")
dbLoadRecords("db/N1914A_PowerMeter.db", "SYS=RF, SUBSYS=REF, DEV=PM03, SIG=AvgCH2, PORT=N1914A_PowerMeter_06")
dbLoadRecords("db/N1914A_PowerMeter.db", "SYS=RF, SUBSYS=REF, DEV=PM04, SIG=AvgCH1, PORT=N1914A_PowerMeter_07")
dbLoadRecords("db/N1914A_PowerMeter.db", "SYS=RF, SUBSYS=REF, DEV=PM04, SIG=AvgCH2, PORT=N1914A_PowerMeter_07")

dbLoadRecords("db/N1914A_PowerMeter.db", "SYS=RF, SUBSYS=REF, DEV=PM05, SIG=AvgCH1, PORT=N1914A_PowerMeter_08")
dbLoadRecords("db/N1914A_PowerMeter.db", "SYS=RF, SUBSYS=REF, DEV=PM05, SIG=AvgCH2, PORT=N1914A_PowerMeter_08")
dbLoadRecords("db/N1914A_PowerMeter.db", "SYS=RF, SUBSYS=REF, DEV=PM06, SIG=AvgCH1, PORT=N1914A_PowerMeter_09")
dbLoadRecords("db/N1914A_PowerMeter.db", "SYS=RF, SUBSYS=REF, DEV=PM06, SIG=AvgCH2, PORT=N1914A_PowerMeter_09")
dbLoadRecords("db/N1914A_PowerMeter.db", "SYS=RF, SUBSYS=REF, DEV=PM07, SIG=AvgCH1, PORT=N1914A_PowerMeter_10")
dbLoadRecords("db/N1914A_PowerMeter.db", "SYS=RF, SUBSYS=REF, DEV=PM07, SIG=AvgCH2, PORT=N1914A_PowerMeter_10")

cd "${TOP}/iocBoot/${IOC}"

iocInit
