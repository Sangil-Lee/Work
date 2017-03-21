#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/delay.h>

//#include <linux/slab.h>
#include <linux/gfp.h>
#include <linux/mm.h>

static struct page *my_page=NULL;
static void *my_page_address=NULL;
static unsigned long my_page_address2=0;

int allocPageTest_init(void)
{
	int *tmp1, *tmp2, i;

	printk(KERN_INFO "allocPageTest Module Load Start!! ....\n");

	//page allocation
	my_page = alloc_pages(GFP_KERNEL, 1);
	if(!my_page) return -1;
	printk("my_page:%p\n", my_page);

#if 0
	for(i=0, tmp1=(int*)my_page; i<10; i++) {
		tmp1[i] = 20+i;
	}
#endif

	my_page_address = page_address(my_page);
	if(!my_page_address) return -2;
	printk("my_page_address:%p\n", my_page_address);

	for(i=0, tmp1=(int*)my_page_address; i<10; i++) {
		tmp1[i] = 50+i;
	}

	my_page_address2 = __get_free_pages(GFP_KERNEL, 1);
	if(!my_page_address2) return -3;
	printk("my_page_address2:%lx\n", my_page_address2);

	for(i=0, tmp1=(int*)my_page_address2; i<10; i++) {
		tmp1[i] = 70+i;
	}

	for(i=0, tmp2=(int*)my_page_address; i<10; i++) {
		printk("my_page_address[%d]:%d\n", i, tmp2[i]);
	}
	for(i=0, tmp2=(int*)my_page_address2; i<10; i++) {
		printk("my_page_address2[%d]:%d\n", i, tmp2[i]);
	}

	printk(KERN_INFO "allocPageTest Module Load End!! ....\n");

	return 0;
}

void allocPageTest_exit(void)
{
	printk("allocPageTest Module Unload Start....\n");

	if(my_page) {
		__free_pages(my_page, 1);
		printk("page free 1...\n");
	}
	if(my_page_address2) {
		free_pages(my_page_address2, 1);
		printk("page free 2...\n");
	}

	printk("allocPageTest Module Unload End....\n");
}

module_init(allocPageTest_init);
module_exit(allocPageTest_exit);

MODULE_LICENSE("Dual BSD/GPL");


/*
 #tail -f /var/log/messages
*/
