CC=gcc -g

test:test.c sample_en.o md5.o
	$(CC) -o test test.c sample_en.o md5.o

sample_en.o:sample_en.c sample_en.h
	$(CC) -c sample_en.c

md5.o:md5.o sample_en.h
	$(CC) -c md5.c

clean:
	rm *.o
	rm test
