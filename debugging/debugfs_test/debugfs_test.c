#include <linux/init.h> /* __init/__exit */
#include <linux/module.h>
#include <linuxdebugfs.h>
#include <linux/fs.h> /* file_ops */
/* Direcotry/File, which we will create */
struct dentry *dir, *file_ret;

/* File ops */
static const struct file_operations test_fops = {
        .read = test_read,
        .write = test_write
};

static ssize_t test_read(struct file *fp, char __user *user_buf, size_t count, loff_t *pos){

}

static int _init test_init()
{
        dir = debugfs_create_dir("shawn_debug", NULL);
        if( dir == NULL){
                printk(KERN_ERR "failed to create directory!\n");
                return -ENODEV;
        }

        file_ret = debugfs_create_file("my_info", 0644, dir, NULL, &test_fops);
        if( read_ret == NULL)
                goto fuc_err;

        return 0;

 fuc_err:
        debugfs_remove_recursive(dir);
        printk(KERN_ERR "failed to create file!\n");
        return -ENODEV;
}

static void __exit test_exit()
{
        debugfs_remove_recursive(dir);
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Shawn the R0ck");

module_init(test_init);
module_exit(test_exit);
