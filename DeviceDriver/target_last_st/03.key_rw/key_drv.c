#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

#include <linux/fs.h>
#include <asm/uaccess.h>		//copy_from_user(). copy_to_user()
#include <linux/slab.h>			//kmalloc

#include <linux/cdev.h>			// struct cdev, ..

#include <linux/device.h>		//struct class,...


#include <mach/gpio.h>
#include <plat/gpio-cfg.h>

#define MODE_IN		0
#define MODE_OUT	1
#define MODE_AF		2

#define EX_GPB4         S5PV210_GPB(4)
#define EX_GPB5         S5PV210_GPB(5)
#define EX_GPB6         S5PV210_GPB(6)
#define EX_GPB7         S5PV210_GPB(7)
#define EX_GPB0         S5PV210_GPB(0)
#define EX_GPB1         S5PV210_GPB(1)
#define EX_GPA0_2       S5PV210_GPA0(2)
#define EX_GPA0_3       S5PV210_GPA0(3)

#define LED_D0 	EX_GPB5
#define LED_D1 	EX_GPB7
#define LED_D2 	EX_GPB6
#define LED_D3 	EX_GPB4
#define KEY_CTL	EX_GPB0
#define LED_C0	EX_GPB1
#define LED_C1	EX_GPA0_2


int key_major=0, key_minor=0;
dev_t key_dev;
struct cdev key_cdev;

void EXT_LED_Init(void){
	//GPB_0,1,4,5,6,7 : Output
	//Outp32(GPBCON,(Inp32(GPBCON)  &= (~0xFFFF00FF)));
	//Outp32(GPBCON,(Inp32(GPBCON)  |= 0x11110011));
	//GPB_0,1,4,5,6,7 : Pull-up
	//Outp32(GPBPUD,(Inp32(GPBPUD)  &= (~0x0000FF0F)));
	//Outp32(GPBPUD,(Inp32(GPBPUD)  |= 0x0000AA0A));
	s3c_gpio_cfgpin(LED_D0, S3C_GPIO_SFN(MODE_OUT));
	s3c_gpio_setpull(LED_D0, S3C_GPIO_PULL_NONE);
	s3c_gpio_cfgpin(LED_D1, S3C_GPIO_SFN(MODE_OUT));
	s3c_gpio_setpull(LED_D1, S3C_GPIO_PULL_NONE);
	s3c_gpio_cfgpin(LED_D2, S3C_GPIO_SFN(MODE_OUT));
	s3c_gpio_setpull(LED_D2, S3C_GPIO_PULL_NONE);
	s3c_gpio_cfgpin(LED_D3, S3C_GPIO_SFN(MODE_OUT));
	s3c_gpio_setpull(LED_D3, S3C_GPIO_PULL_NONE);

	s3c_gpio_cfgpin(LED_C0, S3C_GPIO_SFN(MODE_OUT));
	s3c_gpio_setpull(LED_C0, S3C_GPIO_PULL_NONE);


	//GPA0_2 : Output
	//Outp32(GPA0CON,(Inp32(GPA0CON) &= (~0x00000F00)));
	//Outp32(GPA0CON,(Inp32(GPA0CON) |= 0x00000100 ));
	//GPA0_2 : Pull-up
	//Outp32(GPA0PUD,(Inp32(GPA0PUD) &= (~0x00000030)));
	//Outp32(GPA0PUD,(Inp32(GPA0PUD) |= 0x00000020));
	s3c_gpio_cfgpin(LED_C1, S3C_GPIO_SFN(MODE_OUT));
	s3c_gpio_setpull(LED_C1, S3C_GPIO_PULL_NONE);

	//GPB_4,5,6,7 : 0 -> 1
	//Outp32(GPBDAT,(Inp32(GPBDAT)  &= (~0x000000F0)));
	//Outp32(GPBDAT,(Inp32(GPBDAT)  |= 0x000000F0));
	s3c_gpio_setpin(LED_D0, 1);
	s3c_gpio_setpin(LED_D1, 1);
	s3c_gpio_setpin(LED_D2, 1);
	s3c_gpio_setpin(LED_D3, 1);

#if 1
	//GPB_1 : 0 -> 1 -> 0
	//Outp32(GPBDAT,(Inp32(GPBDAT)  &= (~0x00000002)));
	//Outp32(GPBDAT,(Inp32(GPBDAT)  |= (0x00000002)));
	//Outp32(GPBDAT,(Inp32(GPBDAT)  &= (~0x00000002)));
	s3c_gpio_setpin(LED_C0, 0);
	s3c_gpio_setpin(LED_C0, 1);
	s3c_gpio_setpin(LED_C0, 0);

	//GPA0_2 : 0 -> 1 -> 0
	//Outp32(GPA0DAT,(Inp32(GPBDAT)  &= (~0x00000004)));
	//Outp32(GPA0DAT,(Inp32(GPBDAT)  |= (0x00000004)));
	//Outp32(GPA0DAT,(Inp32(GPBDAT)  &= (~0x00000004)));
	s3c_gpio_setpin(LED_C1, 0);
	s3c_gpio_setpin(LED_C1, 1);
	s3c_gpio_setpin(LED_C1, 0);
#endif

#if 1
	//GPB_0 : 1
	//Outp32(GPBDAT,(Inp32(GPBDAT)  |= 0x00000001));
	s3c_gpio_setpin(KEY_CTL, 1);
#endif
}

