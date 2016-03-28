#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

#include <linux/delay.h>
#include <linux/kthread.h>


#define	SPIN

#ifndef SPIN
int gcnt=0;
#else
//rwlock_t myevent_lock;
DEFINE_RWLOCK(myevent_lock);
//EXPORT_SYMBOL(myevent_lock);
int gcnt=0;
#endif

struct task_struct *task1=NULL;
struct task_struct *task2=NULL;
struct task_struct *task3=NULL;

void my_delay(volatile int cnt)
{
	volatile int i, j;
	for(i=0; i<cnt; i++) {
		for(j=0; j<1000000; j++) {
			j = j;
		}
	}
}

int thread1(void *unused)
{
	int t=0;

	printk("thread1 start..\n");

	while(!kthread_should_stop()) {
#ifndef SPIN
		printk("[thread1_before(%d)!!]\n", gcnt);
		t = gcnt;
		t += 1;
		my_delay(100);
		gcnt = t;
		printk("[thread1_after(%d)!!]\n", gcnt);
#else
		read_lock(&myevent_lock);
		printk("[thread1_before(%d)!!]\n", gcnt);
		read_unlock(&myevent_lock);

		write_lock(&myevent_lock);
		t = gcnt;
		t += 1;
		my_delay(100);
		gcnt = t;
		write_unlock(&myevent_lock);

		read_lock(&myevent_lock);
		printk("[thread1_after(%d)!!]\n", gcnt);
		read_unlock(&myevent_lock);
#endif

		msleep(500);
    }

	task1 = NULL;
	printk("thread1 exit..\n");
    return 0;
}

int thread2(void *unused)
{
	int t=0;

	printk("\t\t-->thread2 start..\n");

	while(!kthread_should_stop()) {
#ifndef SPIN
		printk("\t\t-->thread2_before(%d)!!\n", gcnt);
		t = gcnt;
		t += 100;
		my_delay(200);
		gcnt = t;
		printk("\t\t-->thread2_after(%d)!!\n", gcnt);
#else
		read_lock(&myevent_lock);
		printk("\t\t-->thread2_before(%d)!!\n", gcnt);
		read_unlock(&myevent_lock);

		write_lock(&myevent_lock);
		t = gcnt;
		t += 100;
		my_delay(200);
		gcnt = t;
		write_unlock(&myevent_lock);

		read_lock(&myevent_lock);
		printk("\t\t-->thread2_after(%d)!!\n", gcnt);
		read_unlock(&myevent_lock);
#endif

		msleep(500);
    }

	task2 = NULL;
	printk("\t\t-->thread2 exit..\n");
    return 0;
}

int thread3(void *unused)
{
	int cnt=0;

	while(!kthread_should_stop()) {
		printk("\t\t-->thread3(%d)!!\n", ++cnt);
		msleep(1000);
    }

	task3 = NULL;
	printk("\t\t\t==>thread3 exit..\n");
    return 0;
}

int SpinLockTest_init(void)
{
	printk("SpinLockTest Module id Loaded ....\n");

	task1 = kthread_create(thread1, NULL, "%s", "thread1");
	if (task1) wake_up_process(task1);
	printk("Kernel Thread Create Done(task1:%p)!!\n", task1);

	task2 = kthread_create(thread2, NULL, "%s", "thread2");
	if (task2) wake_up_process(task2);
	printk("Kernel Thread Create Done(task2:%p)!!\n", task2);

	task3 = kthread_create(thread3, NULL, "%s", "thread3");
	if (task3) wake_up_process(task3);
	printk("Kernel Thread Create Done(task3:%p)!!\n", task3);

	return 0;
}

void SpinLockTest_exit(void)
{
	if(task1) {
		printk("task1 thread stopped(%p)!!\n", task1);
		kthread_stop(task1);
	}
	if(task2) {
		printk("task2 thread stopped(%p)!!\n", task2);
		kthread_stop(task2);
	}
	if(task3) {
		printk("task3 thread stopped(%p)!!\n", task3);
		kthread_stop(task3);
	}

	printk("SpinLockTest Module is Unloaded ....\n");
}

module_init(SpinLockTest_init);
module_exit(SpinLockTest_exit);

MODULE_LICENSE("Dual BSD/GPL");
