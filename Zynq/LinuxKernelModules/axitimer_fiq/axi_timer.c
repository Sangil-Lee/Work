#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <asm/io.h>
#include <linux/fs.h>		//required for fops
#include <linux/uaccess.h>	//required for 'cpoy_from_user' and 'copy_to_user'
#include <linux/signal.h>	//required for kernel-to-userspace signals
#include <linux/init.h>
#include <linux/module.h>
#include <linux/err.h>
#include <linux/percpu.h>
#include <linux/mm.h>
#include <linux/platform_device.h>
#include <linux/kernel.h>
#include <linux/smp.h>
#include <linux/slab.h>
#include <linux/io.h>
#include <asm/page.h>
#include <asm/fiq.h>
#include "fiq_glue.h"

MODULE_LICENSE("GPL");


/*-----------------------------*/
/* Macros and Definitions      */
/*-----------------------------*/

/*
 * FIQ Address related macros
 */
#define PHYSICAL_VECTOR_PAGE		0xFFFF0000
#define FIQ_VECTOR_OFFSET		0x1C
#define FIQ_ID				28
#define FIQ_HANDLER_MARGIN_OFFSET	32

#define MAX_FIQ_SIZE			(2*1024)


#define IRQ_NUM		28		// interrupt line
#define TIMER_BASE	0x42800000

#define TIMER_SIZE	0x0000FFFF
#define TCSR0		0x00000000
#define TLR0		0x00000004
#define TCR0		0x00000008
#define T0INT		0x00000100

#define TIMER_TCSR0	0x42800000	// address of timer 0's control and status register
#define TIMER_TLR0	0x42800004	// address of timer 0's load register
#define TIMER_TCR0	0x42800008	// address of timer 0's counter register
//#define LOAD_VAL	0xE2329AFF	// address load value (500000000 cycles) ~5 sec
#define LOAD_VAL	0xF0000000	
unsigned long *pTIMER_TCSR0;	// pointer to timer 0 control and status register
unsigned long *pTIMER_TLR0;		// pointer to timer 0 load register
unsigned long *pTIMER_TCR0;		// pointer to timer 0 counter register

#define DEVICE_NAME "axiFiqTest"		// device name
#define SYSCALL_MAJOR 22		// device major number
#define BUF_LEN 80			// max buffer length
#define SUCCESS 0			// success return value

static int Device_Open = 0;		// device status

static char msg[BUF_LEN];		// the msg the device will give when asked
static char *msg_Ptr;
static int intcount;
struct fiq_handler fiq;

extern unsigned char axitimer_fiq_handler_asm, axitimer_fiq_handler_end;

#ifdef __ASSEMBLY__
#define __REG_NR(x)     r##x
#else
#define __REG_NR(x)     (x)
#endif

#define fiq_raxitmrcnt	__REG_NR(8)
#define fiq_raxitmrcsr	__REG_NR(9)
volatile unsigned long gTimervalue;

ssize_t syscall_write(struct file *flip, const char *buf, size_t length, loff_t *offset)
{
	printk("syscall_write.\n");				// debug: procedure call message
	if (copy_from_user(msg, buf, length) != 0)		// read buffer from user space
		return -EFAULT;					// return error if it failed
	printk("Received: %s \n",msg);				// debug: what string is received

	if (strcmp(msg,"1") == 0)				// enable timer command
	{
		printk("Driver enables timer.\n");		// print timer status message
		iowrite32(LOAD_VAL,pTIMER_TLR0);		// prepare timer cycle 
		iowrite32(0x000000B0,pTIMER_TCSR0);		// load the load-register
		iowrite32(0x000000D0,pTIMER_TCSR0);		// Generate mode,upcounter, reload generate value
								// no load, enable IRQ, enable Timer
		return SUCCESS;					// return 0 to application
	} 
	else if (strcmp(msg,"0") == 0)				// disable timer command
	{
		printk("Driver disables timer.\n");		// print timer status message
		iowrite32(0x00000050,pTIMER_TCSR0);		// Generate mode, upcounter, reload generate value
								// no load, enable IRQ, disable Timer
		return SUCCESS;					// return 0 to application
	} 
	else 
	{
		printk("Driver received wrong value.\n");	// Print error message
		return -EFAULT;					// unknown value received
	}
}
unsigned long gTemp = 0, gTimerVal1,gTimerval2;

