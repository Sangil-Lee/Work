#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/delay.h>

#include <linux/list.h>		//struct list_head
#include <linux/stddef.h>	//offsetof
#include <linux/kernel.h>	//container_of

#include <linux/slab.h>		//kmalloc

#include <linux/kfifo.h>
#include <linux/spinlock.h>
#include <linux/err.h>		//IS_ERR


//#define	HOST
//#define	CHAR_TEST


int kfifo_test(void)
{
#ifdef HOST
	struct kfifo myfifo;
	//DECLARE_KFIFO(myfifo, int, 10);
#else
	struct kfifo *myfifo=NULL;
	spinlock_t my_lock;
#endif
	int data1[10] = {10,20,30,40,50,60,70,80,90,100};
	int *data2;
#ifdef CHAR_TEST
	char tdata2=0;
#else
	int tdata=0;
#endif
	int rst=0, no_mem=0, i, err;

	printk("kfifo test start..\n");

#ifndef HOST
	spin_lock_init(&my_lock);
#endif

	//size : power of 2, kfifo_init() auto call
#ifdef HOST
	rst = kfifo_alloc(&myfifo, (unsigned int)1024, GFP_KERNEL);
	//printk("11111=>kfifo_len:%d, rst:%d\n", kfifo_len(&myfifo), rst);
	if(rst) return -1;
	//INIT_KFIFO(myfifo);
#else
	myfifo = kfifo_alloc((unsigned int)1024, GFP_KERNEL, &my_lock);
	//myfifo = kfifo_alloc(PAGE_SIZE, GFP_KERNEL, &my_lock);
	if(IS_ERR(myfifo)) return -1;
#endif

	no_mem = sizeof(data1)/sizeof(int);
	for(i=0; i<no_mem; i++) {
#ifdef HOST
	#ifdef CHAR_TEST
		rst = kfifo_put(&myfifo, &data1[i]);
	#else
		rst = kfifo_in(&myfifo, &data1[i], 4);
	#endif
		//printk("22222=>kfifo_len:%d, rst:%d\n", kfifo_len(&myfifo), rst);
#else
		kfifo_put(myfifo, (void*)(data1+i), sizeof(int));
#endif
	}

	data2 = kmalloc(no_mem*sizeof(int), GFP_KERNEL);
#ifdef HOST
	for(i=0, err=0; kfifo_len(&myfifo); i++) {
		//printk("33333=>kfifo_len:%d, rst:%d\n", kfifo_len(&myfifo), rst);
	#ifdef CHAR_TEST
		rst = kfifo_get(&myfifo, &tdata2);
		//printk("tdata2:%d, rst:%d\n", tdata2, rst);
	#else
		rst = kfifo_out(&myfifo, &tdata, 4);
		//printk("tdata:%d, rst:%d\n", tdata, rst);
	#endif
		if(rst == 0) {
#else
	for(i=0, err=0; kfifo_len(myfifo); i++) {
		rst = kfifo_get(myfifo, (void*)&tdata, sizeof(tdata));
		if(rst != sizeof(tdata)) {
#endif
			err++;
			break;
		}
	#ifdef CHAR_TEST
		data2[i] = tdata2;
	#else
		data2[i] = tdata;
	#endif
	}

	for(i=0; i<no_mem; i++) {
		printk("data2[%d]:%d\n", i, data2[i]);
	}
#ifdef HOST
	kfifo_free(&myfifo);
#else
	kfifo_free(myfifo);
#endif
	kfree(data2);

	printk("kfifo test end..\n");
	return err;
}

int kfifoTest_init(void)
{
	printk(KERN_INFO "kfifoTest Module Load Start!! ....\n");

	if(!kfifo_test()) printk("kfifo test ok!!\n");
	else printk("kfifo test fail!!\n");

	printk(KERN_INFO "kfifoTest Module Load End!! ....\n");

	return 0;
}

void kfifoTest_exit(void)
{
	printk("kfifoTest Module Unload Start....\n");


	printk("kfifoTest Module Unload End....\n");
}

module_init(kfifoTest_init);
module_exit(kfifoTest_exit);

MODULE_LICENSE("Dual BSD/GPL");


/*
 #tail -f /var/log/messages
*/
