#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/delay.h>

.............


my_timer .......
{
	int i;
	for(i=1; i<=data; i++) {
		printk("Kernel Timer Time-Out Function Doing_%d...\n", i);
	}
	printk("Kernel Timer Time-Out Function Done!!!\n");
}

int timerTest_init(void)
{
	printk(KERN_INFO "timerTest Module is Loaded!! ....\n");

	...........

	return 0;
}

void timerTest_exit(void)
{
	............
	printk("timerTest Module is Unloaded ....\n");
}

module_init(timerTest_init);
module_exit(timerTest_exit);

MODULE_LICENSE("Dual BSD/GPL");


/*
 #tail -f /var/log/messages
*/
