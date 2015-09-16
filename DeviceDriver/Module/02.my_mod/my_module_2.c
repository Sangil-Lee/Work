#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

//int MY_NUM;
extern int MY_NUM;


int my_start(void)
{
	MY_NUM++;
	printk("<4> MY Module_2 is Loaded(MY_NUM:%d)!! ....\n", MY_NUM);

	return 0;
}

void my_end(void)
{
	printk(" MY Module_2 is Unloaded(MY_NUM:%d)....\n", MY_NUM);
}

module_init(my_start);
module_exit(my_end);

//EXPORT_NO_SYMBOLS;
