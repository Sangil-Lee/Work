#!/bin/csh -f

\rm -rf ./csrc

vcs -l compile.log \
    -sverilog \
    example.sv get_systime.c

if ($status != 0) then 
    exit 0
endif

./simv -l simulation.log

