/** @file time.S
 *
 * @brief time sycall wrapper
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date 2008-10-31
 */

#include <asm.h>
#include <bits/swi.h>

	.file "mutex_unlock.S"

FUNC(mutex_unlock)
	@ Add code here.  Put your name at the top.
	swi MUTEX_UNLOCK
	@ cmp ret val with 0
	cmp	r0, #0
	@ greater or equal
	bge	.ret
	@ set errno to -r0
	rsb	r0, r0, #0
	@ set errno
	ldr	r3, =errno
	str	r0, [r3]
	@ set ret value
	mov	r0, #-1
.ret:
	@ moc lr to pc
	mov	pc, lr
