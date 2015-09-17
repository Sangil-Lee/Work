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


#include <linux/timer.h>
#include <linux/delay.h>		//mdelay, udelay


#include <linux/interrupt.h>   	//request_irq

#include <mach/irqs.h>		//IRQ_TIMER0_VIC
//#include <plat/irqs.h>		//IRQ_TIMER1
#include <plat/map-base.h>	//S3C_VA_TIMER
#include <plat/regs-timer.h>	//S3C2410_TCON

#include <linux/io.h>		//__raw_readl

#include <linux/wait.h>
#include <linux/.......... >

#if 0
#include <mach/regs-irq.h>	//S5P_VIC0REG()
#include <asm/hardware/vic.h>	//VIC_INT_ENABLE
#endif


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


int led_major=0, led_minor=0;
dev_t led_dev;
struct cdev led_cdev;

struct timer_list my_timer;

volatile unsigned int flag_timer0Int=0;

DECLARE_WAIT_QUEUE_HEAD(wq_timer);

void func_workqueue(struct work_struct *dummy);

........... (my_wq, .........);

/*
#define	TIMER0_ON	S3C2410_TCFG0 = (0<<8)|(0xFF); S3C2410_TCFG1 = 4; S3C2410_TCON = (0<<4)|(1<<3)|(0<<2)|(0<<1)|(0); S3C2410_TCMPB(0) = 0; S3C2410_TCNTB(0) = timerClkVal; S3C2410_TCON |= (1<<1)|(0); S3C2410_TCON &= ~(1<<1); S3C2410_TCON |= 1
#define	TIMER0_OFF	S3C2410_TCON &= ~(1<<0)
*/
#define	SEC_CLK		(16*1000)

int timerPeriod=1, timerClkVal=3*SEC_CLK;

irqreturn_t timer0_int(int irq, void *dev_id);
void TIMER0_ON(void)
{
	unsigned long tcon;

	__raw_writel((0<<8)|(0xFF), S3C2410_TCFG0); 
	__raw_writel(4, S3C2410_TCFG1); 
	__raw_writel((0<<4)|(1<<3)|(0<<2)|(0<<1)|(0), S3C2410_TCON); 
	__raw_writel(0, S3C2410_TCNTB(0)); 
	__raw_writel(0, S3C2410_TCMPB(0)); 


#if 0
	__raw_writel(0, S5P_VIC0REG(VIC_PL192_VECT_ADDR));
	__raw_writel(0, S5P_VIC0REG(VIC_INT_SELECT));
	__raw_writel((unsigned int)timer0_int, S5P_VIC0REG(VIC_VECT_ADDR0+54));
	tcon = __raw_readl(S5P_VIC0REG(VIC_INT_ENABLE));
	tcon |= (1<<21);	//Timer0 Int. Enable
	__raw_writel(tcon, S5P_VIC0REG(VIC_INT_ENABLE));
#endif

	tcon = __raw_readl(S3C64XX_TINT_CSTAT); 
	tcon |= (1<<0);		//Timer0 Int. Enable on Timer Controller
	__raw_writel(tcon, S3C64XX_TINT_CSTAT); 


	__raw_writel(timerClkVal, S3C2410_TCNTB(0)); 
	tcon = __raw_readl(S3C2410_TCON); 
	tcon |= (1<<1)|(0); 
	__raw_writel(tcon, S3C2410_TCON); 
	tcon = __raw_readl(S3C2410_TCON); 
	tcon &= ~(1<<1); 
	__raw_writel(tcon, S3C2410_TCON); 
	tcon = __raw_readl(S3C2410_TCON); 
	tcon |= 1;
	__raw_writel(tcon, S3C2410_TCON); 

}
void TIMER0_OFF(void)
{
	unsigned long tcon;
	tcon = __raw_readl(S3C2410_TCON); 
	tcon &= ~(1<<0);
	__raw_writel(tcon, S3C2410_TCON); 
}

void func_workqueue(struct work_struct *data)
{
	int i;

	printk("*****WorkQueue_Task Start!!\n");
	//for(i=1; i<(int)data; i++) {
	for(i=1; i<10; i++) {
		printk("*****WorkQueue_Task(%d)!!\n", i);
	}
	............ (&wq_timer);
	printk("*****WorkQueue_Task End!!\n");
}

