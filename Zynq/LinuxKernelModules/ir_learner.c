/*
 * IR learner driver
 *
 * This code is released under the GPL version 2.
 */

#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/of_gpio.h>
#include <linux/interrupt.h>
#include <linux/clk.h>
#include <linux/irq.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/semaphore.h>
#include <linux/miscdevice.h>
#include <linux/poll.h>
#include <linux/fs.h>
#include <linux/atmel_tc.h>

#include <asm/fiq.h>
#include <asm/io.h>
#include <asm/uaccess.h>
#include "../../arch/arm/mach-at91/at91_aic.h"
#include "../../arch/arm/mach-at91/include/mach/at91_pio.h"
#include "ir_learner.h"

#define DETECT_TOUT_MS	15
#define TXT_BUF_SIZE	(MAX_BUF * 10)

extern unsigned char ir_learner_fiq_start, ir_learner_fiq_end;

extern void __iomem *at91_gpio_base[];
#define at91_gpio_b_read(field) \
	__raw_readl(at91_gpio_base[1] + field)
#define at91_gpio_b_write(field, value) \
	__raw_writel(value, at91_gpio_base[1] + field)

static void __iomem *at91_local_tc_base;
#define at91_tc_read(idx, field) \
	__raw_readl(at91_local_tc_base + ATMEL_TC_REG(idx, field))
#define at91_tc_write(idx, field, value) \
	__raw_writel(value, at91_local_tc_base + ATMEL_TC_REG(idx, field))

#define _MASK_FIQ(p)	       	at91_gpio_b_write(PIO_IDR, p->read_pin_mask);
#define _UNMASK_FIQ(p)		at91_gpio_b_write(PIO_IER, p->read_pin_mask);

enum {
	MODE_REL_BIN,
	MODE_CUM_BIN,
	MDOE_REL_TXT,
	MODE_SIMPLE_TXT,
};

struct ir_buffer {
	int header;		/* header: 0xffffffff */
	u32 ntime;		/* signal time (ns) */
	struct timespec	start;	/* start time */
	struct timespec end;	/* end time */
	int rate;		/* timer clock rate */
	int count;		/* total number */
	u32 buf[MAX_BUF];	/* count value */
};

struct ir_learner_priv {
	int	enable_pin;
	bool	enable_pin_active_low;
	int	read_pin;
	int	read_pin_mask;
	struct atmel_tc *tc;
	u32	clk_rate;

	struct semaphore sem;

	int	opend;
	int	fiq_idx;
	struct ir_buffer buf[2];

	int	mode;

	/* text output */
	int	b_end;
	int	b_pos;
	char	*txt_buf;
};

static struct fiq_handler fh = {
	.name		= "ir_learner"
};

struct ir_learner_priv *ir_priv;

volatile static int flag;
static DECLARE_WAIT_QUEUE_HEAD(waitq);

static void enable_timer_irq(void)
{
	at91_aic_write(AT91_AIC_IECR, 1 << AT91SAM9X5_ID_PIOAB);
}

static void disable_timer_irq(void)
{
	at91_aic_write(AT91_AIC_IDCR, 1 << AT91SAM9X5_ID_PIOAB);
}

static void calc_buffer(struct ir_learner_priv *priv, struct ir_buffer *buf)
{
	int i;
	u32 *bbuf = buf->buf;

	WARN_ON(buf->count == 0);
	WARN_ON(buf->end.tv_sec == 0);

	buf->header = ~0;
	buf->rate = priv->clk_rate;

	buf->ntime = (1000000000 / buf->rate) * (bbuf[buf->count-1] - bbuf[0]);

	buf->start = buf->end;
	if (buf->start.tv_nsec < buf->ntime) {
		buf->start.tv_sec--;
		buf->start.tv_nsec += 1000000000;
	}
	buf->start.tv_nsec -= buf->ntime;

	switch (priv->mode)
	{
	case MODE_REL_BIN:
	case MDOE_REL_TXT:
		for (i = buf->count - 1; i >= 1; i--)
			bbuf[i] -= bbuf[i - 1];
		bbuf[0] = 0;
		break;
	case MODE_CUM_BIN:
		for (i = 1; i < buf->count; i++)
			bbuf[i] -= bbuf[0];
		bbuf[0] = 0;
		break;
	case MODE_SIMPLE_TXT:
	default:
		break;
	}
}

