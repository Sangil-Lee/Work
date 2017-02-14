@ECHO OFF
@REM ###########################################
@REM # Script file to run the flow 
@REM ###########################################
xflow -p xc6slx9-csg225-3 -implement impl.opt -config bitgen.opt crc.ngc
