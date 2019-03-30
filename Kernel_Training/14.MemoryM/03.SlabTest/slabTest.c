#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/delay.h>

#include <linux/slab.h>
//#include <linux/gfp.h>
//#include <linux/mm.h>

//#define	HOST

struct my_struct {
	//char name[20];
	char name[50];
	int age;
	int score;
};

static struct kmem_cache *my_cachep=NULL;
struct my_struct *my_obj=NULL;

int slabTest_init(void)
{
	printk(KERN_INFO "slabTest Module is Load Start!! ....\n");

	//create cache
	my_cachep = kmem_cache_create(
				"my_struct",			//name
				sizeof(struct my_struct),					//object size
				0,					//alignment
				//SLAB_HWCACHE_ALIGN,	//flags
				0,					//flags
				NULL				//constructor
			);

	if(my_cachep==NULL) return -1;
	
#ifndef HOST
	printk("Cache Name : %s\n", kmem_cache_name(my_cachep));
#endif
	printk("Cache Object Size : %d\n", kmem_cache_size(my_cachep));

	//alloc object memory from cache
	if((my_obj = kmem_cache_alloc(my_cachep, GFP_KERNEL))) {

		strcpy(my_obj->name, "kim");
		my_obj->age = 17;
		my_obj->score = 77;
#if 0
		for(i=0, tmp1=(int*)my_obj; i<6; i++) {
			tmp1[i] = 10+i;
		}
		for(i=0, tmp2=(int*)my_obj; i<6; i++) {
			printk("obj_%d : %d\n", i, tmp2[i]);
		}

		kmem_cache_free(my_cachep, my_obj);
		for(i=0, tmp2=(int*)my_obj; i<6; i++) {
			printk("obj_%d : %d\n", i, tmp2[i]);
		}
#endif

	} else {
		printk("my_cachep object alloc error!!\n");

		//destroy slab cache
		kmem_cache_destroy(my_cachep);
		return -2;
	}

	printk("my_obj:%s,%d,%d\n", my_obj->name, my_obj->age, my_obj->score);

	printk(KERN_INFO "slabTest Module is Load End!! ....\n");

	return 0;
}

void slabTest_exit(void)
{
	printk("slabTest Module is Unload Start....\n");

	//free object memory
	if(my_obj) {
		kmem_cache_free(my_cachep, my_obj);
		printk("free object memory....\n");
		//printk("my_obj:%s,%d,%d\n", my_obj->name, my_obj->age, my_obj->score);
	}

	//destroy slab cache
	if(my_cachep) {
		kmem_cache_destroy(my_cachep);
		printk("destroy slab cache....\n");
		//printk("Cache Name : %s\n", kmem_cache_name(my_cachep));
		//printk("Cache Object Size : %d\n", kmem_cache_size(my_cachep));
	}

	printk("slabTest Module is Unload End....\n");
}

module_init(slabTest_init);
module_exit(slabTest_exit);

MODULE_LICENSE("Dual BSD/GPL");


/*
 #tail -f /var/log/messages
*/