static int convert_text(struct ir_buffer *buf, char *txt, int mode)
{
	int i, j;
	int pos = 0;
	u32 *bbuf = buf->buf;
	pos += sprintf(txt, "========================================\n");
	pos += sprintf(txt+pos, "Elapsed Time: %d ns\n", buf->ntime);
	pos += sprintf(txt+pos, "Start Time: %d.%09d\n", buf->start.tv_sec,
		       buf->start.tv_nsec);
	pos += sprintf(txt+pos, "End Time: %d.%09d\n", buf->end.tv_sec,
		       buf->end.tv_nsec);
	pos += sprintf(txt+pos, "Clk Rate: %d Hz\n", buf->rate);
	pos += sprintf(txt+pos, "Total Samples: %d\n", buf->count);

	if (mode == MODE_SIMPLE_TXT) {
		pos += sprintf(txt+pos, "\n");
		return pos;
	}

	j = (buf->count / 8) * 8;
	for (i = 0; i < j; i+=8) {
		pos += sprintf(txt+pos, "%8d %8d %8d %8d %8d %8d %8d %8d\n",
			       bbuf[i+0], bbuf[i+1], bbuf[i+2], bbuf[i+3],
			       bbuf[i+4], bbuf[i+5], bbuf[i+6], bbuf[i+7]);
	}
	switch (buf->count - j) {
	case 7:
	case 6:
		pos += sprintf(txt+pos, "%8d %8d %8d %8d %8d %8d\n\n",
			       bbuf[i+0], bbuf[i+1], bbuf[i+2], bbuf[i+3],
			       bbuf[i+4], bbuf[i+5]);
		break;
	case 5:
	case 4:
		pos += sprintf(txt+pos, "%8d %8d %8d %8d\n\n",
			       bbuf[i+0], bbuf[i+1], bbuf[i+2], bbuf[i+3]);
		break;
	case 3:
	case 2:
		pos += sprintf(txt+pos, "%8d %8d\n\n", bbuf[i+0], bbuf[i+1]);
		break;
	default:
		break;
	}
	return pos;
}

static irqreturn_t ir_timeout_irq(int irq, void *handle)
{
	int next;
	struct ir_learner_priv *priv = handle;
	void __iomem *regs = priv->tc->regs;
	u32 stat = __raw_readl(regs + ATMEL_TC_REG(1, SR));
	struct pt_regs fiq_regs;

	u32 c = __raw_readl(regs + ATMEL_TC_REG(0, CV));

	if ((stat & ATMEL_TC_CPCS) == 0)
		return IRQ_NONE;

	/* stop clock */
	__raw_writel(ATMEL_TC_CLKDIS, regs + ATMEL_TC_REG(1, CCR));

	_MASK_FIQ(priv);

	get_fiq_regs(&fiq_regs);
	if (fiq_regs.ARM_fp < 10 || fiq_regs.ARM_fp % 2) {
		/* bad data. restart... */
		fiq_regs.ARM_fp = 0;
		set_fiq_regs(&fiq_regs);

		next = (priv->fiq_idx) ? 0 : 1;
		if (priv->buf[next].count == 0)
			_UNMASK_FIQ(priv);
		return IRQ_HANDLED;
	}

	priv->buf[priv->fiq_idx].count = fiq_regs.ARM_fp;
	getnstimeofday(&priv->buf[priv->fiq_idx].end);

	/* swap buffer */
	next = (priv->fiq_idx) ? 0 : 1;
	fiq_regs.ARM_r10 = (u32)priv->buf[next].buf;
	fiq_regs.ARM_fp = 0;
	set_fiq_regs(&fiq_regs);

	if (priv->buf[next].count) {
		/* blocking */
		return IRQ_HANDLED;
	}
	priv->fiq_idx = next;
	_UNMASK_FIQ(priv);

	return IRQ_WAKE_THREAD;
}

