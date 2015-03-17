#!/usr/bin/perl

use strict;
use warnings;

local $/ = undef;
my $errormessage = "This script will put spaces inside ( or ) and ensure { is on the same line as if/else/functiondef.\nusage: perl codestyle.pl infile [outfile]\n";
my $infile = shift or die $errormessage;
my $outfile = shift || $infile;
open FILE, "<", $infile or die $!;
binmode FILE;

my $code = <FILE>;
close FILE;

# Add spaces inside of ( or )
$code =~ s/\((?=\w)/\( /g;
$code =~ s/(?<=\w)\)/ \)/g;

# Make sure braces are on same line as if/else/function
$code =~ s/(\(.*?\))\s*\{/$1 \{/g;
$code =~ s/\}\s*else\s*(\{|if)/\} else $1/g;

# Replace \t with four spaces
$code =~ s/\t/    /g;



open FILE, ">", $outfile or die $!;
print FILE $code;
close FILE;