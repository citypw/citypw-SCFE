CC= gcc
CFLAGS = 
OMP_FLAGS = -fopenmp
LDFLAGS = -lpthread

all:
	$(CC) -o mt mt_find_primes.c $(CFLAGS) $(LDFLAGS)
	$(CC) -o st st_find_primes.c $(CFLAGS) $(LDFLAGS)

openmp:
	$(CC) -o mt mt_find_primes.c $(CFLAGS) $(LDFLAGS)
	$(CC) -o st st_find_primes.c $(CFLAGS) $(LDFLAGS) $(OMP_FLAGS)

clean:
	rm mt st
