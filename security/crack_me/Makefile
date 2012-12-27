CC = gcc
CFLAGS = -g -I../../include
LIBS=

APP = verify_pw hijack_strcmp.so

all:$(APP)
	@echo "done"

verify_pw: verify_pw.c
	$(CC) $(CFLAGS) $(LIBS) -o $@ $^

hijack_strcmp.so: hijack_strcmp.c
	$(CC) $(CFLAGS) -fPIC -shared -o $@ $^

clean:
	rm -f $(APP)