static irqreturn_t ir_timeout_thread(int irq, void *handle)
{
	flag = 1;
	wake_up_interruptible(&waitq);
	return IRQ_HANDLED;
}

static int ir_start_timer(struct ir_learner_priv *priv)
{
	int ret;
	void __iomem *regs = priv->tc->regs;
	u32 rate = clk_get_rate(priv->tc->clk[0]);
	u32 cmr = ATMEL_TC_TIMER_CLOCK2; /* MCLK/8 */
	u32 tout = DETECT_TOUT_MS * (rate / 8 / 1000);

	/* setup interrupt */
	ret = request_threaded_irq(priv->tc->irq[1],
				   ir_timeout_irq,
				   ir_timeout_thread,
				   IRQF_TIMER | IRQF_SHARED,
				   "ir_learner_irq",
				   priv);
	if (ret) {
		WARN_ON(ret);
		return ret;
	}

	/* 1st timer: free run time (clk: MCLK/8) */
	__raw_writel(cmr, regs + ATMEL_TC_REG(0, CMR));
	__raw_writel(0xff, regs + ATMEL_TC_REG(0, IDR));
	__raw_writel(ATMEL_TC_CLKEN | ATMEL_TC_SWTRG,
		     regs + ATMEL_TC_REG(0, CCR));

	/* 2nd timer: timeout timer */
	__raw_writel(cmr, regs + ATMEL_TC_REG(1, CMR));
	__raw_writel(tout, regs + ATMEL_TC_REG(1, RC));
	__raw_writel(0xff, regs + ATMEL_TC_REG(1, IDR));
	__raw_writel(ATMEL_TC_CPCS, regs + ATMEL_TC_REG(1, IER));
	__raw_writel(ATMEL_TC_CLKDIS, regs + ATMEL_TC_REG(1, CCR));
	return 0;
}

static void ir_stop_timer(struct ir_learner_priv *priv)
{
	void __iomem *regs = priv->tc->regs;

	/* disable timer */
	__raw_writel(0xff, regs + ATMEL_TC_REG(0, IDR));
	__raw_writel(0xff, regs + ATMEL_TC_REG(1, IDR));
	__raw_writel(ATMEL_TC_CLKDIS, regs + ATMEL_TC_REG(0, CMR));
	__raw_writel(ATMEL_TC_CLKDIS, regs + ATMEL_TC_REG(1, CMR));

	/* disable interrupt */
	free_irq(priv->tc->irq[1], priv);
}

static int ir_start(struct ir_learner_priv *priv)
{
	int ret;
	struct pt_regs fiq_regs;

	gpio_direction_output(priv->enable_pin, !priv->enable_pin_active_low);

	ret = ir_start_timer(priv);
	if (ret)
		goto _fail_timer;

	if (priv->mode >= MDOE_REL_TXT) {
		priv->txt_buf = kzalloc(TXT_BUF_SIZE, GFP_KERNEL);
		if (!priv->txt_buf) {
			ret = -ENOMEM;
			goto _fail_alloc;
		}
	}

	/* gpio AB to fiq */
	at91_aic_write(AT91_AIC_FFER, 1 << AT91SAM9X5_ID_PIOAB);
	at91_aic_write(AT91_AIC_SMR(AT91SAM9X5_ID_PIOAB), 0x07);

	at91_gpio_b_write(PIO_AIMDR, priv->read_pin_mask);

	fiq_regs.ARM_r8 = (unsigned long)at91_aic_base;
	fiq_regs.ARM_r9 = (unsigned long)at91_local_tc_base;
	fiq_regs.ARM_r10 = (unsigned long)priv->buf[0].buf;
	fiq_regs.ARM_fp = 0;
	set_fiq_regs(&fiq_regs);

	/* clear interrupt */
	ret = at91_gpio_b_read(PIO_ISR);

	/* enable interrupt */
	_UNMASK_FIQ(priv);
	at91_aic_write(AT91_AIC_IECR, 1 << AT91SAM9X5_ID_PIOAB);

	return 0;

_fail_alloc:
	ir_stop_timer(priv);
_fail_timer:
	gpio_direction_output(priv->enable_pin, priv->enable_pin_active_low);
	return ret;
}

