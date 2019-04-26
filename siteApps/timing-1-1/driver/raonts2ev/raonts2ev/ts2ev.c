/*

* Copyright (C) 2013 - 2016 Xilinx, Inc
*
*   This program is free software; you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation; either version 2 of the License, or
*   (at your option) any later version.

*   This program is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License along
*   with this program. If not, see <http://www.gnu.org/licenses/>.

*/
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/io.h>
#include <linux/interrupt.h>

#include <linux/of_address.h>
#include <linux/of_device.h>
#include <linux/of_platform.h>

#include <linux/irq.h>
#include <asm/io.h>
#include <linux/fs.h>		//required for fops
#include <linux/uaccess.h>	//required for 'cpoy_from_user' and 'copy_to_user'
#include <linux/signal.h>	//required for kernel-to-userspace signals

#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/seq_file.h> /* seq_read, seq_lseek, single_release */
#include <linux/wait.h>

/* Standard module information, edit as appropriate */
MODULE_LICENSE("GPL");
MODULE_AUTHOR ("durutronix");
MODULE_DESCRIPTION("Driver for raonts2ev ip.");
MODULE_ALIAS("custom:raonts2ev");

#define DEVICE_NAME "raonts2ev"    // device name
#define DRIVER_NAME DEVICE_NAME
#define DEVICE_MAJOR 22

#define RET_OK  0
#define RET_ERR -1

#define A_ipSys_intrReg           0x010 //
#define A_ipSys_setting           0x020 //
#define A_ipSys_config            0x024 //
#define A_ipSys_intrMask          0x028 //


static int    Device_Open = 0;		// device status
static int    major       = DEVICE_MAJOR;
static struct cdev  mycdev;
static struct class *myclass = NULL;
void __iomem *ioMemBaseAddress;

/* Simple example of how to receive command line parameters to your module.
   Delete if you don't need them */
unsigned myint = 0xdeadbeef;
char *mystr = "default";

module_param(myint, int, S_IRUGO);
module_param(mystr, charp, S_IRUGO);

ssize_t _write(struct file *flip, const char *buf, size_t length, loff_t *offset);
ssize_t _read(struct file *flip, char *buf, size_t length, loff_t *offset);
long _ioctl (struct file *filp, unsigned int cmd, unsigned long arg);
static int _open(struct inode *inode, struct file *file);
static int _close(struct inode *inode, struct file *file);
static int _fasync(int fd, struct file *filp, int on);

wait_queue_head_t read_wait_queue;


struct local {
  int irq;
  unsigned long mem_start;
  unsigned long mem_end;
  void __iomem *base_addr;
};


// device init and file operations
struct file_operations fops = {
  .owner          = THIS_MODULE, //Taba
  .read           = _read,    // read()
  .write          = _write,   // write()
  .open           = _open,    // open()
  .release        = _close,   // close()
  .unlocked_ioctl = _ioctl,     // ioctl()
 	.fasync         = _fasync,
};

static struct fasync_struct *_async_queue;


//===============================================================================
//  
//===============================================================================
#define MAGIC_NUM 0xDB
#define IOCTL_R           _IOWR (MAGIC_NUM, 1, struct s_regRW)
#define IOCTL_W           _IOWR (MAGIC_NUM, 2, struct s_regRW)

struct s_regRW{
    unsigned int offset;     
    unsigned int val;
};


//===============================================================================
//  write
//===============================================================================
ssize_t _write(struct file *flip, const char *buf, size_t length, loff_t *offset)
{
  printk("%s _write.\n",DEVICE_NAME);
  return RET_OK;
}

//===============================================================================
//  read
//===============================================================================
static int flag = 0;

ssize_t _read(struct file *flip, char *buf, size_t length, loff_t *offset)
{
  unsigned int rData = 0;
//  printk("[k]_read\r\n");
  if( wait_event_interruptible(read_wait_queue, flag != 0 ))
  {
    printk("[err-k] wait_event_interruptible\r\n");
    return (RET_ERR);
  }

  rData = ioread32(ioMemBaseAddress + A_ipSys_intrReg);  // read interrupt register value

  if(copy_to_user((unsigned int *)buf, &rData, sizeof(unsigned int)))
  {
    printk("Error copy_to_user");
    return (RET_ERR);
  } 

  // printk("[k]_read 0x%08x\r\n", rData);
  flag = 0;

  return sizeof(unsigned int);
}

  // unsigned int day = 0;
  // unsigned int hour = 0;
  // unsigned int min = 0;
  // unsigned int sec = 0;
  // unsigned int ms = 0;

  // if( S_intr_1sec(rData) )
  // {
  //   rData = ioread32(ioMemBaseAddress + A_evrW_Time);
  //   hour  = S_evrW_TimeHour(rData);
  //   min   = S_evrW_TimeMin (rData);
  //   sec   = S_evrW_TimeSec (rData);
  //   ms    = S_evrW_TimeMs  (rData);
  //   day   = ioread32(ioMemBaseAddress + A_evrW_Day);
  //   //printk("time   : %d %d.%d.%d:%d\r\n",day,hour,min,sec,ms);

  //   getrawmonotonic(&myTime);

  //   // /clock_gettime(CLOCK_REALTIME, &myTime);
  //   printk("myTime : %ld.%ld\r\n",myTime.tv_sec,myTime.tv_nsec);
  // }
  
  
  // iowrite32(1, ioMemBaseAddress + A_intrClear);
  // iowrite32(0, ioMemBaseAddress + A_intrClear);
  // iowrite32(1, ioMemBaseAddress + A_intrEnable);



