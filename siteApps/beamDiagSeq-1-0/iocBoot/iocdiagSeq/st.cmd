#!../../bin/linux-x86_64/diagSeq

#- You may have to change diagSeq to something else
#- everywhere it appears in this file

< envPaths

cd "${TOP}"

## Register all support components
dbLoadDatabase "dbd/diagSeq.dbd"
diagSeq_registerRecordDeviceDriver pdbbase

## Load record instances
#dbLoadRecords("db/xxx.db","user=ctrluser")
#dbLoadRecords("db/dbBeamDiagSeq.vdb","user=BeamDiag1, num=0")
#dbLoadRecords("db/dbBeamDiagSeq.vdb","user=BeamDiag2, num=1")
dbLoadRecords("db/dbBeamMotorSeq.vdb","Prefix=LEBT:Diag")

cd "${TOP}/iocBoot/${IOC}"
iocInit

## Start any sequence programs
#seq sncxxx,"user=ctrluser"

#seq  sncBeamDiagSeq,  "user=BeamDiag1, num=0"
#seq  sncBeamDiagSeq,  "user=BeamDiag2, num=1"
seq sncBeamMotorSeq, "Prefix=LEBT:Diag"
