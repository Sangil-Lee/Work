#include <linux/module.h>	/* Needed by all modules */
#include <linux/kernel.h>	/* Needed for KERN_INFO	*/
#include <linux/init.h>		/* Needed for the macros */

static int __init init_hello(void)
{
	printk(KERN_INFO "Hi, Hello Module.\n");
	return 0;
}

module_init(init_hello);
