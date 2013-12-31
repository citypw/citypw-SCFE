CC=gcc
AS=nasm
LD=ld
CFLAGS= -g -static -masm=intel
LIBS =

APP = exit asm_exit opcode_exit

all: $(APP)
	@echo "done"


exit: exit.c
	$(CC) $(CFLAGS) -o $@ $^

asm_exit: asm_exit.s
	$(AS) -f elf  $^
	$(LD) -o $@ $@.o

opcode_exit: opcode_exit.c
	$(CC) -Wl,-z,execstack -o $@ $^


clean:
	rm -f *.o $(APP)
