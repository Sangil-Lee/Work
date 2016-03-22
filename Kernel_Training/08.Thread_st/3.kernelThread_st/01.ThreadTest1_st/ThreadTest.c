#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

#include <linux/delay.h>
.............

struct task_struct *my_task=NULL;

static int test_thread(void *unused)
{
	int cnt=0;
	
	while(! .............. ) {
		printk("-->test_thread(%d)!!\n", ++cnt);
		msleep(2000);
		if(cnt == 50) {
			printk("-->test_thread exit!!\n");
			my_task = NULL;
			do_exit(0); 	//kernel_thread_exit(0);
		}
	}
	return 0;
}

int ThreadTest_init(void)
{
	printk("ThreadTest Module id Loaded ....\n");
	my_task = ....... (test_thread, NULL, "%s", "test_thread");
	if (my_task) .......... (my_task);
	printk("Kernel Thread Create Done(my_task:%p)!!\n", my_task);
	return 0;
}

void ThreadTest_exit(void)
{
	if(my_task) {
		printk("my_task thread stopped(%p)!!\n", my_task);
		.......... (my_task);
	}
	printk("ThreadTest Module is Unloaded ....\n");
}

module_init(ThreadTest_init);
module_exit(ThreadTest_exit);

MODULE_LICENSE("Dual BSD/GPL");
