#!/bin/bash

if [ $# -ne 1 ]; then
	echo -e "\e[92m Usage: $0 shellcode-file\e[0m"
	exit 1
fi

for i in $(cat $1); do echo -en $i; done > $1.bin
