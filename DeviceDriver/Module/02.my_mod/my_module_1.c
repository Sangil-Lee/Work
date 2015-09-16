#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

//int MY_NUM;
#if 1
int MY_NUM;
EXPORT_SYMBOL(MY_NUM);
#endif

int my_init(void)
{
	MY_NUM++;
	printk("<1> MY Module_1 is Loaded1(MY_NUM:%d)!! ....\n", MY_NUM);
	printk("<2> MY Module_1 is Loaded2(MY_NUM:%d)!! ....\n", MY_NUM);
	printk("<3> MY Module_1 is Loaded3(MY_NUM:%d)!! ....\n", MY_NUM);
	printk("<4> MY Module_1 is Loaded4(MY_NUM:%d)!! ....\n", MY_NUM);
	printk("<5> MY Module_1 is Loaded5(MY_NUM:%d)!! ....\n", MY_NUM);
	printk("<6> MY Module_1 is Loaded6(MY_NUM:%d)!! ....\n", MY_NUM);
	printk("<7> MY Module_1 is Loaded7(MY_NUM:%d)!! ....\n", MY_NUM);
	printk(KERN_EMERG "Hi,Hi, I'm my_module....\n");
	//MOD_INC_USE_COUNT;
	return 0;
}

void my_exit(void)
{
	printk(" MY Module_1 is Unloaded(MY_NUM:%d)....\n", MY_NUM);
	//MOD_DEC_USE_COUNT;
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("Dual BSD/GPL");
