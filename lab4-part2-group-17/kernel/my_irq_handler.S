@ my_irq_handler.S
@ author: Yuqi Liu<yuqil@andrew.cmu.edu> Wei Chen <wc2@andrew.cmu.edu>
@ date: Nov 6 22:20

	.global my_irq_handler
	.extern c_irq_hander
my_irq_handler:
	@ store registers in stack
	@ go to c_irq_handler
	@ disable irq interrupt by default
	stmfd	sp!, {r0-r12, lr}
	bl c_irq_handler
	ldmfd	sp!, {r0-r12, lr}
	subs pc, lr, #4
