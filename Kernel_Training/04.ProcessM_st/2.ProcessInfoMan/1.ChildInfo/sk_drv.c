#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

#include <linux/fs.h>

//#include <asm-arm/thread_info.h>	//current_thread_info():2.6.17-13
#include <asm-generic/uaccess.h>	//get_current()

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
	struct task_struct *task;
	//struct list_head *list;
	
	printk(KERN_WARNING "[SK_DD]read..\n");


	//printk(KERN_WARNING "[SK_DD]pid : %d\n", current_thread_info()->task->pid);
	printk(KERN_WARNING "[SK_DD]pid : %d\n", get_current()->pid);
	printk(KERN_WARNING "[SK_DD]pid : %d\n", current->pid);
	
#if 0
	list_for_each(list, &current->children)
	{
		task = list_entry(list, struct task_struct, sibling);
		printk(KERN_WARNING "[SK_DD]chlild_pid : %d\n", task->pid);
	}
#endif
#if 1
	list_for_each_entry(task, &current->children, sibling)
	{
		printk(KERN_WARNING "[SK_DD]chlild_pid : %d\n", task->pid);
	}
#endif
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
