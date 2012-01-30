#
# Makefile for the trace utility.
#
# M. Tim Jones <mtj@mtjones.com>
#

CC = gcc

OBJS = trace.o symbols.o stack.o

pvtrace: $(OBJS)
	gcc -o $@ $(OBJS)

.c.o:
	$(CC) $(CFLAGS) -Wall -c $<

install: pvtrace
	cp pvtrace /usr/local/bin

clean:
	rm -f pvtrace *.o
