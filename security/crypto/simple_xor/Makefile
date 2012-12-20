CC = gcc
CFLAGS = -g -I../../include
LIBS=

APP = simple_xor

all:$(APP)
	@echo "done"

pie-sim: simple_xor.c
	$(CC) $(CFLAGS) $(LIBS) -o $@ $^

clean:
	rm -f $(APP)
