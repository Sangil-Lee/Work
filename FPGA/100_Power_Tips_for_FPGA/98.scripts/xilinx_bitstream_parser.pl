#!/devl/perl/bin/perl

use Switch;

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
# A script to parse Xilinx FPGA bitstream in .RBT format
#
if ($#ARGV < 0)
{
    die "Usage: xilinx_bitstream_parser <rbt file>\n";
}

$verbose = 2;

open(RBT_FILE,"<$ARGV[0]")          || die "Error: can't open $ARGV[0] for input\n";

# Bus OP codes
$opcode{"00"} = "no op";
$opcode{"01"} = "read";
$opcode{"10"} = "write";
$opcode{"11"} = "decrypt";

# Regster Addresses
$reg{"00000"} = "CRC ";
$reg{"00001"} = "FAR ";
$reg{"00010"} = "FDRI";
$reg{"00011"} = "FDRO";
$reg{"00100"} = "CMD ";
$reg{"00101"} = "CTL ";
$reg{"00110"} = "MASK";
$reg{"00111"} = "STAT";
$reg{"01000"} = "LOUT";
$reg{"01001"} = "COR ";
$reg{"01010"} = "MFWR";
$reg{"01011"} = "CBC ";
$reg{"01100"} = "ID  ";
$reg{"01101"} = "AXSS";
$reg{"01110"} = "COR1";
$reg{"01111"} = "CSOB";
$reg{"10000"} = "WBSTAR";
$reg{"10001"} = "TIMER";
$reg{"10010"} = "RBCRC0";
$reg{"10011"} = "RBCRC1";
$reg{"10100"} = "RBCRC2";
$reg{"10101"} = "EFAR";
$reg{"10110"} = "BOOTSTS";
$reg{"10111"} = "TESTMODE";
$reg{"11000"} = "CTL1";


# Configuration Commands
$command{"00000"} = "NULL";
$command{"00001"} = "WCFG";
$command{"00010"} = "MFW";
$command{"00011"} = "LFRM";
$command{"00100"} = "RCFG";
$command{"00101"} = "START";
$command{"00110"} = "RCAP";
$command{"00111"} = "RCRC";
$command{"01000"} = "AGHIGH";
$command{"01001"} = "SWITCH";
$command{"01010"} = "GRESTORE";
$command{"01011"} = "SHUTDOWN";
$command{"01100"} = "GCAPTURE";
$command{"01101"} = "DESYNCH";
$command{"01110"} = "DRTEST";
$command{"01111"} = "IPROG";
$command{"10000"} = "CRCC";

$fa = -1;
$last_fa = -1;
$in_LOUT = 0;
@LOUT_wordcnt = ();

while (<RBT_FILE>)
{
   $type = "";
   $op   = "";
   $reg  = "";
   chop;
   next if (! /^[01]/);   
   
   if (/^10101010100110010101010101100110/) {
       $type = "Sync word";
   }
   elsif (/^11111111111111111111111111111111/) {
       $type = "Dummy word";
   }
   elsif (/^00000000000000000000000010111011/) {
       $type = "Bus Width word ";
   }
   elsif (/^00010001001000100000000001000100/) {
       $type = "8/16/32 BusWidth";
   }
   elsif (/^001/) {
       $type = "Type 1";
   }
   elsif (/^010/) {
       $type = "Type 2";
   }
   elsif (/^00000000000000000000000000000000/) {
       $type = "NO OP";
       $wordcnt = 0;
   }
   else {
       if (($register eq "FDRI") || ($register eq "FDRO") || ($last_command eq "LFRM")) {
          $type = "Partial CRC word";
       }else{
          $type = "Type Unknown";
       }
   }
   
   if ($type eq "Type 1" || $type eq "Type 2")    {
      $s = substr($_,3,2);
      if ($opcode{$s} ne "") 
      {
   	      $op = $opcode{$s};
      }
      else 
      {
          $op = "Unknown";
      }
   }
   
   if ($type eq "Type 1") {
      $s = substr($_,14,5);
      if ($reg{$s} ne "") {
   	   $register = $reg{$s};
      }
      else {
          $register = "Unknown";
      }
      $wordcnt = &bin2dec(substr($_,21,11));
   }
   
   if ($type eq "Type 2"){
      $wordcnt = &bin2dec(substr($_,5,27));
   }
   
   $text = "$type";
   if ($op eq "read") 
   {
      $text .= " $op " . "$wordcnt words from " . "$register";
   }
   elsif (($op eq "write") || ($op eq "decrypt"))
   {
      $text .= " $op " . "$wordcnt words to " . "$register";
   }
   elsif ($op eq "no op") 
   {
      $text .= " NO OP";
   }
   printf ("%10s %s\n", &bin2hex($_), $text);


   # A little hack so we can see the next stream also
   if (($register eq "LOUT") && ($wordcnt > 1)) 
   {
      $in_LOUT++;
      push @LOUT_wordcnt, $wordcnt;
      $wordcnt = 0;
   }
    
    
    # start printing out everything
    for ($i=0; $i<$wordcnt && (($op eq "write") || ($op eq "decrypt") || ($op eq "read")) && ($_ = <RBT_FILE>); $i++) 
    {
        chop;

        if (($op eq "write") && ($register eq "FAR "))
        {
    	    $fa = $_;
        }
        
        if (($register eq "FDRI") || ($register eq "FDRO")) 
        {
            # printf ("%s %10s\n", "data word " . $i, $_ );

            # don't print all frame words
            
            if ($i == 0) 
            {
	            printf ("%s %d...%d \n", "data words " , $i , $wordcnt-1);
            }
       }
       elsif ($register eq "LOUT") 
       {
          $this_fa = &bin2dec($_);
          $BlkType = &bin2dec(substr($_,8,3));
          $TopBot  = (&bin2dec(substr($_,11,1)) == 0) ? "Top" : "Bot";
          $MajRow  = &bin2dec(substr($_,12,5));
          $MajCol  = &bin2dec(substr($_,17,8));
          $MinCol  = &bin2dec(substr($_,25,7));
        
           printf ("%10s %s [Block %d %s Row %d Col %d Minor %d] \n", &bin2hex($_) , "Frame Address", $BlkType, $TopBot, $MajRow, $MajCol, $MinCol);        
                
                
          if ($this_fa < $last_fa) {
             printf ("ERROR: Frame Address going down (%d < %d)?\n", $this_fa, $last_fa);
          }
          $last_fa = $this_fa;
       }
        elsif ($register eq "FAR ") 
        {
            $BlkType = &bin2dec(substr($_,8,3));
            $TopBot  = (&bin2dec(substr($_,11,1)) == 0) ? "Top" : "Bot";
            $MajRow  = &bin2dec(substr($_,12,5));
            $MajCol  = &bin2dec(substr($_,17,8));
            $MinCol  = &bin2dec(substr($_,25,7));
           
            printf ("%10s %s [Block %d %s Row %d Col %d Minor %d] \n", &bin2hex($_) , $register . " data word " . $i, $BlkType, $TopBot, $MajRow, $MajCol, $MinCol);            
            
        }
        elsif ($register ne "CMD ") 
        {
            printf ("%10s %s\n", &bin2hex($_) , $register . " data word " . $i);
            
        } 
        else 
        {
            $s = substr($_,27,5);
	        
            if ($command{$s} ne "") 
            {
                $last_command = $command{$s};
	            printf ("%10s %s\n", &bin2hex($_), $command{$s} . " command",  );
	        }
            else
            {
	            printf ("%10s %s\n", &bin2hex($_), "Unknown command" );
	        }
        }
    }
} # while


close(RBT_FILE);


sub bin2dec
{
    local($num) = @_;
    $retval = 0;
    while ($num ne "")
    {
        $retval = $retval * 2;
        if (substr($num,0, 1) eq "1") 
        {
            $retval++;
    	}
        $num = substr($num,1);
    }
    return $retval;
}


# convert 32-digit bin number to hex
sub bin2hex
{
    local($num) = @_;

    my $str_hex = "";
    
    for ($i=0; $i<8; $i++) 
    {
        # left to right
        my $chunk = substr($num, 4 * $i ,4); 

        my $retval = "0";

        switch ($chunk) 
        {
	        case "0000" { $retval = "0" }
	        case "0001" { $retval = "1" }        
	        case "0010" { $retval = "2" }
	        case "0011" { $retval = "3" }        
	        case "0100" { $retval = "4" }
	        case "0101" { $retval = "5" }        
	        case "0110" { $retval = "6" }
	        case "0111" { $retval = "7" }        
	        case "1000" { $retval = "8" }
	        case "1001" { $retval = "9" }        
	        case "1010" { $retval = "A" }
	        case "1011" { $retval = "B" }        
	        case "1100" { $retval = "C" }
	        case "1101" { $retval = "D" }        
	        case "1110" { $retval = "E" }
	        case "1111" { $retval = "F" }        
        }

      #  print $chunk. " : " . $retval . "\n";

        $str_hex .= $retval;
    }
    
  #  print $str_hex;    
    return $str_hex;
}
