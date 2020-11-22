#include <linux/linkage.h>
#include <asm/assembler.h>

#include <mach/at91sam9x5.h>
#include "../../arch/arm/mach-at91/at91_aic.h"
#include "../../arch/arm/mach-at91/include/mach/at91_pio.h"

#include "ir_learner.h"

#define PIO_MASK	(1 << 14)

#define TC_CHAN(idx)	((idx)*0x40)
#define TC_REG(idx, reg) (TC_CHAN(idx) + TC_ ## reg)
#define TC_CCR		0x00		/* Channel Control Register */
#define TC_CV		0x10		/* counter Value */
#define TC0_CV		TC_REG(0, CV)
#define TC1_CCR		TC_REG(1, CCR)
#define TC_CLKEN	(1 << 0)	/* clock enable */
#define TC_CLKDIS	(1 << 1)	/* clock disable */
#define TC_SWTRG	(1 << 2)	/* software trigger */

	.text
/*
 * IR input is low active.
 *
 * R8: AIC base address
 * R9: timer capture address
 * R10: buffer pointer
 * R11(FP): count
 * R12(IP):
 * R13(SP):
 */
	.global ir_learner_fiq_end
ENTRY(ir_learner_fiq_start)
	@ clear PIO int
	ldr	r13, [r8, #OFFSET_PIOB + PIO_ISR]

	@ read data pin
	ldr	r13, [r8, #OFFSET_PIOB + PIO_PDSR]
	and 	r13, #PIO_MASK

	@ first input date must be low active
	tst	r11, r11
	eoreqs	r12, r13, #PIO_MASK
	subeqs	pc, lr, #4

	@ check overflow
	cmp	r11, #MAX_BUF
	subges	pc, lr, #4

	@ save data
	ldr	r13, [r9, #TC0_CV]
	str	r13, [r10, r11, LSL #2]
	add	r11, #1

	@ re-trigger timeout timer
	mov	r13, #(TC_SWTRG + TC_CLKEN)
	str	r13, [r9, #TC1_CCR]

	@ return from FIQ
	subs 	pc, lr, #4
ir_learner_fiq_end:
