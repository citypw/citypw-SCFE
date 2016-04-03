#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

void* (*real_malloc)( size_t) = NULL;
void* (*real_free)(void*) = NULL;

void debug_init()
{
	real_malloc = dlsym( RTLD_NEXT, "malloc");
	if( real_malloc == NULL){
		fprintf(stderr, "Error in dlsym: %s\n", dlerror());
		exit(-1);
	}

	real_free = dlsym( RTLD_NEXT, "free");
	if( real_free == NULL){
		fprintf(stderr, "Error in dlsym: %s\n", dlerror());
		exit(-1);
	}

}

void *malloc(size_t size)
{
	void *ret;

	if( real_malloc == NULL)
		debug_init();

	ret = real_malloc( size);
	printf("malloc(): Addr = %p, size = %ld\n", ret, size);

	return ret;
}

void free(void *ptr)
{
	if( real_free == NULL)
		debug_init();

	printf("free(): Addr = %p\n", ptr);
}
