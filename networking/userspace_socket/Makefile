CC=gcc
CFLAGS= -g

TCP_APPS = simple_tcp_client simple_tcp_server
UDP_APPS = udp_cmd_server udp_cmd_client

APP = TCP UDP

all: $(APP)
	@echo "done"


TCP: $(TCP_APPS)

simple_tcp_server: simple_tcp_server.c
	$(CC) $(CFLAGS) -o $@ $^

simple_tcp_client: simple_tcp_client.c
	$(CC) $(CFLAGS) -o $@ $^


UDP: $(UDP_APPS)

udp_cmd_server: udp_cmd_server.c
	$(CC) $(CFLAGS) -o $@ $^

udp_cmd_client: udp_cmd_client.c
	$(CC) $(CFLAGS) -o $@ $^


clean:
	rm -f simple_tcp_client simple_tcp_server udp_cmd_server udp_cmd_client
