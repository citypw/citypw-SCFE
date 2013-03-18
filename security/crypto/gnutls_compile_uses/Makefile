CC = gcc
CFLAGS = -g
#CFLAGS = -g -I /home/john/john_test/include -L /home/john/john_test/lib
LIBS= -lgnutls

APP = anonymous_cli anonymous_serv ex-client-x509 ex-serv-x509

all:$(APP)
	@echo "done"

anonymous_cli: anonymous_cli.c
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

anonymous_serv: anonymous_serv.c
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

ex-serv-x509: ex-serv-x509.c
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

ex-client-x509: ex-client-x509.c
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

clean:
	rm -f $(APP)
