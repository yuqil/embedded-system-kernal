@ my_swi_handler.S
@ author: Yuqi Liu <yuqil@andrew.cmu.edu> Wei Chen <wc2@andrew.cmu.edu>
@ date: Nov 6 22:16
	.extern	c_swi_handler
	.global my_swi_handler

.extern C_SWI_HANDLER 

my_swi_handler:
	@ store registers in stack
	@ get swi num
	@ go to c_swi_handler
	stmfd	sp!, {r0-r12, lr}
	mov r1, sp
	mrs	r2, spsr
	stmfd	sp!, {r2}

	@ set up r8 correctly
	ldr	ip, =global_data
	ldr r8, [ip]

	@ enable irq fiq and modify cpsr
	mrs	r12, cpsr
	bic r12, r12, #0xC0
	msr cpsr, r12
	ldr	r0, [lr, #-4]
	@ get swi number
	bic	r0, r0, #0xff000000

	@ in svc mode
	@ store user sp, lr on stack
	stmfd sp, {sp,lr}^
	add sp, sp, #-8
	bl	c_swi_handler
	ldmfd sp, {sp, lr}^
	add sp, sp, #8

	ldmfd	sp!, {r2}
	msr	spsr, r2

	ldmfd	sp!, {r0-r12, lr}
	movs	pc, lr