void EXT_LED_On(int num){
	////int led_pos[] = {0, 3, 1, 2, 4, 7, 5, 6};	//hw
	//int led_pos[] = {0, 2, 3, 1, 4, 6, 7, 5};	//sw apping
	int gpio_pos[] = {4, 6, 7, 5, 4, 6, 7, 5};	//sw apping
	int pos;

	printk("[LED_DD]==>LED_ON(%d)\n", num);
	if(num == 8) {
		//led0, led1, led2, led3
		//Outp32(GPBDAT,(Inp32(GPBDAT)  &= (~0x000000F0)));
		s3c_gpio_setpin(LED_D0, 0);
		s3c_gpio_setpin(LED_D1, 0);
		s3c_gpio_setpin(LED_D2, 0);
		s3c_gpio_setpin(LED_D3, 0);
		//low(right) 4 led activate
		//Outp32(GPA0DAT,(Inp32(GPA0DAT)  &= (~0x00000004)));
		//Outp32(GPA0DAT,(Inp32(GPA0DAT)  |= (0x00000004)));
		//Outp32(GPA0DAT,(Inp32(GPA0DAT)  &= (~0x00000004)));
		s3c_gpio_setpin(LED_C1, 0);
		s3c_gpio_setpin(LED_C1, 1);
		s3c_gpio_setpin(LED_C1, 0);

		//led4, led5, led6, led7
		//Outp32(GPBDAT,(Inp32(GPBDAT)  &= (~0x000000F0)));
		s3c_gpio_setpin(LED_D0, 0);
		s3c_gpio_setpin(LED_D1, 0);
		s3c_gpio_setpin(LED_D2, 0);
		s3c_gpio_setpin(LED_D3, 0);
		//high(left) 4 led activate
		//Outp32(GPBDAT,(Inp32(GPBDAT)  &= (~0x00000002)));
		//Outp32(GPBDAT,(Inp32(GPBDAT)  |= (0x00000002)));
		//Outp32(GPBDAT,(Inp32(GPBDAT)  &= (~0x00000002)));
		s3c_gpio_setpin(LED_C0, 0);
		s3c_gpio_setpin(LED_C0, 1);
		s3c_gpio_setpin(LED_C0, 0);
		
		return;
	}

	//pos = led_pos[num];
	pos = gpio_pos[num];

	printk("[LED_DD]==>LED_ON(%d,%d)\n", num, pos);
	if(num < 0 || num > 8) {
		printk("[LED_DD]Invalid Led Num!!\n");
	} else {
		s3c_gpio_setpin(S5PV210_GPB(pos), 0);
		if(num < 4) {
		//led_num on
		//Outp32(GPBDAT,(Inp32(GPBDAT)  &= ~(1 << (pos + 4))));

		//low(right) 4 led activate
		//Outp32(GPA0DAT,(Inp32(GPA0DAT)  &= (~0x00000004)));
		//Outp32(GPA0DAT,(Inp32(GPA0DAT)  |= (0x00000004)));
		//Outp32(GPA0DAT,(Inp32(GPA0DAT)  &= (~0x00000004)));
		s3c_gpio_setpin(LED_C1, 0);
		s3c_gpio_setpin(LED_C1, 1);
		s3c_gpio_setpin(LED_C1, 0);
		} else if(num < 8) {
		//led_num on
		//Outp32(GPBDAT,(Inp32(GPBDAT)  &= ~(1 << (pos ))));
		//s3c_gpio_setpin(S5PV210_GPB(pos), 0);

		//high(left) 4 led activate
		//Outp32(GPBDAT,(Inp32(GPBDAT)  &= (~0x00000002)));
		//Outp32(GPBDAT,(Inp32(GPBDAT)  |= (0x00000002)));
		//Outp32(GPBDAT,(Inp32(GPBDAT)  &= (~0x00000002)));
		s3c_gpio_setpin(LED_C0, 0);
		s3c_gpio_setpin(LED_C0, 1);
		s3c_gpio_setpin(LED_C0, 0);
		}
	}
}


