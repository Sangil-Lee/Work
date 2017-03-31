#!/usr/bin/perl

#
#  getNetSNMPverion.pl
#
#  Converts the string version x.y.z returned from `net-snmp-config --version`
#  to a numeric value in a -D flag for a C compiler
#

# get netsnmp x.y.z version
$cmd = "net-snmp-config --version";
$vers_dotted = `$cmd`;
die "command '$cmd' failed: $!\n" if ( $? == -1 );

# convert x.y.z to array [x,y,z]
@arr = split(/\./, $vers_dotted);

# calculate numeric version
$nvers = 0;
foreach $nn (@arr[0..2]) {
  $nn = 0 if ($nn == "");
  $nvers *= 100;
  $nvers += $nn;
}

# output define flag for compiler
print "-DdevSnmp_NETSNMP_VERSION=$nvers";
#print "-DdevSnmp_NETSNMP_VERSION=50701";


