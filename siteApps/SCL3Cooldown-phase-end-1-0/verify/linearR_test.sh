echo "Linear Regression"
caput SCL3:Cooldown:TempWFInit.B 50
caput SCL3:Cooldown:LRegTempWF.A 3
caput SCL3:Cooldown:TempWFInit.CALC "AA:=(IX+1)*ARNDM;AA"
caput SCL3:Cooldown:TempWFInit.CALC "AA:=(IX+1)+ARNDM*B;AA"
caput SCL3:Cooldown:TempWFInit.B 10
caput SCL3:Cooldown:TempWFInit.B 11
caput SCL3:Cooldown:TempWFInit.B 1
caput SCL3:Cooldown:TempWFInit.B 10
caput SCL3:Cooldown:TempWFInit.B 20
caput SCL3:Cooldown:TempWFInit.B 30
caput SCL3:Cooldown:TempWFInit.B 50

