set XILINX=C:\Xilinx\12.1\ISE_DS\ISE
set PATH=%XILINX%\bin\nt;%XILINX%\lib\nt;%PATH%    

bitgen -b -w -g ActiveReconfig:Yes -g Persist:Yes -g Binary:Yes -r synth/top.bit synth_pr/top_pr.ncd top_pr.bit

