#!../../bin/linux-x86_64/srfcooldown

#- You may have to change srfcooldown to something else
#- everywhere it appears in this file

< envPaths

cd "${TOP}"

## Register all support components
dbLoadDatabase "dbd/srfcooldown.dbd"
srfcooldown_registerRecordDeviceDriver pdbbase

## Load record instances
#dbLoadTemplate("db/cooldownlogic_eval_OM1.4_step6_SRF.sub", "SYS=SRF01, SUBSYS=-Bunker1:, DEV=QWR01, SUBDEV=-Valve:, SIGNAL=Valve")

#Testing...
#dbLoadTemplate("db/Additional.sub")
#dbLoadRecords("db/RefValue.vdb",  "SYS=SRF01,SUBSYS=-Bunker1:,DEV=HWRB01,SUBDEV=-Temp:")
#dbLoadTemplate("db/SRF_HWR_Cooldown.sub", "SYS=SRF01, SUBSYS=-Bunker1:, DEV=HWRB01, SUBDEV=-Valve:, SIGNAL=Valve")

dbLoadRecords("db/PhaseEnd.vdb",	"SYS=SRF01,SUBSYS=-Bunker1:,DEV=HWRB01,SUBDEV=-Level:")
dbLoadRecords("db/IntLockFanout.vdb",  "SYS=SRF01,SUBSYS=-Bunker1:,DEV=HWRB01,SUBDEV=-Logic:")

cd "${TOP}/iocBoot/${IOC}"
iocInit

## Start any sequence programs
#seq sncxxx,"user=ctrluser"
#seq sncSRFCooldown, "SYS=SRF01,SUBSYS=-Bunker1:, DEV=HWRB01, SUBDEV=-Logic:"