irqreturn_t timer0_int(int irq, void *dev_id)
{
	unsigned long tcon;

	printk("[LED_DD]Timer0 Int(irq:%d)!!!!\n", irq);

	tcon = __raw_readl(S3C64XX_TINT_CSTAT); 
	tcon |= (1<<5);		//Timer0 Pending Clear on Timer Controller
	__raw_writel(tcon, S3C64XX_TINT_CSTAT); 
	
	TIMER0_OFF();
	flag_timer0Int=1;
	
	
	.............
		
	printk("[LED_DD]Timer0 Int. End!!!\n");
	
	return IRQ_HANDLED;
}


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


int led_open (struct inode *inode, struct file *filp)
{
	printk(KERN_WARNING "[LED_DD]open..\n");
	
	return 0;
}

int led_release (struct inode *inode, struct file *filp)
{
	printk(KERN_WARNING "[LED_DD]release..\n");
	return 0;
}

ssize_t led_read (struct file *filp, char *buf, size_t count, loff_t *f_pos)
{
	int ret;
	int led_stat;

	printk(KERN_WARNING "[LED_DD]read..\n");

	led_stat = 0x55;
	ret = copy_to_user(buf, &led_stat, count);
	if(ret<0) return -1;

	return count;
}

ssize_t led_write (struct file *filp, const char *buf, size_t count, loff_t *f_pos)
{
	int ret, k_data;
	int led_func, led_no;

	printk(KERN_WARNING "[LED_DD]write..\n");

	TIMER0_ON();
	//while(!flag_timer0Int);
	
	while(!flag_timer0Int) {
		if(filp->f_flags & O_NONBLOCK) return -EAGAIN;
		printk(KERN_WARNING "[LED_DD]Waiting Timer0 Int..\n");
		interruptible_sleep_on(&wq_timer);
		printk(KERN_WARNING "[LED_DD]After Timer0 Int..\n");
	}

	ret = copy_from_user(&k_data, buf, count);
	if(ret<0) return -1;

	led_func = k_data >> 8;
	led_no = k_data & 0xFF;

	switch(led_func) {
	case 1:
		EXT_LED_On(led_no);
		break;
	case 2:
		EXT_LED_Off(led_no);
		break;
	}

	flag_timer0Int = 0;
	
	return count;
}

int led_ioctl(struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg)
{
	int led_func=1, led_no=0;

	printk(KERN_WARNING "[LED_DD]ioctl(cmd:%x)..\n", cmd);

	led_func = cmd>>8;
	led_no = cmd & 0xFF;
	printk(KERN_WARNING "[LED_DD]ioctl(led_func:%d, led_no:%d)..\n", led_func, led_no);
	switch(led_func) {
	case 1:
		EXT_LED_On(led_no);
		break;
	case 2:
		EXT_LED_Off(led_no);
		break;
	case 5 :
		timerPeriod = arg;
		timerClkVal = timerPeriod*SEC_CLK;
		break;
	}

	return 0;
}

struct file_operations led_fops = {
    read:       led_read,
    write:      led_write,
    open:       led_open,
    ioctl:		led_ioctl,
    release:    led_release
};

int led_register_cdev(void)
{
	int error;

	if(led_major) {
		led_dev = MKDEV(led_major, led_minor);
		error = register_chrdev_region(led_dev, 1, "led");
	} else {
		error = alloc_chrdev_region(&led_dev, led_minor, 1, "led");
		led_major = MAJOR(led_dev);
	}
	if(error < 0) {
		printk(KERN_WARNING "led:alloc_major error!!\n");
		return error;
	}

	printk("<1> led:alloc_major ok! => major_num:%d\n", led_major);


	// register_chrdev
	cdev_init(&led_cdev, &led_fops);
	led_cdev.owner = THIS_MODULE;
	error = cdev_add(&led_cdev, led_dev, 1);
	if(error < 0) {
		printk(KERN_WARNING "led:register_chrdev error!!\n");
		return error;
	}

	//EXT_LED_Init();

	return 0;
}


void led_c_dev_release(struct device *device)
{
	printk("led_c_dev_release()..\n");
}
void ldd_b_dev_release(struct device *device)
{
	printk("ldd_b_dev_release()..\n");
}

int led_driver_probe(struct device *dev)
{
	printk("led_driver_probe()..\n");
	EXT_LED_Init();
	return 0;
}
int led_driver_remove(struct device *dev)
{
	printk("led_driver_remove()..\n");
	return 0;
}

struct bus_type ldd_bus_type = {
	.name		= "ldd",
};

