#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>


int hello_init(void)
{
	printk(KERN_WARNING "hello_init...\n");
	return 0;
}

void hello_exit(void)
{
	printk("hello_exit...\n");
}


module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");


