#include <linux/linkage.h>
#include <asm/assembler.h>
#include <asm/cp15.h>


/* We have R8 through R13 to play with */

#ifdef __ASSEMBLY__
#define __REG_NR(x)     r##x
#else
#define __REG_NR(x)     (x)
#endif

#define fiq_rfiq_raxitmrcnt	__REG_NR(8)
#define fiq_raxitmrcsr	__REG_NR(9)
#define fiq_rtmp		__REG_NR(10)
/*
#define fiq_rrx		__REG_NR(10)
#define fiq_rtx		__REG_NR(11)
#define fiq_rcount	__REG_NR(12)
#define fiq_rirq	__REG_NR(13)


	temp = ioread32(pTIMER_TCSR0);// clear timer IRQ
	temp |= T0INT;
	iowrite32(temp, pTIMER_TCSR0);
*/

.text
.global axitimer_fiq_handler_end
.extern axitimer_fiq_handler
.extern debug_print

ENTRY(axitimer_fiq_handler_asm)
	ldr	fiq_rtmp, [fiq_raxitmrcsr]
	orr 	fiq_rtmp, fiq_rtmp, #0x00000100
	str	fiq_rtmp, [fiq_raxitmrcsr]
axitimer_fiq_handler_end:	bl	debug_print

/*
ENTRY(axitimer_fiq_handler_asm)
axitimer_fiq_handler_asm:
	.word	fiq_rx_end1 - fiq_rx_start1
	.word	fiq_rx_irq_ack1 - fiq_rx_start1
fiq_rx_start1:
	ldr	fiq_rtmp, [fiq_raxitmrcsr]
	orr 	fiq_rtmp, fiq_rtmp, #0x00000100
	str	fiq_rtmp, [fiq_raxitmrcsr]
fiq_rx_irq_ack1:
	.word	0
fiq_rx_end1:
axitimer_fiq_handler_end:
*/