//===============================================================================
//  ioctl
//===============================================================================
long _ioctl (struct file *filp, unsigned int cmd, unsigned long arg)
{

  struct s_regRW reg;

  int rdData;

  switch (cmd) {

    case IOCTL_W :
      //printk("IOCTL_W.\n"); 

      if( copy_from_user(&reg, (struct s_regRW *)arg, sizeof(struct s_regRW)))
      {
        printk("Error copy_from_user");
        return (RET_ERR);
      }

      //printk("offset     = %d",  reg.offset);
      //printk("val        = %d",  reg.val);
      // write here
      iowrite32(reg.val, ioMemBaseAddress + reg.offset );
      
      break;

    case IOCTL_R :
      //printk("IOCTL_R.\n"); 

      if( copy_from_user(&reg, (struct s_regRW *)arg, sizeof(struct s_regRW)))
      {
        printk("Error copy_from_user");
        return (RET_ERR);
      }

      //getUpRegConfig(p_card->baseSpiUpdateIp, &reg);
      reg.val = ioread32(ioMemBaseAddress + reg.offset);

      //printk("offset     = %d",  reg.offset);
      //printk("val        = %d",  reg.val);

      if(copy_to_user((struct s_regRW *)arg, &reg, sizeof(struct s_regRW)))
      {
        printk("Error copy_to_user");
        return (RET_ERR);
      } 
      break;

    default:
      break;
  }

  return (RET_OK);
}

//===============================================================================
//  open
//===============================================================================
static int _open(struct inode *inode, struct file *file)
{
//  u32 val = 0;
//  u32 i = 0;
  
  if (Device_Open)          // if the device is allready open,
    return -EBUSY;          // return with an error

  Device_Open++;            // 'open' device

  // iowrite32(M_TxUserSendEn | (M_TxUserFrameSize & C_TxUserFrameSize), ioMemBaseAddress + A_TxUserCtrl );
  // iowrite32(0, ioMemBaseAddress + A_TxUserData);
  
  printk("%s _open\n",DEVICE_NAME);
  
  return RET_OK;
}

//===============================================================================
//  close
//===============================================================================
static int _close(struct inode *inode, struct file *file)
{
  iowrite32(0, ioMemBaseAddress + A_ipSys_config);             // ipSys_intrEnable -> disable

	if (file->f_flags & FASYNC) {
		_fasync (-1, file, 0);
	}

  if (Device_Open == 0)
    return RET_ERR;          // return with an error
  
  Device_Open--;          // 'close' device
  printk("%s _close\n",DEVICE_NAME);

  return RET_OK;
}
















//===============================================================================
//  cleanup
//===============================================================================
static void cleanup(int device_created)
{
    if (device_created) {
        device_destroy(myclass, major);
        cdev_del(&mycdev);
    }
    if (myclass)
        class_destroy(myclass);
    if (major != -1)
        unregister_chrdev_region(major, 1);
}


//===============================================================================
//  sub_probe
//===============================================================================
static void _sub_probe(void __iomem *base)
{
    int device_created = 0;

    printk(KERN_ERR "%s _sub_probe... \n", DEVICE_NAME);
  
    ioMemBaseAddress = base;

    /* cat /proc/devices */
    if (alloc_chrdev_region(&major, 0, 1, DEVICE_NAME "_proc") < 0)
        goto cdev_error;
    /* ls /sys/class */
    if ((myclass = class_create(THIS_MODULE, DEVICE_NAME "_sys")) == NULL)
        goto cdev_error;
    /* ls /dev/ */
    if (device_create(myclass, NULL, major, NULL, DEVICE_NAME "_dev") == NULL)
        goto cdev_error;
    device_created = 1;
    cdev_init(&mycdev, &fops);
    if (cdev_add(&mycdev, major, 1) == -1)
        goto cdev_error;
    return;
cdev_error:
    cleanup(device_created);
    return;
}

//===============================================================================
//  sub_remove
//===============================================================================
static int _sub_remove(void)
{
  cleanup(1);
  return RET_OK;
}




//===============================================================================
//  irq
//===============================================================================
static irqreturn_t _irq(int irq, void *lp)
{
#if 0
  // Clear Interrupt Enable
  iowrite32(0, ioMemBaseAddress + A_intrEnable);
	kill_fasync(&_async_queue, SIGIO, POLL_IN);
#else
  static unsigned int irqTime;

  iowrite32(0, ioMemBaseAddress + A_ipSys_config);             // ipSys_intrEnable -> disable
  //printk(KERN_ALERT "%s local interrupt %d.\n",DEVICE_NAME, irqTime++);
  flag = 1;
  wake_up_interruptible(&read_wait_queue);
#endif
	return IRQ_HANDLED;
}






