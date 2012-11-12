CC = gcc
CFLAGS = -Wl,-z,relro

APP = partial_relro_test

all: $(APP)
	@echo "WTF..."

partial_relro_test: partial_relro_test.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm $(APP) *~
