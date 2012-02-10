CC = gcc
CFLAGS = -g -I../../include
LIBS=

APP = pipe

all:$(APP)
	@echo "done"

pipe: pipe_test.c
	$(CC) $(CFLAGS) $(LIBS) -o $@ $^

clean:
	rm -f $(APP)
