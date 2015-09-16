#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

#include <linux/fs.h>
#include <asm/uaccess.h>		//copy_from_user(). copy_to_user()
#include <linux/slab.h>			//kmalloc

#include "sk.h"

#include <linux/cdev.h>			// struct cdev, ..

#include <linux/device.h>		//struct class,...

int sk_major=0, sk_minor=0;
dev_t sk_dev;
struct cdev sk_cdev;

int sk_open (struct inode *inode, struct file *filp)
{
	printk(KERN_WARNING "[SK_DD]open..\n");
	
	return 0;
}

int sk_release (struct inode *inode, struct file *filp)
{
	printk(KERN_WARNING "[SK_DD]release..\n");
	return 0;
}

ssize_t sk_read (struct file *filp, char *buf, size_t count, loff_t *f_pos)
{
	char *k_data;
	int ret;
	int i;

	printk(KERN_WARNING "[SK_DD]read..\n");

	k_data = kmalloc(count+1, GFP_KERNEL);
	if(k_data==NULL) return -1;

	//read from device
	for(i=0; i<count; i++) k_data[i] = 'A'+i;
	k_data[i] = '\0';

	ret = copy_to_user(buf, k_data, count);
	if(ret < 0) return -1;

	printk(KERN_WARNING "[SK_DD]read done(%s)..\n", k_data);

	return count;
}


ssize_t sk_write (struct file *filp, const char *buf, size_t count, loff_t *f_pos)
{
	char *k_data;
	int ret;

	printk(KERN_WARNING "[SK_DD]write..\n");

	k_data = kmalloc(count+1, GFP_KERNEL);
	if(k_data==NULL) return -1;

	ret = copy_from_user(k_data, buf, count);
	if(ret < 0) return -1;
	k_data[count] = '\0';

	//write to device
	printk(KERN_WARNING "[SK_DD]write done(%s)..\n", k_data);


	return count;
}


//int sk_ioctl(struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg)
long sk_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	SK_INFO	*info = (SK_INFO*)arg;
	int size;
	int err;

	printk(KERN_WARNING "[SK_DD]ioctl(cmd:%x)..\n", cmd);

	if(_IOC_TYPE(cmd) != SK_MAGIC) return -EINVAL;
	if(_IOC_NR(cmd) > SK_MAXNR) return -EINVAL;

	size = _IOC_SIZE(cmd);
	if(size) {
		err = 0;
		if(_IOC_DIR(cmd) & _IOC_READ) {
			//err = verify_area(VERIFY_READ, (void*)arg, size);
			err = access_ok(VERIFY_READ, (void*)arg, size);
		} else if(_IOC_DIR(cmd) & _IOC_WRITE) {
			//err = verify_area(VERIFY_WRITE, (void*)arg, size);
			err = access_ok(VERIFY_WRITE, (void*)arg, size);
		}
		if(!err) return -1;
	}

	switch(cmd) {
	case SK_LED_OFF :
		printk("[SK_DD] ioctl => SK_LED_OFF..\n");
		break;
	case SK_LED_ON :
		printk("[SK_DD] ioctl => SK_LED_ON..\n");
		break;
	case SK_GETSTATE :
		printk("[SK_DD] ioctl => SK_GETSTATE..\n");
		break;
	case SK_READ :
		printk("[SK_DD] ioctl => SK_READ..\n");
		sk_read(filp, info->buff, info->size, NULL);
		break;
	case SK_WRITE :
		printk("[SK_DD] ioctl => SK_WRITE(size:%d, msg:%s)..\n", info->size, info->buff);
		sk_write(filp, info->buff, info->size, NULL);

		break;
	case SK_RW :
		printk("[SK_DD] ioctl => SK_RW..\n");
		break;
	}
	return 0;
}

struct file_operations sk_fops = {
    read:       sk_read,
    write:      sk_write,
    open:       sk_open,
//    ioctl:		sk_ioctl,
    unlocked_ioctl:		sk_ioctl,
    release:    sk_release
};

int sk_register_cdev(void)
{
	int error;

	if(sk_major) {
		sk_dev = MKDEV(sk_major, sk_minor);
		error = register_chrdev_region(sk_dev, 1, "sk");
	} else {
		error = alloc_chrdev_region(&sk_dev, sk_minor, 1, "sk");
		sk_major = MAJOR(sk_dev);
	}
	if(error < 0) {
		printk(KERN_WARNING "sk:alloc_major error!!\n");
		return error;
	}

	printk("<1> sk:alloc_major ok! => major_num:%d\n", sk_major);


	// register_chrdev
	cdev_init(&sk_cdev, &sk_fops);
	sk_cdev.owner = THIS_MODULE;
	error = cdev_add(&sk_cdev, sk_dev, 1);
	if(error < 0) {
		printk(KERN_WARNING "sk:register_chrdev error!!\n");
		return error;
	}

	return 0;
}


#if 0
void sk_c_dev_release(struct device *device)
{
	printk("sk_c_dev_release()..\n");
}
#endif
void ldd_b_dev_release(struct device *device)
{
	printk("ldd_b_dev_release()..\n");
}

struct bus_type ldd_bus_type = {
	.name		= "ldd",
};

struct device ldd_b_dev = {
	.init_name	= "ldd0",
	.release	= ldd_b_dev_release,
};


int sk_init(void)
{
	int result;

	printk("<1> SK Module id Up....\n");

	result = sk_register_cdev();
	if(result < 0) {
		printk(KERN_WARNING "Module(sk) Register Fail!!\n");
		cdev_del(&sk_cdev);
		unregister_chrdev_region(sk_dev, 1);
		return result;
	}
	printk(KERN_WARNING "Module(sk) Register OK!!\n");

	result = bus_register(&ldd_bus_type);
	if(result < 0) {
		printk(KERN_WARNING "Bus Register Fail(%d)!!\n", result);
		cdev_del(&sk_cdev);
		unregister_chrdev_region(sk_dev, 1);
		return result;
	}
	printk(KERN_WARNING "Bus Register OK!!\n");

	ldd_b_dev.bus = &ldd_bus_type; //device register in bus also
	result = device_register(&ldd_b_dev);
	if(result < 0) {
		printk("Bus_Device(ldd_b_dev):Register Fail(%d)!!\n", result);
		cdev_del(&sk_cdev);
		unregister_chrdev_region(sk_dev, 1);
		bus_unregister(&ldd_bus_type);
		return result;
	}

	printk(KERN_WARNING "SK Module Insert Done!!\n");
	return 0;
}

void sk_exit(void)
{
	printk("<1> SK Module id Down....\n");

	cdev_del(&sk_cdev);
	unregister_chrdev_region(sk_dev, 1);

	device_unregister(&ldd_b_dev);

	bus_unregister(&ldd_bus_type);

	printk(KERN_WARNING "SK Module Delete Done!!\n");
}

module_init(sk_init);
module_exit(sk_exit);

MODULE_LICENSE("Dual BSD/GPL");
