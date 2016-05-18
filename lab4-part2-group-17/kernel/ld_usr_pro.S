@ ld_usr_pro.S
@ author: Yuqi Liu <yuqil@andrew.cmu.edu> Wei Chen <wc2@andrew.cmu.edu>
@ date:	  Nov.6 22:11

#include <address.h>
#include <bits/swi.h>
	.file	"ld_usr_pro.S"
	.text
	.global ld_usr_pro
ld_usr_pro:
	@ from kernel to user mode
	@ store registers
	stmfd	sp!, {r1-r12, lr}
	@ldr	r2, =UBOOT_STACK
	ldr r2, =0xa0f00000
	str	sp, [r2]
	@ set cpsr, change svc mode to user mode
	mrs	r3, cpsr
	bic	r3, #0xff
	orr	r3, #0x10
	msr	cpsr, r3
	@ mov args 
	ldr	sp, =USER_STACK
	mov	r2, #0
	str	r2, [sp], #-4
	mov	r3, r0
.set_st:
	subs	r3, r3, #1
	blt	.end
	@ r1: start address
	@ r3 LSL #2: offset
	@ r2: new address
	add	r2, r1, r3, LSL #2
	ldr	r4, [r2]
	str	r4, [sp], #-4
	b	.set_st
.end:
	@ mov argc
	str	r0, [sp], #-4
	@ r8 points to global_data
	@ must keep it
	ldr ip, =global_data
	ldr r8, [ip]

	@ go to user pro
	ldr pc, =USER_PROG
