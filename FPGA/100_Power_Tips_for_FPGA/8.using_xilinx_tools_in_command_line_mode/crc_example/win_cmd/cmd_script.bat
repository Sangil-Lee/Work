@ECHO OFF
@REM ###########################################
@REM # Script file to run the build on windows machine
@REM ###########################################
@REM #
@REM # Command line for ngdbuild
@REM #
ngdbuild -p xc6slx9-csg225-3 -dd _ngo -nt timestamp -intstyle silent -uc ../ucf/crc.ucf crc.ngc crc.ngd  

@REM #
@REM # Command line for map
@REM #
map -o crc_map.ncd -intstyle silent crc.ngd crc.pcf 

@REM #
@REM # Command line for par
@REM #
par -w -ol med -intstyle silent crc_map.ncd crc.ncd crc.pcf 

@REM #
@REM # Command line for post_par_trce
@REM #
trce -e 3 -intstyle silent -xml crc.twx crc.ncd crc.pcf 

@REM #
@REM # Command line for bitgen
@REM #
bitgen -l -w -m -intstyle silent crc.ncd 