void EXT_LED_Off(int num){
	////int led_pos[] = {0, 3, 1, 2, 4, 7, 5, 6};	//hw
	//int led_pos[] = {0, 2, 3, 1, 4, 6, 7, 5};	//sw apping
	int gpio_pos[] = {4, 6, 7, 5, 4, 6, 7, 5};	//sw apping
	int pos;

	printk("[LED_DD]==>LED_OFF(%d)\n", num);
	if(num == 8) {
		//led0, led1, led2, led3
		//Outp32(GPBDAT,(Inp32(GPBDAT)  |= (0x000000F0)));
		s3c_gpio_setpin(LED_D0, 1);
		s3c_gpio_setpin(LED_D1, 1);
		s3c_gpio_setpin(LED_D2, 1);
		s3c_gpio_setpin(LED_D3, 1);
		//low(right) 4 led activate
		//Outp32(GPA0DAT,(Inp32(GPA0DAT)  &= (~0x00000004)));
		//Outp32(GPA0DAT,(Inp32(GPA0DAT)  |= (0x00000004)));
		//Outp32(GPA0DAT,(Inp32(GPA0DAT)  &= (~0x00000004)));
		s3c_gpio_setpin(LED_C1, 0);
		s3c_gpio_setpin(LED_C1, 1);
		s3c_gpio_setpin(LED_C1, 0);

		//led4, led5, led6, led7
		//Outp32(GPBDAT,(Inp32(GPBDAT)  |= (0x000000F0)));
		s3c_gpio_setpin(LED_D0, 1);
		s3c_gpio_setpin(LED_D1, 1);
		s3c_gpio_setpin(LED_D2, 1);
		s3c_gpio_setpin(LED_D3, 1);
		//high(left) 4 led activate
		//Outp32(GPBDAT,(Inp32(GPBDAT)  &= (~0x00000002)));
		//Outp32(GPBDAT,(Inp32(GPBDAT)  |= (0x00000002)));
		//Outp32(GPBDAT,(Inp32(GPBDAT)  &= (~0x00000002)));
		s3c_gpio_setpin(LED_C0, 0);
		s3c_gpio_setpin(LED_C0, 1);
		s3c_gpio_setpin(LED_C0, 0);
		
		return;
	}

	//pos = led_pos[num];
	pos = gpio_pos[num];

	printk("[LED_DD]==>LED_OFF(%d, %d)\n", num, pos);
	if(num < 0 || num > 8) {
		printk("[LED_DD]Invalid Led Num!!\n");
	} else {
		s3c_gpio_setpin(S5PV210_GPB(pos), 1);
		if(num < 4) {
		//led_num off
		//Outp32(GPBDAT,(Inp32(GPBDAT)  |= (1 << (pos + 4))));

		//low(right) 4 led activate
		//Outp32(GPA0DAT,(Inp32(GPA0DAT)  &= (~0x00000004)));
		//Outp32(GPA0DAT,(Inp32(GPA0DAT)  |= (0x00000004)));
		//Outp32(GPA0DAT,(Inp32(GPA0DAT)  &= (~0x00000004)));
		s3c_gpio_setpin(LED_C1, 0);
		s3c_gpio_setpin(LED_C1, 1);
		s3c_gpio_setpin(LED_C1, 0);
		} else if(num < 8) {
		//led_num off
		//Outp32(GPBDAT,(Inp32(GPBDAT)  |= (1 << (pos ))));
		//s3c_gpio_setpin(S5PV210_GPB(pos), 1);

		//high(left) 4 led activate
		//Outp32(GPBDAT,(Inp32(GPBDAT)  &= (~0x00000002)));
		//Outp32(GPBDAT,(Inp32(GPBDAT)  |= (0x00000002)));
		//Outp32(GPBDAT,(Inp32(GPBDAT)  &= (~0x00000002)));
		s3c_gpio_setpin(LED_C0, 0);
		s3c_gpio_setpin(LED_C0, 1);
		s3c_gpio_setpin(LED_C0, 0);
		}
	}
}

