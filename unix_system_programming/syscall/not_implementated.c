#include <linux/errno.h>
#include <sys/syscall.h>
#include <errno.h>

extern void *sys_call_table[];

sc()
{ // system call number 165 doesn't exist at this time.
    __asm__( "movl $1111,%eax\n\t"
             "int $0x80\n\t");
}

main()
{
    errno = -sc();
    perror("test of invalid syscall");
}

