CC = gcc
CFLAGS = -g -I../../include
LIBS=

APP = run
TMP_LIBS = test1.o test2.o

all:$(APP)
	@echo "done"

run:libfunc.a
	@echo "run...."
	$(CC) -o $@ $@.o $^

libfunc.a:$(TMP_LIBS)
	@echo "libfunc.a..."
	ar crv $@ $^
# c: create archive
# r: replace
# v: verbose

test1.o:
	$(CC) -c *.c

test2.o:
	echo "test2.o done"

clean:
	rm $(TMP_LIBS) *.[ao] $(APP)
