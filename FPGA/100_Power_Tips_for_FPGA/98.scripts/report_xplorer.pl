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

use strict;
use warnings;
use Switch;

#
# This script searches Xilinx reports for predefined keywords in order to perform build "sanity test"
# 
# arguments:
#  $ARGV[0] : reports directory. 
#  If not specified, processing all reports in the current directory
#
my $REPORT_DIR = ".";

if ( $#ARGV == 0)
{
    $REPORT_DIR = $ARGV[0];
}

die "$REPORT_DIR is not a directory" unless -d $REPORT_DIR;

print ("\nanalyzing reports in directory: $REPORT_DIR \n\n");


chdir $REPORT_DIR;

my @reports = glob("*.syr *.bld *.mrp *.par *.drc *.bgn *.pad");

foreach my $report (@reports) 
{
    open(my $fh, '<', $report) or die "Can't open $report: $!";
    
    my @data = <$fh>;
    
    close($fh);

    print("[ $report ] \n");
           
    switch ($report) 
    {
    	case /\.syr/	
        { 
           print grep /Number of error/ , @data ;
           print grep / latch /i , @data ;           
           print grep /Found Parallel Case directive/i , @data ; 
           print grep /combinatorial loop/i , @data ; 
           print "\n";
        }
        
    	case /\.bld/
        { 
           print grep /error/ , @data ;  
           print "\n";
        }
        
    	case /\.mrp/
        { 
           print grep /overmapped/i , @data ;
           print grep /Number of error/ , @data ;
           print grep /Gated clock/ , @data ;           
           print grep / conflict / , @data ;           
           print grep /unexpected/ , @data ;           
           print grep /dangling/ , @data ;           
           print grep /unassigned IOs/ , @data ;                                                       
           print grep /subset of IOs are locked/ , @data ;                                                                  
           
           print "\n";
        }
        
    	case /\.par/ 
        { 
           print grep /Number of error/ , @data ;
           print grep /Timing Score/ , @data ;           
           print "\n";
        }
        
        
    	case /\.drc/	
        { 
           print grep /ERROR/  , @data ;
           print grep /errors/ , @data ;           
           print "\n";
        }
        
    	case /\.bgn/	
        { 
           # checking for unused pins = not pulled
           # the format is this:
           # | UnusedPin | Pullnone  |
           foreach (@data)
           {
               if ( m/UnusedPin/i )
               {
                   if ( m/Pullnone/i )
                   {
                     #  floating is ok   
                   }
                   else
                   {
                       print "\nerror: BITGEN setting UnusedPin != Pullnone \n\n";   
                   }
               }
           }
           
           print grep /errors/ , @data ;
           print "\n";
        }

    	case /\.pad/	
        { 
            &parse_pad_report( @data );
        }
                                        
	    else 
        {
            print "error: unsupported report $report \n"
        }    
    }

}        
    
exit 0;


#
# check for all iobs locked, all registered
#
sub parse_pad_report
{
    my $is_pin_section = 0;
    my $num_unlocated = 0;
    my $num_unregistered = 0;
    my $num_filtered_ios = 0;
    
    # this is a project-specific list of pins to exclude from triggering a warning
    # it's case-sensitive
    my @exclude_pins = ("SPARE", "MICTOR");
    
    print "\n";
    
    foreach (@_) 
    { 
    
        if( m/IO Register/i )
        {
            $is_pin_section = 1;
            next;
        } 

        if( m/-----/i )
        {
            $is_pin_section = 0;
            next;
        } 
        
        if($is_pin_section)
        {
            # each line in the "pin section" has the following format:
            #
            # Pin Number|Signal Name|Pin Usage|Pin Name|Direction|IO Standard|IO Bank Number|Drive (mA)|Slew Rate|Termination|IOB Delay|Voltage|Constraint|IO Register|Signal Integrity|
            my ($pin_number , $signal_name) = ("","");
            
            
            # that regexp should cover all possible charachters in the signal name up to the angle bracket
            my $num_matches = ( m/^(\w+)\|([a-zA-Z_0-9]+)/gi );
                   
            if( $num_matches == 1)
            {       
                ($pin_number , $signal_name) = ($1,$2);
            
              # print "[$pin_number,$signal_name] \n";
                
                foreach my $a (@exclude_pins)
                {
                    if (index($signal_name, $a) > -1) 
                    {
                        # print "skipping $signal_name \n";
                        $num_filtered_ios = $num_filtered_ios + 1;
                        goto NEXT_LINE;
                    }    
               } 
            }
            
            if( m/UNLOCATED/i )
            {
                print "error: unlocated IO [ $pin_number ] \n";
                
                $num_unlocated = $num_unlocated + 1; 
            }
            elsif( m/LOCATED/i )
            {
                # looking for this pattern
                # AB5|PcieGnd<1>|IOB|IO_L2N_18|OUTPUT|LVCMOS18|18|12|SLOW||||LOCATED|NO|NONE|
                if ( m/NO\|\w+\|$/i )
                {
                    print "warning: unregistered IO for $signal_name, pin=$pin_number \n";
                    
                    $num_unregistered = $num_unregistered + 1;
                }
            }
        }
    
        NEXT_LINE:
    } 
    
    print "\n  num unlocated IOs: $num_unlocated \n";
    print "  num unregistered IOs: $num_unregistered \n\n";    
    print "  num filtered IOs: $num_filtered_ios \n\n";
    
} # parse_pad_report




