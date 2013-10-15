/**
 * Map a file into the memory. Then write this file into another file
 * where located in /tmp/phrack.log
 * 
 * btw: This file is a great paper which was released in Phrack Issue
 * 64. It's a kind of must-read paper for hackers!
 *
 * This code is GPL'ed!  
 * monkey-coder:Shawn the R0ck
 * <citypw@gmail.com>
 */

#include <stdio.h>
#include <string.h>
#include <sys/mman.h> /* mmap() */
#include <sys/types.h>
#include <sys/stat.h> /* struct stat */
#include <unistd.h>
#include <debug_msg.h>

int main(int argc, char *argv[])
{
        struct stat sb;
        char *map_addr;
        int fd, i;
        FILE *fp;

        /* open a file */
        fp = fopen("/tmp/phrack.log", "r+");
        if(fp == NULL){
                DEBUG_MSG("fp is null!\n", errno);
                return -1;
        }

        /* get file descriptor by converting the file stream */
        fd = fileno(fp);

        /* get a file's status */
        if(fstat(fd, &sb) == -1){
                DEBUG_MSG("fstat() is failed: %d\n", errno);
                return -1;
        }

        printf("This file is %ld Kilobytes!\n", sb.st_size);

        /* map the file into the memory */
        map_addr = mmap(NULL, sb.st_size, PROT_WRITE, MAP_SHARED, fd, 0);
        if(map_addr == MAP_FAILED){
                DEBUG_MSG("can't map the file onto the mem!\n");
                return -1;
        }

	strncpy(map_addr, "hell world", 8);
	printf("mem: %s\n", map_addr);

	if( msync(map_addr, 8, MS_SYNC) == -1)
		perror("msync");

        if(munmap(map_addr, sb.st_size) == -1){
                DEBUG_MSG("munmap() failed: %d\n", errno);
                return -1;
        }

        fclose(fp);
	close(fd);
        return 0;
}
