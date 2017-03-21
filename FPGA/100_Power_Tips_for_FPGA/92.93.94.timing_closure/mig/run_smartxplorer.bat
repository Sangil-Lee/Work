@echo off

set XILINX=C:\Xilinx\12.1\ISE_DS\ISE
set PATH=%XILINX%\bin\nt;%XILINX%\lib\nt;%PATH%    

@echo on
                                     

rem echo "list all implementation strategies"
rem smartxplorer -p xc6vlx75t-ff484-2 -la  example_top.ngd

echo "running smartxplorer"
smartxplorer.exe -p xc6vlx75t-ff484-2 -l host_list.txt -uc example_top.ucf -area_report on -b  -sf strategy_file.sf -variability_passes 3 -ra -wd results example_top.ngd
