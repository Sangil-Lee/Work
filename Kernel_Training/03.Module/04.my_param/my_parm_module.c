#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

int my_age=10;
char *my_name;

module_param(my_age, int, 0);
module_param(my_name, charp, S_IRUSR|S_IWUSR);

int my_parm_start(void)
{
	printk(KERN_INFO "MY Parameter Module is Loaded!! ....\n");

	if((my_age == 0) || (my_name == NULL))
	{
		printk(KERN_INFO "Please Input Youre Parameters!!\n");
		printk(KERN_INFO "Usage : insmod my_parm_module.ko my_age=15 my_name=kim\n");
	} else {
		printk(KERN_INFO "Your Parameter is ....\n");
		printk(KERN_INFO "\tname : %s\n", my_name);
		printk(KERN_INFO "\tage : %d\n", my_age);
	}

	return 0;
}

void my_parm_end(void)
{
	printk("MY Parameter Module is Unloaded ....\n");
}

module_init(my_parm_start);
module_exit(my_parm_end);

MODULE_LICENSE("Dual BSD/GPL");
