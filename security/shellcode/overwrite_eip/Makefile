CC=gcc
AS=as
LD=ld
CFLAGS= -g -fno-stack-protector -mpreferred-stack-boundary=2
# -fno-stack-protector: disable stack protection
# -mprefered-stack-boundary=2: we need the stack is to be aligned on 2 byte boundary
LIBS =

APP = attack victim

all: $(APP)
	@echo "done"


attack: attack.c
	$(CC) -o $@ $^

victim: victim.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f *.o $(APP)
