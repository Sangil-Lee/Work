#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

int hello_init(void)
{
	printk("hello_init is called ...\n");		
	return 0;
}

void hello_exit(void)
{
	printk("hello_exit is called ...\n");		
}

module_init(hello_init);
module_exit(hello_exit);

//absolute macro
MODULE_LICENSE("GPL");
