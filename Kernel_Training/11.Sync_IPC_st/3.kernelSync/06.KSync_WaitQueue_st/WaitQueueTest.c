#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

#include <linux/delay.h>
#include <linux/kthread.h>

#include <linux/timer.h>

#include <.............>

...............
volatile unsigned int flag_myevent = 0;

struct task_struct *task1=NULL;
struct task_struct *task2=NULL;

struct timer_list timer;


void my_timer(unsigned long data)
{
	printk("\t\t\t=====>Kernel Timer Interrupt!!\n");

    timer.expires = jiffies + 2*HZ;
    add_timer(&timer);

	flag_myevent = 1;
	.................

	printk("\t\t\t=====>Kernel Timer Interrupt Done!!!\n");
}

int thread1(void *unused)
{
	int cnt=1;

	while(!kthread_should_stop()) {
		printk("thread1 wait(%d)..\n", cnt);

		if(!flag_myevent) {
			..................
			printk("thread1 doing(%d)..\n", ++cnt);
		}

		flag_myevent = 0;
		//msleep(500);
    }

	task1 = NULL;
	printk("thread1 exit..\n");
    return 0;
}

int thread2(void *unused)
{
	int cnt=0;

	while(!kthread_should_stop()) {
		printk("\t\t-->thread2(%d)!!\n", ++cnt);
		msleep(1000);
    }

	task2 = NULL;
	printk("\t\t\t-->thread2 exit..\n");
    return 0;
}

int WaitQueueTest_init(void)
{
	printk("WaitQueueTest Module id Loaded ....\n");

	task1 = kthread_create(thread1, NULL, "%s", "thread1");
	if (task1) wake_up_process(task1);
	printk("Kernel Thread Create Done(task1:%p)!!\n", task1);

	task2 = kthread_create(thread2, NULL, "%s", "thread2");
	if (task2) wake_up_process(task2);
	printk("Kernel Thread Create Done(task2:%p)!!\n", task2);

    init_timer(&timer);
    timer.expires = jiffies + 2*HZ;
    timer.function = my_timer;
    timer.data = 5;
    add_timer(&timer);

	return 0;
}

void WaitQueueTest_exit(void)
{
	if(task1) {
		printk("task1 thread stopped(%p)!!\n", task1);
		kthread_stop(task1);
	}
	if(task2) {
		printk("task2 thread stopped(%p)!!\n", task2);
		kthread_stop(task2);
	}

    del_timer(&timer);

	printk("WaitQueueTest Module is Unloaded ....\n");
}

module_init(WaitQueueTest_init);
module_exit(WaitQueueTest_exit);

MODULE_LICENSE("Dual BSD/GPL");
