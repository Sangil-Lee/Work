#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

#include <linux/fs.h>

#include <linux/cdev.h>			// struct cdev, ..

#include <asm/uaccess.h>		//copy_from_user(). copy_to_user()


#include <mach/gpio.h>
#include <plat/gpio-cfg.h>

#include <linux/slab.h>

int led_major=0, led_minor=0;
dev_t led_dev;
struct cdev led_cdev[4];

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


int led0_open (struct inode *inode, struct file *filp)
{
	printk(KERN_WARNING "[LED0_DD]open..\n");
	
	return 0;
}

int led0_release (struct inode *inode, struct file *filp)
{
	printk(KERN_WARNING "[LED0_DD]release..\n");
	return 0;
}

ssize_t led0_read (struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
	int k_data;
	int ret;

	printk(KERN_WARNING "[LED0_DD]read..\n");

	//read from device
	//k_data = inl(IO_ADDR);
	//k_data = readl(IO_ADDR);
	k_data = 55;

	ret = copy_to_user(buf, &k_data, count);
	//put_user(k_data, buf);
	if(ret < 0) return -1;

	printk(KERN_WARNING "[LED0_DD]read done(%d)..\n", k_data);

	return count;
}

ssize_t led0_write (struct file *filp, const char __user *buf, size_t count, loff_t *f_pos)
{
	int menu;
	int ret;

	printk(KERN_WARNING "[LED0_DD]write(count:%d)..\n", count);

	ret = copy_from_user(&menu, buf, count);
	if(ret < 0) return -1;	

	switch(menu) {
		case 1 : 	//LED ON
			EXT_LED_On(0);
			break;
		case 2 :	//LED OFF
			EXT_LED_Off(0);
			break;
	}
	
	printk(KERN_WARNING "[LED0_DD]write done(%d)..\n", menu);

	return count;
}

int led0_ioctl(struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg)
{
	printk(KERN_WARNING "[LED0_DD]ioctl..\n");
	switch(cmd) {
		case 3 :
			EXT_LED_Off(8);
			break;
		default :
			EXT_LED_Off(8);
			break;
	}
	
	return 0;
}


int led1_open (struct inode *inode, struct file *filp)
{
	printk(KERN_WARNING "[LED1_DD]open..\n");
	
	return 0;
}

int led1_release (struct inode *inode, struct file *filp)
{
	printk(KERN_WARNING "[LED1_DD]release..\n");
	return 0;
}

ssize_t led1_read (struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
	int k_data;
	int ret;

	printk(KERN_WARNING "[LED1_DD]read..\n");

	//read from device
	//k_data = inl(IO_ADDR);
	//k_data = readl(IO_ADDR);
	k_data = 55;

	ret = copy_to_user(buf, &k_data, count);
	//put_user(k_data, buf);
	if(ret < 0) return -1;

	printk(KERN_WARNING "[LED1_DD]read done(%d)..\n", k_data);

	return count;
}

ssize_t led1_write (struct file *filp, const char __user *buf, size_t count, loff_t *f_pos)
{
	int menu;
	int ret;

	printk(KERN_WARNING "[LED1_DD]write(count:%d)..\n", count);

	ret = copy_from_user(&menu, buf, count);
	if(ret < 0) return -1;	

	switch(menu) {
		case 1 : 	//LED ON
			EXT_LED_On(1);
			break;
		case 2 :	//LED OFF
			EXT_LED_Off(1);
			break;
	}
	
	printk(KERN_WARNING "[LED1_DD]write done(%d)..\n", menu);

	return count;
}

int led1_ioctl(struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg)
{
	printk(KERN_WARNING "[LED1_DD]ioctl..\n");
	switch(cmd) {
		case 3 :
			EXT_LED_Off(8);
			break;
		default :
			EXT_LED_Off(8);
			break;
	}
	
	return 0;
}


int led2_open (struct inode *inode, struct file *filp)
{
	printk(KERN_WARNING "[LED2_DD]open..\n");
	
	return 0;
}

int led2_release (struct inode *inode, struct file *filp)
{
	printk(KERN_WARNING "[LED2_DD]release..\n");
	return 0;
}

ssize_t led2_read (struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
	int k_data;
	int ret;

	printk(KERN_WARNING "[LED2_DD]read..\n");

	//read from device
	//k_data = inl(IO_ADDR);
	//k_data = readl(IO_ADDR);
	k_data = 55;

	ret = copy_to_user(buf, &k_data, count);
	//put_user(k_data, buf);
	if(ret < 0) return -1;

	printk(KERN_WARNING "[LED2_DD]read done(%d)..\n", k_data);

	return count;
}

