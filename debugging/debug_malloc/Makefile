CC = gcc
CFLAGS = -g -I../../include
LIBS=

APP = test hijack_malloc.so

all:$(APP)
	@echo "done"

verify_pw: test.c
	$(CC) $(CFLAGS) $(LIBS) -o $@ $^

hijack_malloc.so: hijack_malloc.c
	$(CC) $(CFLAGS) -fPIC -shared -o $@ $^

clean:
	rm -f $(APP)
