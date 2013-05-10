#!/bin/sh

find . -name "*.h" -o -name "*.c" -o -name "*.cc"  -o -name "*.cpp" -o -name "*.hpp" -o -name "*.S" -o -name "*.s" -o -name "*.asm" > cscope.files

cscope -bkq -i cscope.files

find . -name "*.[chs]" | etags -
