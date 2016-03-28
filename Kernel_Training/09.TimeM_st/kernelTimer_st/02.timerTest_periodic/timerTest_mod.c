#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/delay.h>

#include <linux/timer.h>

struct timer_list timer;


void my_timer(unsigned long data)
{
	int i;
	for(i=1; i<=data; i++) {
		printk("Kernel Timer Time-Out Function Doing_%d...\n", i);
	}

	timer.expires = jiffies + 2*HZ;
	add_timer(&timer);

	printk("Kernel Timer Time-Out Function Done!!!\n");
}

int timerTest_init(void)
{
	printk(KERN_INFO "timerTest Module is Loaded!! ....\n");

	init_timer(&timer);
	//timer.expires = get_jiffies_64() + 3*HZ;
	timer.expires = jiffies + 3*HZ;
	timer.function = my_timer;
	timer.data = 5;
	add_timer(&timer);

	return 0;
}

void timerTest_exit(void)
{
	del_timer(&timer);
	printk("timerTest Module is Unloaded ....\n");
}

module_init(timerTest_init);
module_exit(timerTest_exit);

MODULE_LICENSE("Dual BSD/GPL");


/*
 #tail -f /var/log/messages
*/
