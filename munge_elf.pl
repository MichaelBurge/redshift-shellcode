#!/usr/bin/perl

# Input: objdump -s chess.o | head
# Output: hex bytes suitable for inclusion in python
use strict;
use warnings;
use v5.22;

my $x;
$x = <>; #
$x = <>; # Hex dump of section '.text':

sub print_word {
    my ($w) = @_;
    return unless defined($w);
    my @cs = split //, $w;
    while (@cs) {
        my $c1 = shift @cs;
        my $c2 = shift @cs || '0';
        print "\\x$c1$c2";
    }
}

print "shellcode = b'";
while (<>) {
    my (undef, $c1, $c2, $c3, $c4, undef) = split ' ';
    print_word($_) for ($c1, $c2, $c3, $c4);
}
print "'"
