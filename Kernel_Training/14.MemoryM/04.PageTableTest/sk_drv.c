#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

#include <linux/fs.h>

//#include <asm-arm/thread_info.h>	//current_thread_info():2.6.17-13
#include <asm-generic/uaccess.h>	//get_current()

#include <asm/pgtable.h>

int sk_major=0;

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
	static int t_data=33;
	struct vm_area_struct *vma;
	struct mm_struct *mm;
	pgd_t *pgd;
	pud_t *pud;
	pmd_t *pmd;
	pte_t *pte;
	unsigned long pfn;
	
	printk(KERN_WARNING "[SK_DD]read..\n");
	
	printk("VA(buf):%p\n", buf); 
	t_data++;
	copy_to_user(buf, &t_data, count);

	vma = get_current()->mm->mmap;
	mm = vma->vm_mm;
	pgd = pgd_offset(mm, (long)buf); 
	if(pgd_none(*pgd)) {
		printk("[ERROR]pgd_none!!\n");
		return -1;
	}

	pud = pud_offset(pgd, (long)buf); 
	if(pud_none(*pud)) {
		printk("[ERROR]pud_none!!\n");
		return -2;
	}

	pmd = pmd_offset(pud, (long)buf); 
	if(pmd_none(*pmd)) {
		printk("[ERROR]pmd_none!!\n");
		return -3;
	}
	
	pte = pte_offset_map(pmd, (long)buf); 
	if(pte_present(*pte)) {
		pfn = pte_pfn(*pte);
		printk("pfn:%lu\n", pfn);
	} else {
		printk("[ERROR]pte_none!!\n");
		return -4;
	}
	
	return count;
}


ssize_t sk_write (struct file *filp, const char *buf, size_t count, loff_t *f_pos)
{
	printk(KERN_WARNING "[SK_DD]write..\n");
	return count;
}


//int sk_ioctl(struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg)
long sk_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	printk(KERN_WARNING "[SK_DD]ioctl..\n");
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

int sk_init(void)
{
	int error;

	printk("<1> SK Module id Up....\n");

	error = register_chrdev(sk_major, "sk", &sk_fops);
	if(error < 0) {
		printk(KERN_WARNING "sk:register_chrdev() error!!\n");
		return error;
	} else if(sk_major==0) sk_major = error;
	printk("<1> sk:register_chrdev() ok! => major_num:%d\n", sk_major);

	printk(KERN_WARNING "SK Module Insert Done!!\n");
	return 0;
}

void sk_exit(void)
{
	printk("<1> SK Module id Down....\n");

	unregister_chrdev(sk_major, "sk");

	printk(KERN_WARNING "SK Module Delete Done!!\n");
}

module_init(sk_init);
module_exit(sk_exit);

MODULE_LICENSE("Dual BSD/GPL");