ssize_t syscall_read(struct file *flip, char *buf, size_t length, loff_t *offset)
{
	unsigned long timerval,temp;

	temp = ioread32(pTIMER_TCSR0);// clear timer IRQ

	if (((temp & (0x80)) == 0) || (temp & 0x100))
	{
		timerval = ioread32(pTIMER_TCR0);
		timerval = timerval - LOAD_VAL;
		printk(KERN_ALERT "CPU0: FIQ_AXI_Timer_Count:%d$\n",(timerval));
		iowrite32(LOAD_VAL, pTIMER_TLR0);			// place load value in load register
		iowrite32(0x000000B0, pTIMER_TCSR0);			// load TLR
		iowrite32(0x000000D0, pTIMER_TCSR0);			// Generate mode,downcounter,reload generate
		return 0;
	}
	else
	{
		return 1;
	}

}

// open routine (called when a device opens /dev/syscall)
static int syscall_open(struct inode *inode, struct file *file)
{
	if (Device_Open)					// if the device is allready open,
		return -EBUSY;					// return with an error

	Device_Open++;						// 'open' device
	sprintf(msg,"You tried to open the syscall module.\n");	// print open message
	msg_Ptr = msg;						
	try_module_get(THIS_MODULE);
	return 0;
}

// close routine (called whne a device closes /dev/syscall)
static int syscall_close(struct inode *inode, struct file *file)
{
	Device_Open--;						// 'close' device

	module_put(THIS_MODULE);
	return 0;
}

// device init and file operations
struct file_operations syscall_fops = {
	.read = syscall_read,		// read()
	.write = syscall_write,		// write()
	.open = syscall_open,		// open()
	.release = syscall_close,	// close()
};

int end_lable;
static void testFunc(void);

void axitimer_fiq_handler(void)
{
	unsigned long temp;
	unsigned long timervalue;
	temp  = ioread32(pTIMER_TCSR0);// clear timer IRQ
	temp  &= ~(0x80);
	iowrite32(temp, pTIMER_TCSR0);
}
static void testFunc(void)
{

}

void debug_print(void)
{
	printk(KERN_ERR "In FIQ Handler\n");
}

// init module      
static int __init mod_init(void)  
{
	struct pt_regs regs;
	unsigned long temp;
	int ret;
	void *vector_addr;
	struct fiq_glue_handler handle;
	printk(KERN_ERR "Init AXI Timer syscall module. \n");
    handle.fiq = axitimer_fiq_handler;
    ret = fiq_glue_register_handler(&handle);
    if(ret != 0)
    {
        printk(KERN_ERR "Unable to register handler \n");
    }
    printk(KERN_ERR "  <<<< register FIQ handler successful >>>> \n");

	pTIMER_TCSR0 = ioremap_nocache(TIMER_TCSR0,0x4);	// map timer 0 control and status register
	pTIMER_TLR0 = ioremap_nocache(TIMER_TLR0,0x4);		// map timer 0 load register
	pTIMER_TCR0 = ioremap_nocache(TIMER_TCR0,0x4);		// map timer 0 count register
	iowrite32(LOAD_VAL, pTIMER_TLR0);			// place load value in load register
	temp = ioread32(pTIMER_TLR0);				// debug: read load value to check
	printk("Load value: %lu.\n",temp);			// debug: print the read load value
	iowrite32(0x000000B0, pTIMER_TCSR0);			// load TLR
	iowrite32(0x000000D0, pTIMER_TCSR0);			// Generate mode,downcounter,reload generate
								// value,no load,enable IRQ,enable Timer
	intcount = 0;						// set interrupt count to 0, driver will unload on 100 interrupts
	// manual node creation
	if (register_chrdev(SYSCALL_MAJOR, DEVICE_NAME, &syscall_fops))
		printk("Error: cannot register to major device 22.\n");

	
	printk("Type: mknod /dev/%s c %d 0\n",DEVICE_NAME, SYSCALL_MAJOR);
	printk("And remove it after unloading the module.\n");
//__asm__("sev");
	return SUCCESS;
} 

// exit module
static void __exit mod_exit(void)  		
{
	iounmap(pTIMER_TCSR0);				// unregister timer hardware
	iounmap(pTIMER_TLR0);
	iounmap(pTIMER_TCR0);
	free_irq(IRQ_NUM, NULL);			// unregister timer interrupt
//	release_fiq(&fiq);
	unregister_chrdev(SYSCALL_MAJOR, "axiFiqTest");	// unregister device
	printk(KERN_ERR "Exit syscall Module. \n");	// print unload message
}

module_init(mod_init);
module_exit(mod_exit);

MODULE_AUTHOR ("Xilinx");
MODULE_DESCRIPTION("Testdirver for the Xilinx AXI Timer IP core & Interrupt Latency");
MODULE_LICENSE("GPL v2");
MODULE_ALIAS("custom:axiFiqTest");
