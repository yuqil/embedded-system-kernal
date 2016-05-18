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
 * description: init timer
 * input:
 * return:
 */
extern volatile unsigned long time_ms;

void init_timer(){
#ifdef DEBUG
	printf("timer inited...\n");
#endif
	time_ms = 0;
	// enable OSMR0, set OSMR value, set OSCR = 0
	reg_set(OSTMR_OIER_ADDR, OSTMR_OIER_E0);
	reg_write(OSTMR_OSMR_ADDR(0), 32500);
	reg_write(OSTMR_OSCR_ADDR, 0);
}
