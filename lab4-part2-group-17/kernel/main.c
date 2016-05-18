/** @file main.c
 *
 * @brief kernel main
 *
 * @author 
 *	   
 *	   
 * @date   
 */
 
#include <kernel.h>
#include <task.h>
#include <sched.h>
#include <device.h>
#include <assert.h>
#include <lock.h>
#include "main.h"
#include "bits/swi.h"
#include "bits/fileno.h"
#include "bits/errno.h"
#include "arm/timer.h"
#include "arm/reg.h"
#include "arm/interrupt.h"

uint32_t global_data;
volatile unsigned long time_ms;

void my_print(){
	printf("I am here2\n");
}

int kmain(int argc __attribute__((unused)), char** argv  __attribute__((unused)), uint32_t table)
{
	app_startup();
	global_data = table;

	#ifdef DEBUG
	    printf("global data: %04x\n", table);
	#endif

	unsigned int old_swi_handler[3];  // record old swi
	unsigned int old_irq_handler[3];  // record old irq

	// hack swi and irq handler
	wireHandler((unsigned int*)0x08, &my_swi_handler, (unsigned int*)&old_swi_handler);
	wireHandler((uint32_t*)0x18, &irq_wrapper, (uint32_t*)&old_irq_handler);
	
	#ifdef DEBUG
	    printf("wireHandler finished.\n");
	#endif 

	init_timer();    // initialize timer
	init_irq();      // initialize irqs
	mutex_init();    // initialize mutex

	#ifdef DEBUG
	    printf("ready to load\n");
	#endif

	ld_usr_pro(argc, argv);    // load user program
	recover(old_swi_handler);  // recover swi
	recover(old_irq_handler);  // recover irq
		
	assert(0);        /* should never get here */
}
