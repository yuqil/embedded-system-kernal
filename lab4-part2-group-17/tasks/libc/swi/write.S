/** @file write.S
 *
 * @brief write sycall wrapper
 *
 * @author Yuqi Liu <yuqil@andrew.cmu.edu>
 *         Wei Chen <wc2@andrew.cmu.edu>
 * @date   Sun, 1 NOV 
 *
 */

#include <asm.h>
#include <bits/swi.h>
	.file "write.S"

    FUNC(write)
	.global errono
	
write:
	@ call swi
	swi	WRITE_SWI
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
