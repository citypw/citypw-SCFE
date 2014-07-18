# Sample Makefile for Capstone Disassembly Engine

LIBNAME = capstone

test1: test1.o
	${CC} $< -O3 -Wall -l$(LIBNAME) -o $@

%.o: %.c
	${CC} -c $< -o $@

clean:
	rm -f test1 test1.o
