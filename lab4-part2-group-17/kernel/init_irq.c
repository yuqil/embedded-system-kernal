/*
 * author: Yuqi Liu <yuqil@andrew.cmu.edu> Wei Chen <wc2@andrew.cmu.edu>
 */
#include <exports.h>
#include <arm/psr.h>
#include <arm/exception.h>
#include <arm/interrupt.h>
#include <arm/timer.h>
#include "bits/swi.h"
#include "bits/fileno.h"
#include "bits/errno.h"
#include "arm/timer.h"
#include "arm/reg.h"
#include "arm/interrupt.h"
#include "address.h"
#include "main.h"

/* 
 * description: init irq handler
 * input:
 * return:
 */
void init_irq(){
#ifdef DEBUG
	printf("irq inited...\n");
#endif
	// enable OSMR0, set irq stack, enable irq
	reg_write(INT_ICMR_ADDR, 1 << INT_OSTMR_0);
	reg_write(INT_ICLR_ADDR, 0);
}
