/** @file read.S
 *
 * @brief read sycall wrapper
 *
 * @author Yuqi Liu <yuqil@andrew.cmu.edu>
 *         Wei Chen <wc2@andrew.cmu.edu>
 * @date   Sun, 1 NOV 
 *
 */

#include <asm.h>
#include <bits/swi.h>

	.file	"read.S"
	
	FUNC(read)
	.global errno

read:
	@ call swi
	swi	READ_SWI
	cmp	r0, #0
	bge	.ret
	rsb	r0, r0, #0
	ldr	r3, =errno
	str	r0, [r3]
	@ set ret value
	mov	r0, #-1
.ret:
	@ set ret addr
	mov	pc, lr
