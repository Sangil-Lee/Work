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
#include <linux/fs.h> /* register_chrdev, unregister_chrdev */
#include <linux/seq_file.h> /* seq_read, seq_lseek, single_release */

#include "raonts2regmap.h"

/* Standard module information, edit as appropriate */
MODULE_LICENSE("GPL");
MODULE_AUTHOR ("durutronix");
MODULE_DESCRIPTION("Driver for raonts2gtp ip.");
MODULE_ALIAS("custom:raonts2gtp");

#define DEVICE_NAME "raonts2gtp"    // device name
#define DRIVER_NAME DEVICE_NAME
#define raonts2gtp_MAJOR 22

#define RET_OK  0
#define RET_ERR -1

static int    Device_Open = 0;		// device status
static int    major       = raonts2gtp_MAJOR;
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

wait_queue_head_t read_wait_queue;


struct raonts2gtp_local {
  int irq;
  unsigned long mem_start;
  unsigned long mem_end;
  void __iomem *base_addr;
};


// device init and file operations
struct file_operations raonts2gtp_fops = {
  .owner          = THIS_MODULE, //Taba
  .read           = _read,    // read()
  .write          = _write,   // write()
  .open           = _open,    // open()
  .release        = _close,   // close()
  .unlocked_ioctl = _ioctl,     // ioctl()
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
ssize_t _read(struct file *flip, char *buf, size_t length, loff_t *offset)
{
  //unsigned int vBTGR = 0;
  printk("%s _read.\n",DEVICE_NAME);
  
  // if( wait_event_interruptible(read_wait_queue, 0 ))
  // {
  //   printk("wait_event_interruptible Error");
  //   return (CRIT_ERR);
  // }
  
  // vBTGR = ioread32(ioMemBaseAddress + BTGR_OFFSET);
  // printk("%s _read, BTGR_OFFSET, vBTGR = 0x%08x tick\n",DEVICE_NAME ,vBTGR);  
  
  // if(copy_to_user((unsigned int *)buf, &vBTGR, sizeof(unsigned int)))
  // {
  //   printk("Error copy_to_user");
  //   return (-1);
  // } 
  
  return RET_OK;
}

//===============================================================================
//  ioctl
//===============================================================================
long _ioctl (struct file *filp, unsigned int cmd, unsigned long arg)
{

  struct st_regRW reg;
  struct st_ts2gtp_stat gtStatus;
  int rdData;

  switch (cmd) {

    case IOCTL_W :
      //printk("raonts2gtp_IOCTL_W.\n"); 
      if( copy_from_user(&reg, (struct st_regRW *)arg, sizeof(struct st_regRW)))
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
      //printk("raonts2gtp_IOCTL_R.\n"); 

      if( copy_from_user(&reg, (struct st_regRW *)arg, sizeof(struct st_regRW)))
      {
        printk("Error copy_from_user");
        return (RET_ERR);
      }

      //getUpRegConfig(p_card->baseSpiUpdateIp, &reg);
      reg.val = ioread32(ioMemBaseAddress + reg.offset);

      //printk("offset     = %d",  reg.offset);
      //printk("val        = %d",  reg.val);

      if(copy_to_user((struct st_regRW *)arg, &reg, sizeof(struct st_regRW)))
      {
        printk("Error copy_to_user");
        return (RET_ERR);
      } 
      break;


    case IOCTL_ts2gtp_stat :
      //printk("IOCTL_ts2gtp_stat.\n"); 

      rdData = ioread32(ioMemBaseAddress + A_gt_status);

      gtStatus.gtp_soft_reset         = S_gtp_soft_reset          (rdData);
      gtStatus.gt0_cpllfbclklost      = S_gt0_cpllfbclklost       (rdData);
      gtStatus.gt0_cplllock           = S_gt0_cplllock            (rdData);
      gtStatus.gt0_txresetdone        = S_gt0_txresetdone         (rdData);
      gtStatus.gt0_txfsmresetdone     = S_gt0_txfsmresetdone      (rdData);
      gtStatus.gt0_rxresetdone        = S_gt0_rxresetdone         (rdData);
      gtStatus.gt0_rxfsmresetdone     = S_gt0_rxfsmresetdone      (rdData);

      gtStatus.gt0_rxDataError        = S_gt0_rxDataError         (rdData);
      gtStatus.gt0_track_data         = S_gt0_track_data          (rdData);

      gtStatus.gt1_cpllfbclklost      = S_gt1_cpllfbclklost       (rdData);
      gtStatus.gt1_cplllock           = S_gt1_cplllock            (rdData);
      gtStatus.gt1_txresetdone        = S_gt1_txresetdone         (rdData);
      gtStatus.gt1_txfsmresetdone     = S_gt1_txfsmresetdone      (rdData);
      gtStatus.gt1_rxresetdone        = S_gt1_rxresetdone         (rdData);
      gtStatus.gt1_rxfsmresetdone     = S_gt1_rxfsmresetdone      (rdData);

      gtStatus.gt1_rxDataError        = S_gt1_rxDataError         (rdData);
      gtStatus.gt1_track_data         = S_gt1_track_data          (rdData);


      rdData = ioread32(ioMemBaseAddress + A_gt_trackLossCnt); 
      gtStatus.gt0_trackLossCnt        = S_gt0_trackLossCnt       (rdData);
      gtStatus.gt1_trackLossCnt        = S_gt1_trackLossCnt       (rdData);
      
      if(copy_to_user((struct st_ts2gtp_stat *)arg, &gtStatus, sizeof(struct st_ts2gtp_stat)))
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
  
  printk("%s _open\n",DEVICE_NAME);
  
  return RET_OK;
}

//===============================================================================
//  close
//===============================================================================
static int _close(struct inode *inode, struct file *file)
{
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
    cdev_init(&mycdev, &raonts2gtp_fops);
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
  // static unsigned int irqCnt = 0;
  // unsigned int irqTime;

  // irqCnt++;

  // iowrite32(1, ioMemBaseAddress + A_irqClear);
  // iowrite32(0, ioMemBaseAddress + A_irqClear);

  // irqTime = ioread32(ioMemBaseAddress + A_irqClearTime);

  // printk(KERN_ALERT "%s local interrupt %8d_%016lu\n",DEVICE_NAME, irqCnt, irqTime);

  return IRQ_HANDLED;
}

//===============================================================================
//  _probe
//===============================================================================
static int raonts2gtp_probe(struct platform_device *pdev)
{
  struct resource *r_irq; /* Interrupt resources */
  struct resource *r_mem; /* IO mem resources */
  struct device *dev = &pdev->dev;
  struct raonts2gtp_local *lp = NULL;

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
  
  lp = (struct raonts2gtp_local *) kmalloc(sizeof(struct raonts2gtp_local), GFP_KERNEL);
  
  if (!lp) 
  {
    dev_err(dev, "Cound not allocate raonts2gtp device\n");
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
static int raonts2gtp_remove(struct platform_device *pdev)
{
  struct device *dev = &pdev->dev;
  struct raonts2gtp_local *lp = dev_get_drvdata(dev);
  
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
static struct of_device_id raonts2gtp_of_match[] = {
  { .compatible = "xlnx,ts2gtp-1.0", },
  { /* end of list */ },
};
MODULE_DEVICE_TABLE(of, raonts2gtp_of_match);
#else
# define raonts2gtp_of_match
#endif

//===============================================================================
//  platform_driver
//===============================================================================
static struct platform_driver raonts2gtp_driver = {
  .driver = {
    .name = DRIVER_NAME,
    .owner = THIS_MODULE,
    .of_match_table = raonts2gtp_of_match,
  },
  .probe    = raonts2gtp_probe,
  .remove   = raonts2gtp_remove,
};


//===============================================================================
//  mod_init
//===============================================================================
static int __init mod_init(void)
{
  printk(KERN_ALERT "<1>Init module!!!!!\n");
  return platform_driver_register(&raonts2gtp_driver);
}

//===============================================================================
//  mod_exit
//===============================================================================
static void __exit mod_exit(void)
{
  platform_driver_unregister(&raonts2gtp_driver);
  printk(KERN_ALERT "Exit module.\n");
}

//===============================================================================
//  module_init, module_exit
//===============================================================================
module_init(mod_init);
module_exit(mod_exit);

