#!../../bin/linux-x86_64/FastProtect

############################################################################
## Set up environment -- allow for overrides
epicsEnvSet(FP_HOST, "$(FP_HOST=192.168.1.10)")
epicsEnvSet(P, "$(FP_P=FP:)")
epicsEnvSet(R, "$(FP_R=1:)")
< envPaths
cd "${TOP}"

############################################################################
# Register all support components
dbLoadDatabase "dbd/FastProtect.dbd"
FastProtect_registerRecordDeviceDriver pdbbase

############################################################################
# Set up ASYN ports
# (ASYN_PORT, IP_ADDRESS, logDir, Priority, eventRate)
fastProtectConfigure("FP", "$(FP_HOST)", "/tmp/log", 0, 0)
#asynSetTraceMask("FP", 0, 0x9)
#asynSetTraceMask("FP_LOG", 0, 0x19)

############################################################################
# Load record instances
dbLoadRecords("db/fastProtect.db","P=$(P),R=$(R),PORT=FP")
dbLoadRecords("db/asynRecord.db","P=$(IOC):,R=asyn,PORT=FP,ADDR=-1,OMAX=0,IMAX=0")

############################################################################
# Start IOC
cd "${TOP}/iocBoot/${IOC}"
iocInit