ssize_t led2_write (struct file *filp, const char __user *buf, size_t count, loff_t *f_pos)
{
	int menu;
	int ret;

	printk(KERN_WARNING "[LED2_DD]write(count:%d)..\n", count);

	ret = copy_from_user(&menu, buf, count);
	if(ret < 0) return -1;	

	switch(menu) {
		case 1 : 	//LED ON
			EXT_LED_On(2);
			break;
		case 2 :	//LED OFF
			EXT_LED_Off(2);
			break;
	}
	
	printk(KERN_WARNING "[LED2_DD]write done(%d)..\n", menu);

	return count;
}

int led2_ioctl(struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg)
{
	printk(KERN_WARNING "[LED2_DD]ioctl..\n");
	switch(cmd) {
		case 3 :
			EXT_LED_Off(8);
			break;
		default :
			EXT_LED_Off(8);
			break;
	}
	
	return 0;
}


int led3_open (struct inode *inode, struct file *filp)
{
	printk(KERN_WARNING "[LED3_DD]open..\n");
	
	return 0;
}

int led3_release (struct inode *inode, struct file *filp)
{
	printk(KERN_WARNING "[LED3_DD]release..\n");
	return 0;
}

ssize_t led3_read (struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
	int k_data;
	int ret;

	printk(KERN_WARNING "[LED3_DD]read..\n");

	//read from device
	//k_data = inl(IO_ADDR);
	//k_data = readl(IO_ADDR);
	k_data = 55;

	ret = copy_to_user(buf, &k_data, count);
	//put_user(k_data, buf);
	if(ret < 0) return -1;

	printk(KERN_WARNING "[LED_DD]read done(%d)..\n", k_data);

	return count;
}

ssize_t led3_write (struct file *filp, const char __user *buf, size_t count, loff_t *f_pos)
{
	int menu;
	int ret;

	printk(KERN_WARNING "[LED3_DD]write(count:%d)..\n", count);

	ret = copy_from_user(&menu, buf, count);
	if(ret < 0) return -1;	

	switch(menu) {
		case 1 : 	//LED ON
			EXT_LED_On(3);
			break;
		case 2 :	//LED OFF
			EXT_LED_Off(3);
			break;
	}
	
	printk(KERN_WARNING "[LED3_DD]write done(%d)..\n", menu);

	return count;
}

int led3_ioctl(struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg)
{
	printk(KERN_WARNING "[LED3_DD]ioctl..\n");
	switch(cmd) {
		case 3 :
			EXT_LED_Off(8);
			break;
		default :
			EXT_LED_Off(8);
			break;
	}
	
	return 0;
}


struct file_operations led_fops[4] = {
{
    read:       led0_read,
    write:      led0_write,
    open:       led0_open,
    ioctl:		led0_ioctl,
    release:    led0_release
},
{
    read:       led1_read,
    write:      led1_write,
    open:       led1_open,
    ioctl:		led1_ioctl,
    release:    led1_release
},
{
    read:       led2_read,
    write:      led2_write,
    open:       led2_open,
    ioctl:		led2_ioctl,
    release:    led2_release
},
{
    read:       led3_read,
    write:      led3_write,
    open:       led3_open,
    ioctl:		led3_ioctl,
    release:    led3_release
}
};


int led_init(void)
{
	int error;
	int i;

	printk("<1> LED Module id Up....\n");

	if(led_major) {
		led_dev = MKDEV(led_major, led_minor);
		error = register_chrdev_region(led_dev, 4, "led");
	} else {
		error = alloc_chrdev_region(&led_dev, led_minor, 4, "led");
		led_major = MAJOR(led_dev);
	}
	if(error < 0) {
		printk(KERN_WARNING "led:alloc_major error!!\n");
		return error;
	}

	printk("<1> led:alloc_major ok! => major_num:%d\n", led_major);


	for(i=0; i<4; i++) {
		// register_chrdev
		cdev_init(&led_cdev[i], &led_fops[i]);
		led_cdev[i].owner = THIS_MODULE;
		error = cdev_add(&led_cdev[i], led_dev+i, 1);
		if(error < 0) {
			printk(KERN_WARNING "led:register_chrdev error!!\n");
			return error;
		}
	}
	
	EXT_LED_Init();

	printk(KERN_WARNING "LED Module Insert Done!!\n");
	return 0;
}

void led_exit(void)
{
	int i;
	
	printk("<1> LED Module id Down....\n");

	for(i=0; i<4; i++) {
		cdev_del(&led_cdev[i]);
	}
	unregister_chrdev_region(led_dev, 4);

	printk(KERN_WARNING "LED Module Delete Done!!\n");
}

module_init(led_init);
module_exit(led_exit);

MODULE_LICENSE("Dual BSD/GPL");
