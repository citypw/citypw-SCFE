CC=gcc
CFLAGS= -g -I../
LIBS =
TCP_APPS = pollserv pollcli

APP = TCP

all: $(APP)
	@echo "done"


TCP: $(TCP_APPS)

server_select: pollserv.c
	$(CC) $(CFLAGS) -o $@ $^

client_select: pollcli.c
	$(CC) $(CFLAGS) -o $@ $^


clean:
	rm -f $(TCP_APPS)
