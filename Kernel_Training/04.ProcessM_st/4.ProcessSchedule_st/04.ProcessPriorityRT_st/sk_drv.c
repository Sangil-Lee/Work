#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

#include <linux/fs.h>
#include <asm/uaccess.h>

#include <linux/sched.h>
#include <linux/fs_struct.h>	//struct fs_struct
#include <linux/fdtable.h>	//struct files_struct

#define this_rq()		(&__get_cpu_var(runqueues))

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
	
	printk(KERN_WARNING "[SK_DD]read(pid:%d(%s), prio:%d, static_prio:%d, normal_prio:%d, rt_prio:%u)..\n", current->pid, current->comm, current->prio, current->static_prio, current->normal_prio, current->rt_priority);

#ifdef HOST
	printk(KERN_WARNING "[SK_DD]read(pid:%d(%s), sleep_max:%llu, slice_max:%llu)..\n", current->pid, current->comm, current->se.statistics.sleep_max, current->se.statistics.slice_max);
#else
	printk(KERN_WARNING "[SK_DD]read(pid:%d(%s), sleep_max:%llu, slice_max:%llu)..\n", current->pid, current->comm, current->se.sleep_max, current->se.slice_max);
#endif
	printk(KERN_WARNING "[SK_DD]read(pid:%d(%s), vruntime:%llu, rt_timeslice:%u)..\n", current->pid, current->comm, current->se.vruntime, current->rt.time_slice);

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
