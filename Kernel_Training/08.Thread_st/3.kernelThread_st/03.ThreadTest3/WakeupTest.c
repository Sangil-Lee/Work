#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

#include <linux/delay.h>
#include <linux/kthread.h>

extern wait_queue_head_t myevent_waitqueue;

extern rwlock_t myevent_lock;
extern unsigned int myevent_id;

struct task_struct *wakeup_task=NULL;

static int wakeup_thread(void *unused)
{
	int cnt=0;
	
	while(!kthread_should_stop()) {
		printk("[wakeup_thread(%d)!!]\n", ++cnt);
		write_lock(&myevent_lock);
		myevent_id = cnt;
		mdelay(1);
		write_unlock(&myevent_lock);
		wake_up_interruptible(&myevent_waitqueue);

		if(cnt == 50) {
			printk("[wakeup_thread exit!!]\n");
			wakeup_task = NULL;
			do_exit(0); 	//kernel_thread_exit(0);
		}
		msleep(2000);
	}
	printk("[wakeup_thread exit!!]\n");
	return 0;
}

int WakeupTest_init(void)
{
	printk("WakeupTest Module id Loaded ....\n");

	wakeup_task = kthread_create(wakeup_thread, NULL, "%s", "wakeup_thread");
	if (wakeup_task) wake_up_process(wakeup_task);
	printk("Kernel Thread Create Done(wakeup_task:%p)!!\n", wakeup_task);

	return 0;
}

void WakeupTest_exit(void)
{
	if(wakeup_task) {
		printk("wakeup_task thread stopped(%p)!!\n", wakeup_task);
		kthread_stop(wakeup_task);
	}
	printk("WakeupTest Module is Unloaded ....\n");
}

module_init(WakeupTest_init);
module_exit(WakeupTest_exit);

MODULE_LICENSE("Dual BSD/GPL");