struct device ldd_b_dev = {
	.init_name	= "ldd0",
	.release	= ldd_b_dev_release,
};

struct device_driver led_driver = {
	.name		= "led_driver",
	.owner		= THIS_MODULE,
	.bus		= &ldd_bus_type,
	.probe		= led_driver_probe,
	.remove		= led_driver_remove,
};

struct class *led_class;

struct device led_c_dev = {
	.init_name	= "led0",
	.driver		= &led_driver,
	.parent		= &ldd_b_dev,
	.release	= led_c_dev_release,
};


void led_timer(unsigned long data)
{
	static int cnt=0;

	printk("<4> LED Kernel Timer Function....\n");
	EXT_LED_Off(8);
	EXT_LED_On(data);

	my_timer.expires = get_jiffies_64() + HZ*2;	//2Sec Timer
	my_timer.data = cnt++;	//led no
	if(cnt==8) cnt=0;
	add_timer(&my_timer);
}

int led_init(void)
{
	int result;

	printk("<1> LED Module id Up....\n");

	result = led_register_cdev();
	if(result < 0) {
		printk(KERN_WARNING "Module(led) Register Fail!!\n");
		cdev_del(&led_cdev);
		unregister_chrdev_region(led_dev, 1);
		return result;
	}
	printk(KERN_WARNING "Module(led) Register OK!!\n");


	result = bus_register(&ldd_bus_type);
	if(result < 0) {
		printk(KERN_WARNING "Bus Register Fail(%d)!!\n", result);
		cdev_del(&led_cdev);
		unregister_chrdev_region(led_dev, 1);
		return result;
	}
	printk(KERN_WARNING "Bus Register OK!!\n");

	ldd_b_dev.bus = &ldd_bus_type;
	result = device_register(&ldd_b_dev);
	if(result < 0) {
		printk("Bus_Device(ldd_b_dev):Register Fail(%d)!!\n", result);
		cdev_del(&led_cdev);
		unregister_chrdev_region(led_dev, 1);
		bus_unregister(&ldd_bus_type);
		return result;
	}

	result = driver_register(&led_driver);
	if(result < 0) {
		printk("Driver(led_driver):Register Fail(%d)!!\n", result);
		cdev_del(&led_cdev);
		unregister_chrdev_region(led_dev, 1);
		device_unregister(&ldd_b_dev);
		bus_unregister(&ldd_bus_type);
		return result;
	}
	printk("Driver(led_driver):Register OK!!\n");

	led_class = class_create(THIS_MODULE, "led");
	if(IS_ERR(led_class)) {
		printk("Class Create Fail!!\n");
		cdev_del(&led_cdev);
		unregister_chrdev_region(led_dev, 1);
		device_unregister(&ldd_b_dev);
		driver_unregister(&led_driver);
		bus_unregister(&ldd_bus_type);
		return result;
	}
	printk("Class Create OK!!\n");


	led_c_dev.class = led_class;
	led_c_dev.devt = led_dev;
	result = device_register(&led_c_dev);
	if(result < 0) {
		printk("Class_Device(led_c_dev):Register Fail(%d)!!\n", result);
		cdev_del(&led_cdev);
		unregister_chrdev_region(led_dev, 1);
		device_unregister(&ldd_b_dev);
		driver_unregister(&led_driver);
		bus_unregister(&ldd_bus_type);
		class_destroy(led_class);
		return result;
	}
	printk("Class_Device(led_c_dev):Register OK!!\n");

	result = request_irq(IRQ_TIMER0_VIC, timer0_int, 
				IRQF_TIMER|IRQF_DISABLED, "LED", NULL);
	if(result != 0) {
	   printk(KERN_WARNING "000_request_irq() Error(%d)!!\n", result);
	}

	printk(KERN_WARNING "LED Module Insert Done!!\n");
	return 0;
}

void led_exit(void)
{
	printk("<1> LED Module id Down....\n");

	free_irq(IRQ_TIMER0_VIC, NULL);

	cdev_del(&led_cdev);
	unregister_chrdev_region(led_dev, 1);

	device_unregister(&led_c_dev);
	device_unregister(&ldd_b_dev);
	driver_unregister(&led_driver);

	class_destroy(led_class);
	bus_unregister(&ldd_bus_type);

	printk(KERN_WARNING "LED Module Delete Done!!\n");
}

module_init(led_init);
module_exit(led_exit);

MODULE_LICENSE("Dual BSD/GPL");