static void ir_stop(struct ir_learner_priv *priv)
{
	int i;

	_MASK_FIQ(priv);
	at91_aic_write(AT91_AIC_IECR, 1 << AT91SAM9X5_ID_PIOAB);

	ir_stop_timer(priv);

	priv->b_end = 0;
	for (i = 0; i < ARRAY_SIZE(priv->buf); i++)
		memset(&priv->buf[i], 0, offsetof(struct ir_buffer, buf));
	priv->b_pos = 0;
	priv->fiq_idx = 0;
	if (priv->txt_buf) {
		kfree(priv->txt_buf);
		priv->txt_buf = NULL;
	}

	gpio_direction_output(priv->enable_pin, priv->enable_pin_active_low);
}

static u32 ir_poll(struct file *fp, poll_table * wait)
{
	int cur;
	unsigned int mask = 0;
	struct ir_learner_priv *priv = (void*)fp->private_data;

	if (down_interruptible(&priv->sem))
		return -ERESTARTSYS;

	flag = 0;
	cur = (priv->fiq_idx) ? 0 : 1;
	if (priv->buf[cur].count != 0)
		mask |= POLLIN | POLLRDNORM;    /* readable */
	else {
		poll_wait(fp, &waitq,  wait);
		cur = (priv->fiq_idx) ? 0 : 1;
		if (priv->buf[cur].count != 0)
			mask |= POLLIN | POLLRDNORM;
	}

	up(&priv->sem);
	return mask;
}

static ssize_t ir_read(struct file *fp, char __user *buf,
			   size_t count, loff_t *ppos)
{
	int ret = 0;
	int cur, left;
	void *src;
	struct ir_buffer *ir_buf;
	struct ir_learner_priv *priv = (void*)fp->private_data;

	if (down_interruptible(&priv->sem))
		return -ERESTARTSYS;

	flag = 0;
	cur = (priv->fiq_idx) ? 0 : 1;
	ir_buf = &priv->buf[cur];
	if (ir_buf->count == 0) {
		if (fp->f_flags & O_NONBLOCK) {
			ret = -EAGAIN;
			goto _end;
		}

		do {
			ret = wait_event_interruptible(waitq, flag != 0);
			if (ret)
				goto _end;

			cur = (volatile int)(priv->fiq_idx) ? 0 : 1;
			ir_buf = &priv->buf[cur];
		} while (ir_buf->count == 0);
	}

	if (priv->b_pos == 0) {
		/* first to read */
		calc_buffer(priv, ir_buf);
		if (priv->txt_buf) /* txt mode */
			priv->b_end = convert_text(ir_buf, priv->txt_buf,
						   priv->mode);
		else
			priv->b_end = sizeof(struct ir_buffer)
				- (MAX_BUF - ir_buf->count) * sizeof(u32);
	}

	if (priv->txt_buf) {
		left = priv->b_end - priv->b_pos;
		src = priv->txt_buf + priv->b_pos;
	} else {
		left = priv->b_end - priv->b_pos;
		src = (char*)ir_buf + priv->b_pos;
	}

	if (count > left)
		count = left;

	if (copy_to_user(buf, src, count)) {
		ret = -EFAULT;
		goto _end;
	}
	priv->b_pos += count;

	if (left == count) {
		priv->b_end = 0;
		priv->b_pos = 0;
		ir_buf->end.tv_sec = 0;
		ir_buf->end.tv_nsec = 0;

		disable_timer_irq();
		ir_buf->count = 0;
		/* if fiq stopped by overflow, restart */
		if (priv->buf[priv->fiq_idx].count) {
			priv->fiq_idx = cur;
			_UNMASK_FIQ(priv);
		}
		enable_timer_irq();
	}
	up(&priv->sem);
	return count;
_end:
	up(&priv->sem);
	return ret;
}

