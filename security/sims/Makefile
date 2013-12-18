CC = gcc
CFLAGS = -g -I../../include
LIBS=

APP = pie-sim memcpy_fortify

all:$(APP)
	@echo "done"

pie-sim: pie-sim.c
	$(CC) $(CFLAGS) $(LIBS) -o $@ $^

memcpy_fortify: memcpy_fortify.c
	$(CC) $(CFLAGS) $(LIBS) -o $@ $^

clean:
	rm -f $(APP)
