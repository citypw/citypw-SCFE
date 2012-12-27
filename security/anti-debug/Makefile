CC = gcc

APP = anti-gdb chk_breakpoint set_breakpoint

all: $(APP)
	echo "Done"

anti-gdb: anti-gdb.c
	echo "Compiling the shits..."
	$(CC) -o $@ $^

chk_breakpoint: chk_breakpoint.c
	$(CC) -o $@ $^

set_breakpoint: set_breakpoint.c
	$(CC) -o $@ $^

clean:
	rm $(APP) *~
