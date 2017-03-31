#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/delay.h>

#include <linux/list.h>		//struct list_head
#include <linux/stddef.h>	//offsetof
#include <linux/kernel.h>	//container_of

#include <linux/slab.h>		//kmalloc

struct info {
	int type;
	int color;
	struct list_head list;
};

struct info goods1 = {
	.type = 1,
	.color = 3,
	.list = LIST_HEAD_INIT(goods1.list)
};
struct info goods2 = {
	.type = 2,
	.color = 1,
	.list = LIST_HEAD_INIT(goods2.list)
};
struct info goods3 = {
	.type = 3,
	.color = 2,
	.list = LIST_HEAD_INIT(goods2.list)
};

static LIST_HEAD(goods_list);

int listTest_init(void)
{
	struct list_head *lp;
	struct info *gp;

	printk(KERN_INFO "listTest Module Load Start!! ....\n");

	printk("size => %d, %d, %d\n", 
		sizeof(struct list_head), sizeof(int), sizeof(struct info));
	printk("address => %p, %p, %p, %p\n", 
		&goods1, &goods1.type, &goods1.color, &goods1.list);
	printk("offset => %u, %u, %u\n",
		........ (struct info, type),
		offsetof(struct info, color),
		offsetof(struct info, list));
	printk("container_of => %p, %p\n",
		.......... ((int *)&goods1.type, struct info, type),
		container_of((struct list_head *)&goods1.list, struct info, list));


	list_add(&goods1.list, &goods_list);
	list_add(&goods2.list, &goods_list);
	list_add(&goods3.list, &goods_list);

	//gp = malloc(sizeof(*gp));
	gp = kmalloc(sizeof(*gp), GFP_KERNEL);
	gp->type = 2;
	gp->color = 2;
	list_add_tail(&gp->list, &goods_list);

	........ (lp, &goods_list) {
		gp = ........ (lp, struct info, list);
		printk("goods : %d, %d\n", gp->type, gp->color);
	}
	printk(KERN_INFO "listTest Module Load End!! ....\n");

	return 0;
}

void listTest_exit(void)
{
	printk("listTest Module Unload Start....\n");


	printk("listTest Module Unload End....\n");
}

module_init(listTest_init);
module_exit(listTest_exit);

MODULE_LICENSE("Dual BSD/GPL");


/*
 #tail -f /var/log/messages
*/
