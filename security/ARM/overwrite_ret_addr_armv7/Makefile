CC=gcc
AS=as
LD=ld
CFLAGS= -g

# -fno-stack-protector: disable stack protection, disabled in ARM by default
# -mprefered-stack-boundary=2: Do we need the stack aligne on 2 byte boundary for 32-bit platform?
LIBS =

APP = victim

all: $(APP)
	@echo "done"


attack: attack.c
	$(CC) -o $@ $^

victim: victim.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f *.o $(APP)
