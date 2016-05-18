/*
 * author: Yuqi Liu <yuqil@andrew.cmu.edu> Wei Chen <wc2@andrew.cmu.edu>
 */
#include <exports.h>
#include <arm/psr.h>
#include <arm/exception.h>
#include <arm/interrupt.h>
#include <arm/timer.h>
#include <lock.h>
#include "bits/swi.h"
#include "bits/fileno.h"
#include "bits/errno.h"
#include "arm/timer.h"
#include "arm/reg.h"
#include "arm/interrupt.h"
#include "address.h"
#include "main.h"
#include "sched.h"
#include "syscall.h"

extern volatile unsigned long time_ms;
 /* 
 * description: new swi handler
 * input: swi number
 *		  parameters for systems call
 * return:
 */
void c_swi_handler(unsigned swi_num, unsigned *regs){
#ifdef DEBUG
	printf("In c_swi_handler. cong! swi num: %04x\n", swi_num);
#endif
	
	unsigned long cur_time, end_sleep_time;

	// read
	if (READ_SWI == swi_num){
#ifdef DEBUG
		printf("read fd: %d\n", regs[0]);
#endif
		regs[0] = my_read(regs[0], (void*)regs[1], regs[2]);
	} 

	// write
	else if (WRITE_SWI == swi_num){
#ifdef DEBUG
		printf("write fd: %d\n", regs[0]);
#endif
		regs[0] = my_write(regs[0], (const void*)regs[1], regs[2]);
	} 

	// sleep certain time
	else if (SLEEP_SWI == swi_num) {
#ifdef DEBUG
		printf("in sleep function..\n");
#endif
		cur_time = time_ms;
		end_sleep_time = cur_time + regs[0];
		while (time_ms < end_sleep_time);
#ifdef DEBUG
		printf("go out from sleep..\n");
#endif
	} 

	// time : return current time
	else if (TIME_SWI == swi_num) {
		regs[0] = time_ms;
	} 

	// create task
	else if (CREATE_SWI == swi_num){
		//printf("#########%ld\n", (size_t)regs[1]);
		regs[0] = task_create((task_t*)regs[0], (size_t)regs[1]);
	} 

	// event_wait
	else if (EVENT_WAIT == swi_num){
		regs[0] = event_wait((unsigned int)regs[0]);
	} 

	// create mutex
	else if (MUTEX_CREATE == swi_num){
		//printf("begin mutex create.\n");
		regs[0] = mutex_create();
		//printf("mutex_create finished.\n");
	} 

	// lock mutex
	else if (MUTEX_LOCK == swi_num){
		regs[0] = mutex_lock((int)regs[0]);
	} 

	// unlock mutex
	else if (MUTEX_UNLOCK == swi_num){
		regs[0] = mutex_unlock((int)regs[0]);
	}

	
	else{
		printf("Invalid swi number.\n");
		return;
	}
	
}