static int ir_open_raw(struct inode * inode, struct file * filp,
		       int mode)
{
	int ret = -EBUSY;
	struct ir_learner_priv *priv = ir_priv;

	if (down_interruptible(&priv->sem))
		return -ERESTARTSYS;

	if (priv->opend)
		goto _end;

	priv->mode = mode;
	ret = ir_start(priv);
	if (ret)
		goto _end;

	filp->private_data = ir_priv;
_end:
	up(&priv->sem);
	return ret;
}

static int ir_open_rel_bin(struct inode * inode, struct file * filp)
{
	return ir_open_raw(inode, filp, MODE_REL_BIN);
}

static int ir_open_cum_bin(struct inode * inode, struct file * filp)
{
	return 	ir_open_raw(inode, filp, MODE_CUM_BIN);
}

static int ir_open_rel_txt(struct inode * inode, struct file * filp)
{
	return 	ir_open_raw(inode, filp, MDOE_REL_TXT);
}

static int ir_open_simple_txt(struct inode * inode, struct file * filp)
{
	return 	ir_open_raw(inode, filp, MODE_SIMPLE_TXT);
}

static int ir_release(struct inode * inode, struct file * filp)
{
	struct ir_learner_priv *priv = filp->private_data;

	if (down_interruptible(&priv->sem))
		return -ERESTARTSYS;

	filp->private_data = NULL;

	ir_stop(priv);
	up(&priv->sem);
	return 0;
}

static const struct file_operations misc_fops[] =
{
	[MODE_REL_BIN] = {
		.owner		= THIS_MODULE,
		.read		= ir_read,
		.poll		= ir_poll,
		.open		= ir_open_rel_bin,
		.release	= ir_release,
	},
	[MODE_CUM_BIN] = {
		.owner		= THIS_MODULE,
		.read		= ir_read,
		.poll		= ir_poll,
		.open		= ir_open_cum_bin,
		.release	= ir_release,
	},
	[MDOE_REL_TXT] = {
		.owner		= THIS_MODULE,
		.read		= ir_read,
		.poll		= ir_poll,
		.open		= ir_open_rel_txt,
		.release	= ir_release,
	},
	[MODE_SIMPLE_TXT] = {
		.owner		= THIS_MODULE,
		.read		= ir_read,
		.poll		= ir_poll,
		.open		= ir_open_simple_txt,
		.release	= ir_release,
	},
};

static struct miscdevice misc_device[] = {
	[MODE_REL_BIN] = {
		.minor		= MISC_DYNAMIC_MINOR,
		.name		= "ir_learner_rel_bin",
		.fops		= &misc_fops[MODE_REL_BIN]
	},
	[MODE_CUM_BIN] = {
		.minor		= MISC_DYNAMIC_MINOR,
		.name		= "ir_learner_cum_bin",
		.fops		= &misc_fops[MODE_CUM_BIN]
	},
	[MDOE_REL_TXT] = {
		.minor		= MISC_DYNAMIC_MINOR,
		.name		= "ir_learner_rel_txt",
		.fops		= &misc_fops[MDOE_REL_TXT]
	},
	[MODE_SIMPLE_TXT] = {
		.minor		= MISC_DYNAMIC_MINOR,
		.name		= "ir_learner_simple_txt",
		.fops		= &misc_fops[MODE_SIMPLE_TXT]
	}
};

