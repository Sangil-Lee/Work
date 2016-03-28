#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

#include <linux/fs.h>

#include <linux/timer.h>

#include <linux/sched.h>

struct timer_list timer;

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

void my_timer(unsigned long data)
{
	struct task_struct * task = (struct task_struct *)data;
	printk("Kernel Timer Time-Out Function Doing...\n");
	printk(KERN_WARNING "[SK_DD]my_timer=>proc_state_%d:%lx!!\n", task->pid, task->state);

	set_task_state(task, EXIT_DEAD);	//include/linux/sched.h
	//set_task_state(task, EXIT_ZOMBIE);	//include/linux/sched.h
	
	//set_task_state(task, TASK_RUNNING);	//include/linux/sched.h
	//__activate_task(task, rq);

	printk(KERN_WARNING "[SK_DD]my_timer=>proc_state_%d:%lx!!\n", task->pid, task->state);
	printk("Kernel Timer Time-Out Function Done!!!\n");
}

ssize_t sk_read (struct file *filp, char *buf, size_t count, loff_t *f_pos)
{
	printk(KERN_WARNING "[SK_DD]read..\n");
	
	init_timer(&timer);
	//timer.expires = get_jiffies_64() + 5*HZ;
	timer.expires = jiffies + 5*HZ;
	timer.function = my_timer;
	timer.data = (long)current;
	add_timer(&timer);
	
	printk(KERN_WARNING "[SK_DD]read=>proc_state_%d:%ld!!\n", current->pid, current->state);

	set_task_state(current, TASK_STOPPED);	//include/linux/sched.h
	//set_task_state(current, EXIT_DEAD);	//include/linux/sched.h

	//set_task_state(current, TASK_INTERRUPTIBLE);	//include/linux/sched.h
	//schedule();	//deactivate_task() call

	printk(KERN_WARNING "[SK_DD]read=>proc_state_%d:%lx!!\n", current->pid, current->state);

	printk(KERN_WARNING "[SK_DD]read done!!\n");
	
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

	del_timer(&timer);
	unregister_chrdev(sk_major, "sk");

	printk(KERN_WARNING "SK Module Delete Done!!\n");
}

module_init(sk_init);
module_exit(sk_exit);

MODULE_LICENSE("Dual BSD/GPL");
