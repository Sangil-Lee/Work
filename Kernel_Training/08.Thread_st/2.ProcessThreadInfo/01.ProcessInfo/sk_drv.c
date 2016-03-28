#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

#include <linux/fs.h>
#include <asm/uaccess.h>

#include <linux/sched.h>
#include <linux/fs_struct.h>	//struct fs_struct
#include <linux/fdtable.h>	//struct files_struct

int sk_major=0;

int sk_open (struct inode *inode, struct file *filp)
{
	printk(KERN_WARNING "[SK_DD]open..\n");
	
	return 0;
}

int sk_release (struct inode *inode, struct file *filp)
{
	printk(KERN_WARNING "[SK_DD]release..\n");
	return 0;
}

ssize_t sk_read (struct file *filp, char *buf, size_t count, loff_t *f_pos)
{
	static int data=0;
	unsigned long rst;
	
	//printk(KERN_WARNING "[SK_DD]read(flags:%x, fs->users:%d)..\n", current->flags, current->fs->users);
	//printk(KERN_WARNING "[SK_DD]read(pid:%d, fs->users:%d, files->count:%d)..\n", current->pid, current->fs->users, current->files->count);
	printk(KERN_WARNING "[SK_DD]read(pid:%d, fs->users:%d, files->count:%d)..\n", current->pid, current->fs->users, atomic_read(&current->files->count));
	
	data++;
	rst = copy_to_user(buf, &data, count);

	return count;
}


ssize_t sk_write (struct file *filp, const char *buf, size_t count, loff_t *f_pos)
{
	printk(KERN_WARNING "[SK_DD]write..\n");
	return count;
}


//int sk_ioctl(struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg)
long sk_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	printk(KERN_WARNING "[SK_DD]ioctl..\n");
	return 0;
}

struct file_operations sk_fops = {
    read:       sk_read,
    write:      sk_write,
    open:       sk_open,
//    ioctl:		sk_ioctl,
    unlocked_ioctl:		sk_ioctl,
    release:    sk_release
};

int sk_init(void)
{
	int error;

	printk("<1> SK Module id Up....\n");

	error = register_chrdev(sk_major, "sk", &sk_fops);
	if(error < 0) {
		printk(KERN_WARNING "sk:register_chrdev() error!!\n");
		return error;
	} else if(sk_major==0) sk_major = error;
	printk("<1> sk:register_chrdev() ok! => major_num:%d\n", sk_major);

	printk(KERN_WARNING "SK Module Insert Done!!\n");
	return 0;
}

void sk_exit(void)
{
	printk("<1> SK Module id Down....\n");

	unregister_chrdev(sk_major, "sk");

	printk(KERN_WARNING "SK Module Delete Done!!\n");
}

module_init(sk_init);
module_exit(sk_exit);

MODULE_LICENSE("Dual BSD/GPL");
