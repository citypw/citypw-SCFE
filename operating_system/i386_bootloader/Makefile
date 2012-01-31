CC := gcc
LD := ld
OBJCOPY := objcopy
OBJDUMP := objdump
GUILE := guile
TOOLS := ../tools
TOP := $(shell pwd)

CFLAGS := $(CFLAGS) -O$(O_LEV) -nostdinc -fno-builtin -fno-stack-protector \
        -I./ -MD -Wall -Wno-format -Wno-unused -Werror -std=c99 $(STABS)

### TARGET ###
all: boot.img

boot.o: i386_boot.S boot_main.c 
	@echo + cc $<
	@mkdir -p $(@D)
	$(V)$(CC) -nostdlib $(CFLAGS) -Os -r -o $@ $^

boot: boot.o
	@echo + ld $@ from $^
	@mkdir -p $(@D)
	$(V)$(LD) -N -e start -Ttext 0x7C00 -o $@.1st $^
	$(V)$(OBJCOPY) -S -O binary $@.1st $@
	$(V)$(GUILE) -e main $(TOOLS)/sign.gl boot

boot.img: boot
	@echo + mk $@
	$(V)dd if=/dev/zero of=$(@F).bak count=10000
	$(V)dd if=boot.out of=$(@F).bak conv=notrunc 
	$(V)dd if=boot of=$(@F).bak seek=1 conv=notrunc 
	$(V)mv $(@F).bak $(@F)

.PHONY: pretty clean bochs qemu

bochs: boot.img
	$(V)bochs

qemu: boot.img
	$(V)qemu -hda $^ -m 32

clean:
	@echo "Cleaning all objs & generated files..."
	$(V)rm -frd *.ld *.log .sw{o,p}
	$(V)rm -frd *.o *.d *.img        
	$(V)rm -frd *~ boot.*
	$(V)test -e boot && rm -f boot || echo "No bootloader generated!"
