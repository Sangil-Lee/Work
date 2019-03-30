onbreak {quit -f}
onerror {quit -f}

vsim -t 1ps -lib xil_defaultlib evr_mgt_gtx_opt

do {wave.do}

view wave
view structure
view signals

do {evr_mgt_gtx.udo}

run -all

quit -force
