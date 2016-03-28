#include <linux/module.h>	/* Needed by all modules */
#include <linux/kernel.h>	/* Needed for KERN_INFO	*/
#include <linux/init.h>		/* Needed for the macros */

#define	DRIVER_AUTHOR	"Soo Hyun Kim <kshcyber@korea.com>"
#define	DRIVER_DESC	"A sample driver"

static void __exit exit_hello(void)
{
	printk(KERN_INFO "Goodbye, Hello Module.\n");
}

module_exit(exit_hello);
/* Get rid of taint message by declaring code as GPL. */
MODULE_LICENSE("GPL");
MODULE_AUTHOR(DRIVER_AUTHOR);		/* Who wrote this module? */
MODULE_DESCRIPTION(DRIVER_DESC);	/* What does this module do */
MODULE_SUPPORTED_DEVICE("testdevice");


/* execution
   #modinfo hello_multi.ko
*/
