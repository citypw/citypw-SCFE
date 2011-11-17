KERNEL_DIR=/usr/src/linux-2.6.33.4
KERNEL_HEADERS = -l$(KERNEL_DIR)/include

obj-m:=nf_queue_ipq_test.o
PWD = $(shell pwd)

all:
	$(MAKE) -C $(KERNEL_DIR) M=$(PWD) modules
	gcc -I $(KERNEL_DIR)/include test.c -lipq -o test

clean:
	rm -rf *.ko test
	rm -rf *.o
	rm -rf *.mod.*
	rm -rf Module.*
	rm -rf modules.*
	rm -rf .*
