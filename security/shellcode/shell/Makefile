CC=gcc
AS=as
LD=ld
CFLAGS= -g
LIBS =

APP = c_sh asm_sh opcode_sh

all: $(APP)
	@echo "done"


c_sh: c_sh.c
	$(CC) $(CFLAGS) -o $@ $^

asm_sh: asm_sh.s
	$(AS) -o asm_sh.o $^
	$(LD) -o $@ $@.o

opcode_sh: opcode_sh.c
	$(CC) $(CFLAGS) -o $@ $^


clean:
	rm -f *.o $(APP)
