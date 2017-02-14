#!/usr/bin/perl

use FindBin qw($Bin);
use lib "$Bin/../../../../lib/perl";

use Test::More tests => 14;

use DBD::Menu;

my $menu = DBD::Menu->new('test');
isa_ok $menu, 'DBD::Menu';
is $menu->name, 'test', 'Menu name';
is $menu->choices, 0, 'Choices == zero';
$menu->add_choice('ch1', 'Choice 1');
is $menu->choices, 1, 'First choice added';
ok $menu->legal_choice('Choice 1'), 'First choice legal';
is_deeply $menu->choice(0), ['ch1', 'Choice 1'], 'First choice found';
$menu->add_choice('ch2', 'Choice 2');
is $menu->choices, 2, 'Second choice added';
ok $menu->legal_choice('Choice 1'), 'First choice still legal';
is_deeply $menu->choice(0), ['ch1', 'Choice 1'], 'First choice still found';
ok $menu->legal_choice('Choice 2'), 'Second choice legal';
is_deeply $menu->choice(1), ['ch2', 'Choice 2'], 'Second choice found';
ok !$menu->legal_choice('Choice 3'), 'Third choice not legal';
is_deeply $menu->choice(2), undef, 'Third choice undefined';

like $menu->toDeclaration, qr/ ^
    \s* typedef \s+ enum \s+ {
    \s+     ch1 \s+ \/\* [^*]* \*\/,
    \s+     ch2 \s+ \/\* [^*]* \*\/,
    \s+     test_NUM_CHOICES ,?
    \s+ } \s+ test; \s* $ /x, 'C declaration';