//===============================================================================
//  _fasync
//===============================================================================

static int _fasync(int fd, struct file *filp, int mode)
{
	int tmp;
	tmp = fasync_helper(fd, filp, mode, &_async_queue);
	if (fd != -1)
		kill_fasync(&_async_queue, SIGIO, POLL_IN);
	return(tmp);
}

//===============================================================================
//  _probe
//===============================================================================
static int probe(struct platform_device *pdev)
{
  struct resource *r_irq; /* Interrupt resources */
  struct resource *r_mem; /* IO mem resources */
  struct device *dev = &pdev->dev;
  struct local *lp = NULL;

  int rc = 0;
  dev_info(dev, "Device Tree Probing\n");
  
  init_waitqueue_head(&read_wait_queue);  
  
  /* Get iospace for the device */
  r_mem = platform_get_resource(pdev, IORESOURCE_MEM, 0);
  
  if (!r_mem) 
  {
    dev_err(dev, "invalid address\n");
    return -ENODEV;
  }
  
  lp = (struct local *) kmalloc(sizeof(struct local), GFP_KERNEL);
  
  if (!lp) 
  {
    dev_err(dev, "Cound not allocate raonts2ev device\n");
    return -ENOMEM;
  }

  dev_set_drvdata(dev, lp);
  lp->mem_start = r_mem->start;
  lp->mem_end = r_mem->end;
  
  dev_info(dev, "axi_mem_start = %lu.\n", lp->mem_start);
  dev_info(dev, "axi_mem_end   = %lu.\n", lp->mem_end);

  if (!request_mem_region(lp->mem_start,
        lp->mem_end - lp->mem_start + 1,
        DRIVER_NAME)) 
  {
    dev_err(dev, "Couldn't lock memory region at %p\n",
      (void *)lp->mem_start);
    rc = -EBUSY;
    goto error1;
  }

  lp->base_addr = ioremap(lp->mem_start, lp->mem_end - lp->mem_start + 1);
  
  if (!lp->base_addr) 
  {
    dev_err(dev, "%s: Could not allocate iomem\n",DEVICE_NAME);
    rc = -EIO;
    goto error2;
  }

  /* Get IRQ for the device */
  r_irq = platform_get_resource(pdev, IORESOURCE_IRQ, 0);
  
  if (!r_irq) 
  {
    dev_info(dev, "no IRQ found\n");
    dev_info(dev, "%s at 0x%08x mapped to 0x%08x\n",DEVICE_NAME,
      (unsigned int __force)lp->mem_start,
      (unsigned int __force)lp->base_addr);
    return 0;
  }

  lp->irq = r_irq->start;
  
  rc = request_irq(lp->irq, &_irq, 0, DRIVER_NAME, lp);
  
  if (rc) 
  {
    dev_err(dev, "testmodule: Could not allocate interrupt %d.\n", lp->irq);
    goto error3;
  }

  dev_info(dev,"%s at 0x%08x mapped to 0x%08x, irq=%d\n",DEVICE_NAME,
    (unsigned int __force)lp->mem_start,
    (unsigned int __force)lp->base_addr,
    lp->irq);

  _sub_probe(lp->base_addr);

  return 0;
error3:
  free_irq(lp->irq, lp);
error2:
  release_mem_region(lp->mem_start, lp->mem_end - lp->mem_start + 1);
error1:
  kfree(lp);
  dev_set_drvdata(dev, NULL);
  return rc;
}


//===============================================================================
//  _remove
//===============================================================================
static int remove(struct platform_device *pdev)
{
  struct device *dev = &pdev->dev;
  struct local *lp = dev_get_drvdata(dev);
  
  printk("%s _remove\n", DEVICE_NAME);
  _sub_remove();
  
  free_irq(lp->irq, lp);
  release_mem_region(lp->mem_start, lp->mem_end - lp->mem_start + 1);
  kfree(lp);
  dev_set_drvdata(dev, NULL);
  return RET_OK;
}

//===============================================================================
//  of_device_id
//===============================================================================
#ifdef CONFIG_OF
static struct of_device_id of_match[] = {
  { .compatible = "xlnx,ts2ev-1.0", },
  { /* end of list */ },
};
MODULE_DEVICE_TABLE(of, of_match);
#else
# define of_match
#endif

//===============================================================================
//  platform_driver
//===============================================================================
static struct platform_driver driver = {
  .driver = {
    .name = DRIVER_NAME,
    .owner = THIS_MODULE,
    .of_match_table = of_match,
  },
  .probe    = probe,
  .remove   = remove,
};


//===============================================================================
//  mod_init
//===============================================================================
static int __init mod_init(void)
{
  printk(KERN_ALERT "Init module 1v21\n");
  return platform_driver_register(&driver);
}

//===============================================================================
//  mod_exit
//===============================================================================
static void __exit mod_exit(void)
{
  platform_driver_unregister(&driver);
  printk(KERN_ALERT "Exit module.\n");
}

//===============================================================================
//  module_init, module_exit
//===============================================================================
module_init(mod_init);
module_exit(mod_exit);
