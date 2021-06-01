#!../../bin/linux-x86_64/kobra

#- You may have to change kobra to something else
#- everywhere it appears in this file

< envPaths

cd "${TOP}"

## Register all support components
dbLoadDatabase "dbd/kobra.dbd"
kobra_registerRecordDeviceDriver pdbbase

#dbLoadTemplate("${TOP}/iocBoot/${IOC}/MGParameter.sub",	 "DEVICE=KOBRA-MAG:Q10-PS")
dbLoadTemplate("${TOP}/iocBoot/${IOC}/MGParameter_SW1.sub", "DEVICE=KOBRA-MAG:SW1-PS")
#dbLoadTemplate("${TOP}/iocBoot/${IOC}/MGParameter_SW2.sub", "DEVICE=KOBRA-MAG:SW2-PS")
#dbLoadTemplate("${TOP}/iocBoot/${IOC}/MGParameter_Q1P.sub",  "DEVICE=KOBRA-MAG:Q1-PS")
#dbLoadTemplate("${TOP}/iocBoot/${IOC}/MGParameter_Q9.sub",	 "DEVICE=KOBRA-MAG:Q9-PS")
#dbLoadTemplate("${TOP}/iocBoot/${IOC}/MGParameter_Q3.sub",	 "DEVICE=KOBRA-MAG:Q3-PS")
#dbLoadTemplate("${TOP}/iocBoot/${IOC}/MGParameter_Q4.sub",  "DEVICE=KOBRA-MAG:Q4-PS")
#dbLoadTemplate("${TOP}/iocBoot/${IOC}/MGParameter_Q5.sub",   "DEVICE=KOBRA-MAG:Q5-PS")
#dbLoadTemplate("${TOP}/iocBoot/${IOC}/MGParameter_Q6.sub",   "DEVICE=KOBRA-MAG:Q6-PS")
#dbLoadTemplate("${TOP}/iocBoot/${IOC}/MGParameter_Q8.sub",   "DEVICE=KOBRA-MAG:Q8-PS")
#dbLoadTemplate("${TOP}/iocBoot/${IOC}/MGParameter_H1.sub",   "DEVICE=KOBRA-MAG:H1-PS")
#dbLoadTemplate("${TOP}/iocBoot/${IOC}/MGParameter_Q2P.sub",  "DEVICE=KOBRA-MAG:Q2-PS")
#dbLoadTemplate("${TOP}/iocBoot/${IOC}/MGParameter_Q7P.sub",  "DEVICE=KOBRA-MAG:Q7-PS")
#dbLoadTemplate("${TOP}/iocBoot/${IOC}/MGParameter_Q10P.sub",  "DEVICE=KOBRA-MAG:Q10-PS")
#dbLoadTemplate("${TOP}/iocBoot/${IOC}/MGParameter_Q11P.sub",  "DEVICE=KOBRA-MAG:Q11-PS")
#dbLoadTemplate("${TOP}/iocBoot/${IOC}/MGParameter_Q12P.sub",  "DEVICE=KOBRA-MAG:Q12-PS")
#dbLoadTemplate("${TOP}/iocBoot/${IOC}/MGParameter_Q13P.sub",  "DEVICE=KOBRA-MAG:Q13-PS")
#dbLoadTemplate("${TOP}/iocBoot/${IOC}/MGParameter_Q14P.sub",  "DEVICE=KOBRA-MAG:Q14-PS")
#dbLoadTemplate("${TOP}/iocBoot/${IOC}/MGParameter_Q15P.sub",  "DEVICE=KOBRA-MAG:Q15-PS")
#dbLoadTemplate("${TOP}/iocBoot/${IOC}/MGParameter_SQP.sub",  "DEVICE=KOBRA-MAG:SQ-PS")
#dbLoadTemplate("${TOP}/iocBoot/${IOC}/MGParameter_LQP.sub",  "DEVICE=KOBRA-MAG:LQ-PS")
#dbLoadTemplate("${TOP}/iocBoot/${IOC}/MGParameter_H2P.sub",  "DEVICE=KOBRA-MAG:H2-PS")
#dbLoadTemplate("${TOP}/iocBoot/${IOC}/MGParameter_D1P.sub",  "DEVICE=KOBRA-MAG:D1-PS")
#dbLoadTemplate("${TOP}/iocBoot/${IOC}/MGParameter_D2P.sub",  "DEVICE=KOBRA-MAG:D2-PS")


dbLoadRecords("db/dbAutoRampup.db", "DEVICE=KOBRA-MAG:SW1-PS")

cd "${TOP}/iocBoot/${IOC}"
iocInit

## Start any sequence programs
#seq sncxxx,"user=ctrluser"
#seq sncAutoRampup,"DEVICE=KOBRA-MAG:SW1-PS,  TARGET=KOBRA-MAG:SW1-PS:SetCurrent"
## for Test
#seq sncAutoRampup,"DEVICE=KOBRA-MAG:SW1-PS,  TARGET=KOBRA-MAG:SW1-PS:SetCurrent"
seq sncAutoRampdown,"DEVICE=KOBRA-MAG:SW1-PS,  TARGET=KOBRA-MAG:SW1-PS:SetCurrent"


