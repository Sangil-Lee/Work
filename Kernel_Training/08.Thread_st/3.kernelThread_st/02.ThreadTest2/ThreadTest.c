#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

#include <linux/delay.h>
#include <linux/kthread.h>

static DECLARE_WAIT_QUEUE_HEAD(myevent_waitqueue);

//rwlock_t myevent_lock;
DEFINE_RWLOCK(myevent_lock);
unsigned int myevent_id;

struct task_struct *my_task=NULL;
struct task_struct *wakeup_task=NULL;

void run_umode_handler(int event_id)
{
	printk("----->User Mode Handler Doing(event_id:%d)...\n", event_id);
	udelay(5);
	//printk("----->User Mode Handler Done!!\n");
}

static int my_thread(void *unused)
{
    unsigned int event_id = 0;
    DECLARE_WAITQUEUE(wait, current);

    printk("my_thread start..\n");

    allow_signal(SIGKILL);

    add_wait_queue(&myevent_waitqueue, &wait);

    while(!kthread_should_stop()) {
        set_current_state(TASK_INTERRUPTIBLE);

        schedule();

        printk("my_thread wakeup..\n");
        if (signal_pending(current)) break;
        
        read_lock(&myevent_lock);
        if (myevent_id) {
            event_id = myevent_id;
            read_unlock(&myevent_lock);
            run_umode_handler(event_id); 
        } else {
            read_unlock(&myevent_lock);
        }
    }

    set_current_state(TASK_RUNNING);
    remove_wait_queue(&myevent_waitqueue, &wait);

    my_task = NULL;
    printk("my_thread exit..\n");
    return 0;
}

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

int ThreadTest_init(void)
{
	printk("ThreadTest Module id Loaded ....\n");
	my_task = kthread_create(my_thread, NULL, "%s", "my_thread");
	if (my_task) wake_up_process(my_task);
	printk("Kernel Thread Create Done(my_task:%p)!!\n", my_task);

	msleep(3);

	wakeup_task = kthread_create(wakeup_thread, NULL, "%s", "wakeup_thread");
	if (wakeup_task) wake_up_process(wakeup_task);
	printk("Kernel Thread Create Done(wakeup_task:%p)!!\n", wakeup_task);

	return 0;
}

void ThreadTest_exit(void)
{
	if(my_task) {
		printk("my_task thread stopped(%p)!!\n", my_task);
		kthread_stop(my_task);
	}
	if(wakeup_task) {
		printk("wakeup_task thread stopped(%p)!!\n", wakeup_task);
		kthread_stop(wakeup_task);
	}
	printk("ThreadTest Module is Unloaded ....\n");
}

module_init(ThreadTest_init);
module_exit(ThreadTest_exit);

MODULE_LICENSE("Dual BSD/GPL");
