CC=gcc
AS=nasm
LD=ld
CFLAGS= -g
LIBS =

APP = c_port_bind asm_port_bind port_connect

all: $(APP)
	@echo "done"


c_port_bind: port_bind.c
	$(CC) $(CFLAGS) -o $@ $^

asm_port_bind: port_bind.s
	$(AS) -f elf -o $@.o $^
	$(LD) -o $@ $@.o

port_connect: port_connect.s
	$(AS) -f elf $^
	$(LD) -o $@ $@.o


clean:
	rm -f *.o $(APP)