static int __init ir_learner_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct pinctrl *pinctrl;
	struct ir_learner_priv *priv;
	enum of_gpio_flags flags;
	struct atmel_tc *tc;
	int ret, i;
	
	if (!dev->of_node)
		return -EINVAL;

	/* register FIQ */
	ret = claim_fiq(&fh);
	if (ret)
		return ret;

	set_fiq_handler(&ir_learner_fiq_start, &ir_learner_fiq_end -
					       &ir_learner_fiq_start);

	BUG_ON((char*)at91_gpio_base[0] - (char*)at91_aic_base
	       != OFFSET_PIOA);

	/* Get Timer Block */
	tc = atmel_tc_alloc(1, "ir_learner");
	if (tc == NULL) {
		dev_err(&pdev->dev, "failed to allocate Timer Counter Block\n");
		ret = -ENOMEM;
		goto _free_fiq;
	}
	at91_local_tc_base = tc->regs;

	pinctrl = devm_pinctrl_get_select_default(&pdev->dev);
	if (IS_ERR(pinctrl)) {
		ret = PTR_ERR(pinctrl);
		goto _free_tc;
	}

	priv = devm_kzalloc(dev, sizeof(struct ir_learner_priv), GFP_KERNEL);
	if (!priv) {
		ret = -ENOMEM;
		goto _free_tc;
	}

	priv->tc = tc;
	priv->clk_rate = clk_get_rate(tc->clk[0]) / 8;
	priv->enable_pin = of_get_gpio_flags(pdev->dev.of_node, 0, &flags);
	priv->enable_pin_active_low = (flags == OF_GPIO_ACTIVE_LOW);
	ret = devm_gpio_request(&pdev->dev, priv->enable_pin, "ENABLE_IR");
	if (ret < 0)
		return ret;
	gpio_direction_output(priv->enable_pin, priv->enable_pin_active_low);

	priv->read_pin = of_get_gpio(pdev->dev.of_node, 1);
	ret = devm_gpio_request(&pdev->dev, priv->read_pin, "READ_IR");
	if (ret < 0)
		return ret;
	gpio_direction_input(priv->read_pin);
	priv->read_pin_mask = (1 << (priv->read_pin % AT91_PIN_PB0));

	platform_set_drvdata(pdev, priv);
	ir_priv = priv;

	/* register input device */
	sema_init(&priv->sem, 1);
	for (i = 0; i < ARRAY_SIZE(misc_device); i++) {
		ret = misc_register(&misc_device[i]);
		if (ret)
			break;
	}

	if (ret) {
		for (; i >= 0; i--)
			misc_deregister(&misc_device[i]);
		goto _free_tc;
	}

	printk(KERN_INFO "ir_learner: initialized\n");
	return 0;

_free_tc:
	atmel_tc_free(tc);
_free_fiq:
	release_fiq(&fh);
	return ret;
}

static int ir_learner_remove(struct platform_device *pdev)
{
	int i;
	struct ir_learner_priv *priv = platform_get_drvdata(pdev);

	ir_priv = NULL;
	for (i = 0; i < ARRAY_SIZE(misc_device); i++)
		misc_deregister(&misc_device[i]);

	release_fiq(&fh);
	atmel_tc_free(priv->tc);

	platform_set_drvdata(pdev, NULL);
	return 0;
}

static const struct of_device_id ir_learner_dt_ids[] = {
	{ .compatible = "florida,ir-learner", },
	{},
};
MODULE_DEVICE_TABLE(of, ir_learner_dt_ids);

static struct platform_driver ir_learner_device_driver = {
	.probe		= ir_learner_probe,
	.remove		= ir_learner_remove,
	.driver		= {
		.name	= "ir_learner",
		.of_match_table = ir_learner_dt_ids,
		.owner		= THIS_MODULE,
	}
};

static int __init ir_learner_init(void)
{
	return platform_driver_register(&ir_learner_device_driver);
}
device_initcall(ir_learner_init);

static void __exit ir_learner_exit(void)
{
	platform_driver_unregister(&ir_learner_device_driver);
}
module_exit(ir_learner_exit);

MODULE_AUTHOR("YongSung Lee <aqewrf@gmail.com>");
MODULE_DESCRIPTION("GPIO based IR Learner");
MODULE_LICENSE("GPL v2");
