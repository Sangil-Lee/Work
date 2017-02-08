set XILINX=C:\Xilinx\12.1\ISE_DS\ISE
set XILINX_DSP=%XILINX%
set PATH=%XILINX%\bin\nt;%XILINX%\lib\nt;%PATH%    

echo xpower

xpwr example_top.ncd example_top.pcf -v -o example_top.pwr 

echo done!
