#!../../bin/linux-x86_64/fieldmap

## You may have to change fieldmap to something else
## everywhere it appears in this file

< envPaths

cd "${TOP}"

## Register all support components
dbLoadDatabase "dbd/fieldmap.dbd"
fieldmap_registerRecordDeviceDriver pdbbase

## Load record instances
#dbLoadRecords("db/xxx.db","user=ctrluserHost")


#dbLoadRecords("db/Gauss2Ampere.db","SYS=MEBT")
#dbLoadRecords("db/Gauss2Ampere_SR.db","SYS=MEBT")
#dbLoadRecords("db/Spline.vdb")
dbLoadRecords("db/Gauss2Ampere_Spline.db","SYS=MEBT")

cd "${TOP}/iocBoot/${IOC}"
iocInit

## Start any sequence programs
#seq sncFieldMap,"SYS=MEBT,PREFIX=MEBT1,DEV=PS1,FIELD_PV=QWROHI:SR,CURRENT_PV=CurrentSetpt"
#seq sncFieldMap,"SYS=MEBT,PREFIX=MEBT1,DEV=PS2,FIELD_PV=QWRNHI:SR,CURRENT_PV=CurrentSetpt"
#seq sncFieldMap,"SYS=MEBT,PREFIX=MEBT-QM3,DEV=Danfysik,FIELD_PV=HWRHI,CURRENT_PV=SETCURRENT"
#seq sncFieldMap,"SYS=MEBT,PREFIX=MEBT-QM4,DEV=Danfysik,FIELD_PV=QWRNHI,CURRENT_PV=SETCURRENT"
#seq sncFieldMap,"SYS=MEBT,PREFIX=DW,DEV=MPS,FIELD_PV=QWROHI,CURRENT_PV=Icmd_SP"
