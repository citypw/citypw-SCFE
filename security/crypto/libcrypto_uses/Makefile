CC = gcc
CFLAGS = -g
LIBS= -lcrypto

APP = md5_test

all:$(APP)
	@echo "done"

md5_test: md5_test.c
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

clean:
	rm -f $(APP)