void EXT_KEY_Init(void){
	s3c_gpio_cfgpin(LED_D0, S3C_GPIO_SFN(MODE_IN));
	s3c_gpio_setpull(LED_D0, S3C_GPIO_PULL_UP);
	s3c_gpio_cfgpin(LED_D1, S3C_GPIO_SFN(MODE_IN));
	s3c_gpio_setpull(LED_D1, S3C_GPIO_PULL_UP);
	s3c_gpio_cfgpin(LED_D2, S3C_GPIO_SFN(MODE_IN));
	s3c_gpio_setpull(LED_D2, S3C_GPIO_PULL_UP);
	s3c_gpio_cfgpin(LED_D3, S3C_GPIO_SFN(MODE_IN));
	s3c_gpio_setpull(LED_D3, S3C_GPIO_PULL_UP);

	s3c_gpio_cfgpin(KEY_CTL, S3C_GPIO_SFN(MODE_OUT));
	s3c_gpio_setpull(KEY_CTL, S3C_GPIO_PULL_NONE);

	s3c_gpio_setpin(KEY_CTL, 1);
}



int key_open (struct inode *inode, struct file *filp)
{
	printk(KERN_WARNING "[KEY_DD]open..\n");
	
	return 0;
}

int key_release (struct inode *inode, struct file *filp)
{
	printk(KERN_WARNING "[KEY_DD]release..\n");
	return 0;
}

ssize_t key_read (struct file *filp, char *buf, size_t count, loff_t *f_pos)
{
	int ret;
	int key_stat=0;

	printk(KERN_WARNING "[KEY_DD]read..\n");

	s3c_gpio_setpin(KEY_CTL, 0);

	if(!gpio_get_value(LED_D0)) key_stat = 1;
	else if(!gpio_get_value(LED_D1)) key_stat = 2;
	else if(!gpio_get_value(LED_D2)) key_stat = 3;
	else if(!gpio_get_value(LED_D3)) key_stat = 4;

	ret = copy_to_user(buf, &key_stat, count);
	if(ret<0) return -1;

	s3c_gpio_setpin(KEY_CTL, 1);

	EXT_LED_Init();
	EXT_LED_Off(8);
	EXT_LED_On(key_stat-1);
	EXT_KEY_Init();

	return count;
}

ssize_t key_write (struct file *filp, const char *buf, size_t count, loff_t *f_pos)
{
	int ret, k_data;

	printk(KERN_WARNING "[KEY_DD]write..\n");
	ret = copy_from_user(&k_data, buf, count);
	if(ret<0) return -1;

	return count;
}

int key_ioctl(struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg)
{
	printk(KERN_WARNING "[KEY_DD]ioctl(cmd:%x)..\n", cmd);

	return 0;
}

struct file_operations key_fops = {
    read:       key_read,
    write:      key_write,
    open:       key_open,
    ioctl:		key_ioctl,
    release:    key_release
};

int key_register_cdev(void)
{
	int error;

	if(key_major) {
		key_dev = MKDEV(key_major, key_minor);
		error = register_chrdev_region(key_dev, 1, "key");
	} else {
		error = alloc_chrdev_region(&key_dev, key_minor, 1, "key");
		key_major = MAJOR(key_dev);
	}
	if(error < 0) {
		printk(KERN_WARNING "key:alloc_major error!!\n");
		return error;
	}

	printk("<1> key:alloc_major ok! => major_num:%d\n", key_major);


	// register_chrdev
	cdev_init(&key_cdev, &key_fops);
	key_cdev.owner = THIS_MODULE;
	error = cdev_add(&key_cdev, key_dev, 1);
	if(error < 0) {
		printk(KERN_WARNING "key:register_chrdev error!!\n");
		return error;
	}

	//EXT_KEY_Init();

	return 0;
}


