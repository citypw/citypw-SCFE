obj-m += hide_file.o

default:
	@echo "To build hide_file:"
	@echo "  make KDIR=/path/to/kernel"
	@echo
	@echo "To clean the build dir:"
	@echo "  make clean KDIR=/path/to/kernel"
	@echo
	@echo "Supported targets:"
	@echo "linux-x86        Linux, x86"
	@echo

linux-x86:
ifndef KDIR
	@echo "Must provide KDIR!"
	@exit 1
endif
	$(MAKE) -C $(KDIR) M=$(PWD) modules


clean:
ifndef KDIR
	@echo "Must provide KDIR!"
	@exit 1
endif
	$(MAKE) -C $(KDIR) M=$(PWD) clean
