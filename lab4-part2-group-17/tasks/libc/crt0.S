@
@ Authors: Wei Chen <wc2@andrew.cmu.edu>
@ Date:    Mon Oct 12 20:17:23 EDT 2015

#include <asm.h>
#include <bits/swi.h>
	.file "crt0.S"

FUNC(_start)
	@ needs code.  Put your names at the top.

_start:
	@ load argc and argv
	ldmfd 	sp!, {r0, r1}
	@ branch with link to main
	bl 	main
.L:
	b	.L
