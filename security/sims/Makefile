CC = gcc
CFLAGS = -g -I../../include
LIBS=

APP = pie-sim

all:$(APP)
	@echo "done"

pie-sim: pie-sim.c
	$(CC) $(CFLAGS) $(LIBS) -o $@ $^

clean:
	rm -f $(APP)
