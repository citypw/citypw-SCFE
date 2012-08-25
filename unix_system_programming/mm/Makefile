CC = gcc
CFLAGS = -g -I../../include
LIBS=

APP = malloc_test brk_test mmap_test

all:$(APP)
	@echo "done"

malloc_test: malloc_test.c
	$(CC) $(CFLAGS) $(LIBS) -o $@ $^

brk_test: brk_test.c
	$(CC) $(CFLAGS) $(LIBS) -o $@ $^

mmap_test: mmap_test.c
	$(CC) $(CFLAGS) $(LIBS) -o $@ $^

clean:
	rm -f $(APP)
