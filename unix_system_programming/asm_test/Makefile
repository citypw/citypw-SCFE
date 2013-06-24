CC = gcc
AS = as
LD = ld
CFLAGS = -I /lib/ld-linux.so.2
LIBS= -lc
APP = if

all:$(APP)
	@echo "done"

if: if.s
	$(AS) -o $@.o $^
	$(LD) -o $@ $@.o $(CFLAGS) $(LIBS)

clean:
	rm -f $(APP)
