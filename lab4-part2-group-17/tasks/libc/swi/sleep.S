/** @file sleep.S
 *
 * @brief sleep sycall wrapper
 *
 * @author Yuqi Liu <yuqil@andrew.cmu.edu>
 *         Wei Chen <wc2@andrew.cmu.edu>
 * @date   Sun, 1 NOV 
 *
 */

#include <asm.h>
#include <bits/swi.h>

	.file "sleep.S"

FUNC(sleep)
	@ Add code here.  Put your names at the top.
	swi SLEEP_SWI

.L1:
    mov pc, lr
