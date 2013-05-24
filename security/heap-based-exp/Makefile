CC=gcc
AS=as
LD=ld
CFLAGS= -g -fno-stack-protector

# -fno-stack-protector: disable stack protection
# -mprefered-stack-boundary=2: Do we need the stack aligne on 2 byte boundary for 32-bit platform?
LIBS =

APP = vulprog exp1 exp2

all: $(APP)
	@echo "done"


exp1: exp1.c
	$(CC) -o $@ $^

exp2: exp2.c
	$(CC) -o $@ $^

vulprog: victim.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f *.o $(APP)