void key_c_dev_release(struct device *device)
{
	printk("key_c_dev_release()..\n");
}
void ldd_b_dev_release(struct device *device)
{
	printk("ldd_b_dev_release()..\n");
}

int key_driver_probe(struct device *dev)
{
	printk("key_driver_probe()..\n");
	EXT_KEY_Init();
	EXT_LED_Init();
	return 0;
}
int key_driver_remove(struct device *dev)
{
	printk("key_driver_remove()..\n");
	return 0;
}

struct bus_type ldd_bus_type = {
	.name		= "ldd",
};

struct device ldd_b_dev = {
	.init_name	= "ldd0",
	.release	= ldd_b_dev_release,
};

struct device_driver key_driver = {
	.name		= "key_driver",
	.owner		= THIS_MODULE,
	.bus		= &ldd_bus_type,
	.probe		= key_driver_probe,
	.remove		= key_driver_remove,
};

struct class *key_class;

struct device key_c_dev = {
	.init_name	= "key0",
	.driver		= &key_driver,
	.parent		= &ldd_b_dev,
	.release	= key_c_dev_release,
};


int key_init(void)
{
	int result;

	printk("<1> KEY Module id Up....\n");

	result = key_register_cdev();
	if(result < 0) {
		printk(KERN_WARNING "Module(key) Register Fail!!\n");
		cdev_del(&key_cdev);
		unregister_chrdev_region(key_dev, 1);
		return result;
	}
	printk(KERN_WARNING "Module(key) Register OK!!\n");


	result = bus_register(&ldd_bus_type);
	if(result < 0) {
		printk(KERN_WARNING "Bus Register Fail(%d)!!\n", result);
		cdev_del(&key_cdev);
		unregister_chrdev_region(key_dev, 1);
		return result;
	}
	printk(KERN_WARNING "Bus Register OK!!\n");

	ldd_b_dev.bus = &ldd_bus_type;
	result = device_register(&ldd_b_dev);
	if(result < 0) {
		printk("Bus_Device(ldd_b_dev):Register Fail(%d)!!\n", result);
		cdev_del(&key_cdev);
		unregister_chrdev_region(key_dev, 1);
		bus_unregister(&ldd_bus_type);
		return result;
	}

	result = driver_register(&key_driver);
	if(result < 0) {
		printk("Driver(key_driver):Register Fail(%d)!!\n", result);
		cdev_del(&key_cdev);
		unregister_chrdev_region(key_dev, 1);
		device_unregister(&ldd_b_dev);
		bus_unregister(&ldd_bus_type);
		return result;
	}
	printk("Driver(key_driver):Register OK!!\n");

	key_class = class_create(THIS_MODULE, "key");
	if(IS_ERR(key_class)) {
		printk("Class Create Fail!!\n");
		cdev_del(&key_cdev);
		unregister_chrdev_region(key_dev, 1);
		device_unregister(&ldd_b_dev);
		driver_unregister(&key_driver);
		bus_unregister(&ldd_bus_type);
		return result;
	}
	printk("Class Create OK!!\n");


	key_c_dev.class = key_class;
	key_c_dev.devt = key_dev;
	result = device_register(&key_c_dev);
	if(result < 0) {
		printk("Class_Device(key_c_dev):Register Fail(%d)!!\n", result);
		cdev_del(&key_cdev);
		unregister_chrdev_region(key_dev, 1);
		device_unregister(&ldd_b_dev);
		driver_unregister(&key_driver);
		bus_unregister(&ldd_bus_type);
		class_destroy(key_class);
		return result;
	}
	printk("Class_Device(key_c_dev):Register OK!!\n");


	printk(KERN_WARNING "KEY Module Insert Done!!\n");
	return 0;
}

void key_exit(void)
{
	printk("<1> KEY Module id Down....\n");

	cdev_del(&key_cdev);
	unregister_chrdev_region(key_dev, 1);

	device_unregister(&key_c_dev);
	device_unregister(&ldd_b_dev);
	driver_unregister(&key_driver);

	class_destroy(key_class);
	bus_unregister(&ldd_bus_type);

	printk(KERN_WARNING "KEY Module Delete Done!!\n");
}

module_init(key_init);
module_exit(key_exit);

MODULE_LICENSE("Dual BSD/GPL");
