< envPaths

cd ${TOP}

## Register all support components
dbLoadDatabase("dbd/testDbPv.dbd")
testDbPv_registerRecordDeviceDriver(pdbbase)

## Load record instances
dbLoadRecords("db/dbInteger.db","name=byte01,type=byter")
dbLoadRecords("db/dbInteger.db","name=short01,type=shortr")
dbLoadRecords("db/dbInteger.db","name=ubyte01,type=ubyte")
dbLoadRecords("db/dbInteger.db","name=ushort01,type=ushort")
dbLoadRecords("db/dbInteger.db","name=uint01,type=ulong")
dbLoadRecords("db/dbInteger.db","name=int01,type=longout")
dbLoadRecords("db/dbInteger.db","name=wait01,type=wait")
dbLoadRecords("db/dbScalar.db","name=float01,type=floatr")
dbLoadRecords("db/dbScalar.db","name=double01,type=ai")
dbLoadRecords("db/dbArray.db","name=byteArray01,type=CHAR")
dbLoadRecords("db/dbArray.db","name=shortArray01,type=SHORT")
dbLoadRecords("db/dbArray.db","name=intArray01,type=LONG")
dbLoadRecords("db/dbArray.db","name=ubyteArray01,type=UCHAR")
dbLoadRecords("db/dbArray.db","name=ushortArray01,type=USHORT")
dbLoadRecords("db/dbArray.db","name=uintArray01,type=ULONG")
dbLoadRecords("db/dbArray.db","name=floatArray01,type=FLOAT")
dbLoadRecords("db/dbArray.db","name=doubleArray01,type=DOUBLE")
dbLoadRecords("db/dbString.db","name=string01")
dbLoadRecords("db/dbBigstringin.db","name=bigstring01")
dbLoadRecords("db/dbStringArray.db","name=stringArray01")
dbLoadRecords("db/dbEnum.db","name=enum01")
dbLoadRecords("db/dbCounter.db","name=counter01");

dbLoadRecords("db/dbInteger.db","name=byte02,type=byter")
dbLoadRecords("db/dbInteger.db","name=short02,type=shortr")
dbLoadRecords("db/dbInteger.db","name=ubyte02,type=ubyte")
dbLoadRecords("db/dbInteger.db","name=ushort02,type=ushort")
dbLoadRecords("db/dbInteger.db","name=uint02,type=ulong")
dbLoadRecords("db/dbInteger.db","name=int02,type=longout")
dbLoadRecords("db/dbInteger.db","name=wait02,type=wait")
dbLoadRecords("db/dbScalar.db","name=float02,type=floatr")
dbLoadRecords("db/dbScalar.db","name=double02,type=ai")
dbLoadRecords("db/dbArray.db","name=byteArray02,type=CHAR")
dbLoadRecords("db/dbArray.db","name=shortArray02,type=SHORT")
dbLoadRecords("db/dbArray.db","name=intArray02,type=LONG")
dbLoadRecords("db/dbArray.db","name=floatArray02,type=FLOAT")
dbLoadRecords("db/dbArray.db","name=doubleArray02,type=DOUBLE")
dbLoadRecords("db/dbString.db","name=string02")
dbLoadRecords("db/dbStringArray.db","name=stringArray02")
dbLoadRecords("db/dbEnum.db","name=enum02")
dbLoadRecords("db/dbCounter.db","name=counter02");

dbLoadRecords("db/dbInteger.db","name=byte03,type=byter")
dbLoadRecords("db/dbInteger.db","name=short03,type=shortr")
dbLoadRecords("db/dbInteger.db","name=ubyte03,type=ubyte")
dbLoadRecords("db/dbInteger.db","name=ushort03,type=ushort")
dbLoadRecords("db/dbInteger.db","name=uint03,type=ulong")
dbLoadRecords("db/dbInteger.db","name=int03,type=longout")
dbLoadRecords("db/dbInteger.db","name=wait03,type=wait")
dbLoadRecords("db/dbScalar.db","name=float03,type=floatr")
dbLoadRecords("db/dbScalar.db","name=double03,type=ai")
dbLoadRecords("db/dbArray.db","name=byteArray03,type=CHAR")
dbLoadRecords("db/dbArray.db","name=shortArray03,type=SHORT")
dbLoadRecords("db/dbArray.db","name=intArray03,type=LONG")
dbLoadRecords("db/dbArray.db","name=floatArray03,type=FLOAT")
dbLoadRecords("db/dbArray.db","name=doubleArray03,type=DOUBLE")
dbLoadRecords("db/dbString.db","name=string03")
dbLoadRecords("db/dbStringArray.db","name=stringArray03")
dbLoadRecords("db/dbEnum.db","name=enum03")
dbLoadRecords("db/dbCounter.db","name=counter03");

cd ${TOP}/iocBoot/${IOC}
iocInit()
epicsThreadSleep(2.0)
casr
startPVAServer
dbpf string01 10.1
dbpf wait01 1
dbpf wait02 2
dbpf wait03 3
