KERNEL_DIR=/usr/src/linux-2.6.33.4
KERNEL_HEADERS = -l$(KERNEL_DIR)/include

obj-m:=filter_if.o
PWD = $(shell pwd)

all:
	$(MAKE) -C $(KERNEL_DIR) M=$(PWD) modules

clean:
	rm -rf *.ko
	rm -rf *.o
	rm -rf *.mod.*
	rm -rf Module.*
	rm -rf modules.*
	rm -rf .*
