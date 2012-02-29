CC=gcc
CFLAGS= -g -I../
LIBS =
TCP_APPS = server_select client_select

APP = TCP

all: $(APP)
	@echo "done"


TCP: $(TCP_APPS)

server_select: server_select.c
	$(CC) $(CFLAGS) -o $@ $^

client_select: client_select.c
	$(CC) $(CFLAGS) -o $@ $^


clean:
	rm -f server_select client_select
