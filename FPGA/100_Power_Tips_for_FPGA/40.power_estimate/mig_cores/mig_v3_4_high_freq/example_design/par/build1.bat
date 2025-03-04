set XILINX=C:\Xilinx\12.1\ISE_DS\ISE
set XILINX_DSP=%XILINX%
set PATH=%XILINX%\bin\nt;%XILINX%\lib\nt;%PATH%    

call rem_files.bat


echo Synthesis Tool: XST

mkdir "../synth/__projnav" > ise_flow_results.txt
mkdir "../synth/xst" >> ise_flow_results.txt
mkdir "../synth/xst/work" >> ise_flow_results.txt

xst -ifn xst_options.txt -ofn example_top.syr -intstyle ise >> ise_flow_results.txt
ngdbuild -intstyle ise -dd ../synth/_ngo -nt timestamp -uc example_top.ucf -p xc6vlx75t-ff484-2 example_top.ngc example_top.ngd >> ise_flow_results.txt

echo Implementation Tool: XST
map -intstyle ise -p xc6vlx75t-ff484-2 -detail -w -ol high -xe n -register_duplication off -global_opt off -ir off -pr off -lc off -power off -o example_top_map.ncd example_top.ngd example_top.pcf >> ise_flow_results.txt
par -w -intstyle ise -ol high -xe n example_top_map.ncd example_top.ncd example_top.pcf >> ise_flow_results.txt
trce -intstyle ise -v 3 -fastpaths -xml example_top example_top.ncd -o example_top.twr example_top.pcf >> ise_flow_results.txt
bitgen -intstyle ise -f bitgen_options.ut example_top.ncd >> ise_flow_results.txt
echo done!
