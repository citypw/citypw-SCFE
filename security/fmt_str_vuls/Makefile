CC = gcc
APP = type1 type2
CFLAGS = -fno-stack-protector -D_FORTIFY_SOURCE=0 -z norelro -z execstack

all: $(APP)

type1:
	$(CC) $(CFLAGS) -o $@ $@.c

type2:
	$(CC) $(CFLAGS) -o $@ $@.c

clean:
	rm -rf $(APP) *~
