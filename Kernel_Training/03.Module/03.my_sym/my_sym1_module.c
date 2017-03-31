#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

char * my_url="www.naver.com";


void my_favorate_url(void)
{
	printk(KERN_INFO "MY favorate URL : %s\n", my_url);
}

EXPORT_SYMBOL(my_favorate_url);

int my_sym1_start(void)
{
	printk(KERN_INFO "MY Symbol(1) Module is Loaded!! ....\n");

	return 0;
}

void my_sym1_end(void)
{
	printk("MY Symbol(1) Module is Unloaded ....\n");
}

module_init(my_sym1_start);
module_exit(my_sym1_end);

//EXPORT_NO_SYMBOLS;
