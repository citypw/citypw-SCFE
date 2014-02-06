#include <linux/module.h>
#include <linux/kernel.h>
#include <asm/unistd.h>
#include <asm/fcntl.h>
#include <linux/types.h>
#include <linux/dirent.h>
#include <linux/string.h>
#include <linux/fs.h>
#include <linux/syscalls.h>

unsigned long **sys_call_table;

asmlinkage long (*orig_mkdir) (const char __user * pathname, umode_t mode);

asmlinkage long hacked_mkdir(const char __user * pathname, umode_t mode)
{
	printk(KERN_INFO "pathname: %s won't be created!\n", pathname);
	return 0;		/*everything is ok, but the new systemcall
				   does nothing */
}

static unsigned long **get_sys_call_table(void)
{
	unsigned long int offset = PAGE_OFFSET; // 0xc0000000 by default
	unsigned long **sct;

	while (offset < ULLONG_MAX) {
		sct = (unsigned long **) offset;

		if (sct[__NR_close] == (unsigned long *) sys_close)
			return sct;

		offset += sizeof(void *);
	}

	return NULL;
}

static void disable_wp_protection(void)
{
	unsigned long value;
	asm volatile ("mov %%cr0, %0":"=r" (value));

	if (!(value & 0x00010000))
		return;

	asm volatile ("mov %0, %%cr0"::"r" (value & ~0x00010000));
}

static void enable_wp_protection(void)
{
	unsigned long value;
	asm volatile ("mov %%cr0, %0":"=r" (value));

	if ((value & 0x00010000))
		return;

	asm volatile ("mov %0, %%cr0"::"r" (value | 0x00010000));
}

int init_module(void)
{				/*module setup */
	if (!(sys_call_table = get_sys_call_table())) {
		printk(KERN_INFO "request sys_call_table failed!\n");
		return -1;
	}
	printk(KERN_INFO "sys_close = %p\n", sys_call_table);
	disable_wp_protection();
	orig_mkdir = sys_call_table[__NR_mkdir];
	sys_call_table[__NR_mkdir] = hacked_mkdir;
	enable_wp_protection();
	return 0;
}

void cleanup_module(void)
{				/*module shutdown */
	disable_wp_protection();
	sys_call_table[__NR_mkdir] = orig_mkdir;	/*set mkdir syscall to the origal
							   one */
	enable_wp_protection();
}
