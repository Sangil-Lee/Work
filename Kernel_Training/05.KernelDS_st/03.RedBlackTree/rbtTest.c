#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/delay.h>

#include <linux/list.h>		//struct list_head
#include <linux/stddef.h>	//offsetof
#include <linux/kernel.h>	//container_of

#include <linux/slab.h>		//kmalloc

#include <linux/rbtree.h>


struct info {
	int val;
	struct rb_node node;
};

struct info *my_rb_search(struct rb_root *root, int val)
{
	struct rb_node *node = root->rb_node;
	struct info *tdata;

	while(node) {
		tdata = rb_entry(node, struct info, node);
		//printk("val-->%d\n", tdata->val);
		if(tdata->val > val) {
			//printk("goto left..\n");
			node = node->rb_left;
		} else if(tdata->val < val) {
			//printk("goto right..\n");
			node = node->rb_right;
		} else {
			//printk("find!!\n");
			return tdata;
		}
	}
	return NULL;
}

void my_rb_insert(struct rb_root *root, struct info *data)
{
	struct rb_node **link = &root->rb_node, *parent=NULL;
	int val = data->val;
	struct info *tdata;

	while(*link) {
		parent = *link;
		tdata = rb_entry(parent, struct info, node);
		if(tdata->val > val) link = &((*link)->rb_left);
		else link = &((*link)->rb_right);
	}

	rb_link_node(&(data->node), parent, link);
	rb_insert_color(&(data->node), root);
}

void my_rb_drop(struct rb_root *root)
{
	struct rb_node *node;

	printk("\nrb_drop..\n");
	for(node=rb_first(root); node; node=rb_next(node)) {
		kfree((void*)rb_entry(node, struct info, node));
	}
	root->rb_node = NULL;
}

void my_rb_print(struct rb_root *root)
{
	struct rb_node *node;

	printk("\nrb_node list => \n");
	for(node=rb_first(root); node; node=rb_next(node)) {
		printk("%d, ", rb_entry(node, struct info, node)->val);
	}
	printk("\n\n");
}

int rbtTest_init(void)
{
	struct rb_root my_root = RB_ROOT;
	struct info *data;
	int i;
#ifdef KIM_COM
	struct rb_node *node;
#endif

	printk(KERN_INFO "rbtTest Module Load Start!! ....\n");

	for(i=0, data=NULL; i<10; i++) {
		data = kmalloc(sizeof(struct info), GFP_KERNEL);
		data->val = 10+i;
		my_rb_insert(&my_root, data);
#ifdef KIM_COM
		data = NULL; node=NULL;
		node = rb_first(&my_root);
		data = rb_entry(node, struct info, node);
		printk("first_val : %d\n", data->val);
		data = NULL; node=NULL;
		node = rb_last(&my_root);
		data = rb_entry(node, struct info, node);
		printk("last_val : %d\n", data->val);
		data = NULL; node=NULL;
		data = my_rb_search(&my_root, 10+i);
		printk("search_val : %d\n", data->val);
#endif
	}

	my_rb_print(&my_root);

	data = my_rb_search(&my_root, 15);
	if(data) {
		printk("rb search val => %d\n", data->val);
		rb_erase(&data->node, &my_root);
		kfree(data);
		printk("rb erased!!\n");
	} else printk("rb search error!!\n");
	my_rb_print(&my_root);
	
	data = my_rb_search(&my_root, 12);
	if(data) {
		printk("rb search val => %d\n", data->val);
		rb_erase(&data->node, &my_root);
		kfree(data);
		printk("rb erased!!\n");
	} else printk("rb search error!!\n");
	my_rb_print(&my_root);

	my_rb_drop(&my_root);
	my_rb_print(&my_root);

	printk(KERN_INFO "rbtTest Module Load End!! ....\n");

	return 0;
}

void rbtTest_exit(void)
{
	printk("rbtTest Module Unload Start....\n");


	printk("rbtTest Module Unload End....\n");
}

module_init(rbtTest_init);
module_exit(rbtTest_exit);

MODULE_LICENSE("Dual BSD/GPL");


/*
 #tail -f /var/log/messages
*/
