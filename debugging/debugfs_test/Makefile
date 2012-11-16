obj-m = debugfs_test.o

PWD=$(shell pwd)

all:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) modules

clean:
	rm -f *.o *.ko Module.* modules.* *.mod.c *~
