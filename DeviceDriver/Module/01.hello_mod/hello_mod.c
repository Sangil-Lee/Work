#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

/***
int init_module(void)
{
	printk("<1> Hello Module id Loaded ....\n");
	printk(KERN_INFO"Hi~~~\n");
	return 0;
}

void cleanup_module(void)
{
	printk(" Hello Module is Unloaded ....\n");
}
***/

int hello_init(void)
{
	printk("<4> Hello Module id Loaded ....\n");
	return 0;
}

void hello_exit(void)
{
	printk(" Hello Module is Unloaded ....\n");
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("Dual BSD/GPL");
