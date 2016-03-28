#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

extern void my_favorate_url(void);

int my_sym2_start(void)
{
	printk(KERN_INFO "MY Symbol(2) Module is Loaded!! ....\n");
	my_favorate_url();

	return 0;
}

void my_sym2_end(void)
{
	printk("MY Symbol(2) Module is Unloaded ....\n");
}

module_init(my_sym2_start);
module_exit(my_sym2_end);

//EXPORT_NO_SYMBOLS;
