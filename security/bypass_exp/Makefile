CC=gcc
AS=as
LD=ld
CFLAGS= -g

# -fno-stack-protector: disable stack protection
# -mprefered-stack-boundary=2: Do we need the stack aligne on 2 byte boundary for 32-bit platform?
LIBS =

APP = non_exec_stack aslr_only

all: $(APP)
	@echo "done"


non_exec_stack: victim.c
	$(CC) $(CFLAGS) -fno-stack-protector -o $@ $^

aslr_only: victim.c
	$(CC) $(CFLAGS) -fno-stack-protector -z execstack -mpreferred-stack-boundary=2 -o $@ $^

clean:
	rm -f *.o $(APP)
