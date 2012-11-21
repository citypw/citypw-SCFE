/* This program is a example of null pointer dereference in kernel.
 *
 * monkey-coder: Shawn the R0ck@copyleft, <citypw@gmail.com>
 *  
 * This code is also GPL'ed!
 */

#include <linux/init.h> /* __init/__exit */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/debugfs.h>
#include <linux/fs.h> /* file_ops */
#include <asm/uaccess.h> /* copy_from_user() */

/* Direcotry/File, which we will create */
struct dentry *dir, *file_ret;

struct null_bug{
	unsigned int (*test)(void);
};

ssize_t (*test_bug)(void);

static ssize_t test_write(struct file *fp, const char __user *user_buf, size_t count, loff_t *pos){
	struct null_bug *nb = NULL;

	printk(KERN_INFO "write request from Userspace!\n");

//	nb->test();
	test_bug(); /* boom... */
	return count;
}

/* File ops */
static const struct file_operations test_fops = {
	       		        .write = test_write
};

static int __init test_init(void)
{
	/* create a dicreoty: /sys/kernel/debug/shawn_debug */
        dir = debugfs_create_dir("shawn_bug", NULL);
        if( dir == NULL){
                printk(KERN_ERR "failed to create directory!\n");
                return -ENODEV;
        }

        file_ret = debugfs_create_file("exp_me", 0666, dir, NULL, &test_fops);
        if( file_ret == NULL)
                goto fuc_err;

        return 0;

 fuc_err:
        debugfs_remove_recursive(dir);
        printk(KERN_ERR "failed to create file!\n");
        return -ENODEV;
}

static void __exit test_exit(void)
{
        debugfs_remove_recursive(dir);
	printk(KERN_INFO "Bye, debugfs world!\n");
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Shawn the R0ck");

module_init(test_init);
module_exit(test_exit);
