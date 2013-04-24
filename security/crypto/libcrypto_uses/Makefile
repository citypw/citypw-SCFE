CC = gcc
CFLAGS = -g
LIBS= -lcrypto

APP = md5_test cbc_test weird_cbc

all:$(APP)
	@echo "done"

md5_test: md5_test.c
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

cbc_test: cbc_test.c
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

weird_cbc: weird_cbc.c
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

clean:
	rm -f $(APP)
