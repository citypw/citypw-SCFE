obj-m = null_bug.o
CC = gcc
PWD=$(shell pwd)

EXPS = exp shellcode

all: exp shellcode
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) modules

exp:
	$(CC) -o $@ $@.c

shellcode:
	$(CC) -o $@ $@.s -nostdlib -Ttext=0

clean:
	rm -f $(EXPS) *.o *.ko Module.* modules.* *.mod.c *~
