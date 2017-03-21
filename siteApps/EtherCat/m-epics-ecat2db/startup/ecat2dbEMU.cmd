epicsEnvSet("PREFIX", "ecat2EMU")

ecat2configure(0,500,1,1)

dbLoadRecords("ecat2master.template", "PREFIX=$(PREFIX)")
#dbLoadRecords("ecat2ek1101.template", "PREFIX=$(PREFIX), MOD_ID=COUPLER, SLAVE_IDX=0")
dbLoadRecords("ecat2el4104.db", "PREFIX=$(PREFIX), MOD_ID=AOMOD0, SLAVE_IDX=3")
dbLoadRecords("ecat2el3164.db", "PREFIX=$(PREFIX), MOD_ID=AIMOD0, SLAVE_IDX=4")
dbLoadRecords("ecat2el4134.db", "PREFIX=$(PREFIX), MOD_ID=AOMOD1, SLAVE_IDX=6")
