CC = gcc
CFLAGS = -g -I../../include
LIBS=

APP = verify_pw

all:$(APP)
	@echo "done"

verify_pw: verify_pw.c
	$(CC) $(CFLAGS) $(LIBS) -o $@ $^

clean:
	rm -f $(APP)
