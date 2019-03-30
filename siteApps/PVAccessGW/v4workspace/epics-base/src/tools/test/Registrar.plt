#!/usr/bin/perl

use FindBin qw($Bin);
use lib "$Bin/../../../../lib/perl";

use Test::More tests => 2;

use DBD::Registrar;

my $reg = DBD::Registrar->new('test');
isa_ok $reg, 'DBD::Registrar';
is $reg->name, 'test', 'Registrar name';

