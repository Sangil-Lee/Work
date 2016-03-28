#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

#include <linux/delay.h>
#include <linux/kthread.h>

#include <linux/timer.h>

//#include <linux/wait.h>
#include <linux/workqueue.h>

void func_workqueue(struct work_struct *dummy);

DECLARE_WAIT_QUEUE_HEAD(myevent_wq);

//DECLARE_WORK(work_queue, func_workqueue, (void*)5);
DECLARE_WORK(work_queue, func_workqueue);

volatile unsigned int flag_myevent = 0;

struct task_struct *task1=NULL;
struct task_struct *task2=NULL;

struct timer_list timer;

//void func_workqueue(void *data)
void func_workqueue(struct work_struct *dummy)
{
	int i;

	printk("*****WorkQueue_Task Start!!\n");
	//for(i=1; i<(int)data; i++) {
	for(i=1; i<10; i++) {
		printk("*****WorkQueue_Task(%d)!!\n", i);
	}
	wake_up_interruptible(&myevent_wq);
	printk("*****WorkQueue_Task End!!\n");
}

void my_timer(unsigned long data)
{
	printk("\t\t\t=====>Kernel Timer Interrupt!!\n");

    timer.expires = jiffies + 2*HZ;
    add_timer(&timer);

	flag_myevent = 1;
	schedule_work(&work_queue);

	printk("\t\t\t=====>Kernel Timer Interrupt Done!!!\n");
}

int thread1(void *unused)
{
	int cnt=1;

	while(!kthread_should_stop()) {
		printk("thread1 wait(%d)..\n", cnt);

		if(!flag_myevent) {
			interruptible_sleep_on(&myevent_wq);
			printk("thread1 doing(%d)..\n", cnt++);
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
		msleep(100);
    }

	task2 = NULL;
	printk("\t\t\t-->thread2 exit..\n");
    return 0;
}

int WorkQueueTest_init(void)
{
	printk("WorkQueueTest Module id Loaded ....\n");

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

void WorkQueueTest_exit(void)
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

	printk("WorkQueueTest Module is Unloaded ....\n");
}

module_init(WorkQueueTest_init);
module_exit(WorkQueueTest_exit);

MODULE_LICENSE("Dual BSD/GPL");
