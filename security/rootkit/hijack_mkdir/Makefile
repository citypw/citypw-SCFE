obj-m = hijack_mkdir.o
CC = gcc
PWD=$(shell pwd)

all:
	make -C /usr/src/linux-headers-`uname -r` M=$(PWD) modules

clean:
	rm -rf *.o *.ko Module.* modules.* *.mod.c .* *~
