/*
 * author: Yuqi Liu <yuqil@andrew.cmu.edu> Wei Chen <wc2@andrew.cmu.edu>
 */


#include <exports.h>
#include <arm/psr.h>
#include <arm/exception.h>
#include <arm/interrupt.h>
#include <arm/timer.h>
#include <device.h>
#include "bits/swi.h"
#include "bits/fileno.h"
#include "bits/errno.h"
#include "arm/timer.h"
#include "arm/reg.h"
#include "arm/interrupt.h"
#include "address.h"
#include "main.h"
#include "config.h"

extern volatile unsigned long time_ms;
 
/* 
 * description: new irq handler
 * input:
 * return:
 */
void c_irq_handler() {
	time_ms += OS_TIMER_RESOLUTION;
#ifdef DEBUG
	printf("%ld\n", time_ms);
#endif
	// set OSCR = 0, 
	reg_write(OSTMR_OSCR_ADDR, 0);
	reg_set(OSTMR_OSSR_ADDR, OSTMR_OSSR_M0);

	// check the device sleeping queue if any event occurs
	dev_update(time_ms);
}
