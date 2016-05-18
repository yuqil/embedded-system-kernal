/** @file sched.c
 * 
 * @brief Top level implementation of the scheduler.
 *
 * @author Wei Chen<wc2@andrew.cmu.edu>, Yuqi Liu<yuqil@andrew.cmu.edu>
 * @date 2008-11-20
 */

#include <types.h>
#include <assert.h>

#include <kernel.h>
#include <config.h>
#include "sched_i.h"

#include <arm/reg.h>
#include <arm/psr.h>
#include <arm/exception.h>
#include <arm/physmem.h>

tcb_t system_tcb[OS_MAX_TASKS]; /*allocate memory for system TCBs */
static void __attribute__((unused)) idle(void);

void sched_init(task_t* main_task  __attribute__((unused)))
{
	tcb_t* idle_tcb = &system_tcb[OS_MAX_TASKS - 1];
	#ifdef DEBUG
	    printf("in sched_init: init idle task\n");
	#endif

	// initial idle task    
	idle_tcb->cur_prio = IDLE_PRIO;
	idle_tcb->sleep_queue = 0;
	idle_tcb->holds_lock = 0;
	idle_tcb->context.r4 = (uint32_t)idle;
	idle_tcb->context.r5 = 0;
	idle_tcb->context.r6 = 0;
	idle_tcb->context.r8 = global_data;
	idle_tcb->context.sp = (void*)idle_tcb->kstack_high;
	idle_tcb->context.lr = (void*)launch_task;

	// add idle task
	runqueue_add(idle_tcb, IDLE_PRIO);
}

/**
 * @brief This is the idle task that the system runs when no other task is runnable
 */
 
static void __attribute__((unused)) idle(void)
{
	 enable_interrupts();
	 while(1);
}

/* Function sort_tasks, 
 * Input: task array tasks, number of tasks
 */
void sort_tasks(task_t* tasks, size_t num_tasks){
	uint8_t i, j, idx;
	uint32_t min;
	task_t tmp;
	#ifdef DEBUG
	    printf("begin to sort tasks.\n");
	#endif

	// sort tasks using selection sort
	for (i = 0; i < num_tasks; i++){
		idx = i;
		min = (uint32_t)(tasks[i].T);
		for (j = i + 1; j < num_tasks; j++){
			if ((uint32_t)(tasks[j].T) < min){
				min = (uint32_t)(tasks[j].T);
				idx = j;
			}
		}
		if (idx != i){
			tmp = tasks[i];
			tasks[i] = tasks[idx];
			tasks[idx] = tmp;
		}
	}
	#ifdef DEBUG
	    printf("sort finisded\n");
	#endif
}

/**
 * @brief Allocate user-stacks and initializes the kernel contexts of the
 * given threads.
 *
 * This function assumes that:
 * - num_tasks < number of tasks allowed on the system.
 * - the tasks have already been deemed schedulable and have been appropriately
 *   scheduled.  In particular, this means that the task list is sorted in order
 *   of priority -- higher priority tasks come first.
 *
 * @param tasks  A list of scheduled task descriptors.
 * @param size   The number of tasks is the list.
 */
void allocate_tasks(task_t** tasks  __attribute__((unused)), size_t num_tasks  __attribute__((unused)))
{
	tcb_t *new_tcb;
	uint8_t i = 0;

	#ifdef DEBUG
	    printf("begin to allocate task\n");
	#endif

	// allocate tasks to tcbs
    for (i = 0; i < num_tasks; i++){
		new_tcb = &system_tcb[i + 1];
		new_tcb->cur_prio = i + 1;
        new_tcb->native_prio = i + 1;
		new_tcb->sleep_queue = 0;
		new_tcb->holds_lock = 0;

		new_tcb->context.r4 = (uint32_t)((*tasks)[i].lambda);
		new_tcb->context.r5 = (uint32_t)((*tasks)[i].data);
		new_tcb->context.r6 = (uint32_t)((*tasks)[i].stack_pos);
		new_tcb->context.r8 = global_data;
		new_tcb->context.sp = (void*)new_tcb->kstack_high;
		new_tcb->context.lr = (void*)launch_task;

		#ifdef DEBUG
		    printf("in allocate_tasks: fun: %04x\n", (*tasks)[i].lambda);
		    printf("in alloctate_tasks: %04x task's r4: %04x\n", i, system_tcb[i].context.r4);
        #endif

		// add to runqueue
		runqueue_add(new_tcb, i + 1);
	}
	#ifdef DEBUG
	    printf("finied task allocation\n");
	#endif
}

