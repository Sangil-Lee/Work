#! /bin/perl -w

# -----------------------------------------------------------------------------
#  Copyright (C) 2011 OutputLogic.com 
#  This source file may be used and distributed without restriction 
#  provided that this copyright statement is not removed from the file 
#  and that any derivative work contains the original copyright notice 
#  and the associated disclaimer. 
#  
#  THIS SOURCE FILE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS 
#  OR IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED	
#  WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. 
# -----------------------------------------------------------------------------
#
# A script to call Xilinx TRCE with different parameters
#

use strict;
use warnings;
use Env;
use File::Path;
use Cwd;

my $REPORT_1 = "timing_analyzer_1";
my $REPORT_2 = "timing_analyzer_2";

my $PROJ_WORK    = "./synth";

my $PROJ_NAME = "timing_analyzer";

my $LIMIT_VERBOSE       = 20;
my $LIMIT_ERROR         = 20;
my $LIMIT_UNCONSTRAINED = 20;
my $LIMIT_TOTAL         = 20;

my $now_string = localtime;
print "\n\t [$0] running TRCE: $now_string\n\n\t ";

my $OS_WIN     = ( $^O eq "MSWin32" );


if($OS_WIN)
{
    # default windows installation of ISE 12.1
    $ENV{"PATH"}   = 'c:\Xilinx\12.1\ISE_DS\ISE\bin\nt;' . $ENV{"PATH"};
    $ENV{"XILINX"} = 'c:\Xilinx\12.1\ISE_DS\ISE'; 
}
else
{
    # defailt linux installation
    $ENV{"PATH"}   = '/opt/Xilinx/12.1/ISE_DS/common/bin/lin64;' . '/opt/Xilinx/12.1/ISE_DS/ISE/bin/lin64;' . $ENV{"PATH"};
    $ENV{"XILINX"} = '/opt/Xilinx/12.1/ISE_DS/ISE'; 
}

chdir $PROJ_WORK or die "$!";


# Usage: 
# trce [-e|-v [<limit>]] [-l <limit>] [-n <limit>] 
# [-u <limit>] 
# [-a] [-s <speed>] 
# [-o <report[.twr]>] 
# [-stamp <stampfile>] 
# [-tsi <tsifile[.tsi]>] 
# [-xml <report[.twx]>] 
# [-nodatasheet] 
# [-timegroups] 
# [-fastpaths] 
# [-intstyle ise|xflow|silent] [-ise <projectfile>] [-filter <filter_file[.filter]>]
# <design[.ncd]> [<constraint[.pcf]>]

#  
# <limit> of -l takes priority over -e
# specify either -e or -v
#
system("trce -intstyle silent -e $LIMIT_ERROR -l $LIMIT_TOTAL -u $LIMIT_UNCONSTRAINED -timegroups -tsi ../$REPORT_1.tsi -xml ../$REPORT_1.twx -o ../$REPORT_1.twr $PROJ_NAME.ncd $PROJ_NAME.pcf" ); 

system("trce -intstyle silent -v $LIMIT_VERBOSE -l $LIMIT_TOTAL -u $LIMIT_UNCONSTRAINED -timegroups -tsi ../$REPORT_2.tsi -xml ../$REPORT_2.twx -o ../$REPORT_2.twr $PROJ_NAME.ncd $PROJ_NAME.pcf" ); 

print "\n\t TRCE is complete\n";







