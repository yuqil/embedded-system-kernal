/** @file ctx_switch.c
 * 
 * @brief C wrappers around assembly context switch routines.
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date 2008-11-21
 */
 

#include <types.h>
#include <assert.h>

#include <config.h>
#include <kernel.h>
#include "sched_i.h"

#ifdef DEBUG_MUTEX
#include <exports.h>
#endif

static __attribute__((unused)) tcb_t* cur_tcb; /* use this if needed */
/*
 * @brief Initialize the current TCB and priority.
 *
 * Set the initialization thread's priority to IDLE so that anything
 * will preempt it when dispatching the first task.
 */
void dispatch_init(tcb_t* idle __attribute__((unused)))
{
	
}


/**
 * @brief Context switch to the highest priority task while saving off the 
 * current task state.
 *
 * This function needs to be externally synchronized.
 * We could be switching from the idle task.  The priority searcher has been tuned
 * to return IDLE_PRIO for a completely empty run_queue case.
 */
void dispatch_save(void)
{
	uint8_t highest_priority = highest_prio();
	// if there is a higher priority task, context switch
	if (cur_tcb->cur_prio > highest_priority){  
		tcb_t *tmp_tcb = cur_tcb;
		tcb_t *next_tcb = runqueue_remove(highest_priority);
		runqueue_add(cur_tcb, cur_tcb->cur_prio);
		cur_tcb = next_tcb;

		ctx_switch_full(&next_tcb->context, &tmp_tcb->context);
	}
}

/**
 * @brief Context switch to the highest priority task that is not this task -- 
 * don't save the current task state.s
 *
 * There is always an idle task to switch to.
 */
void dispatch_nosave(void)
{
	uint8_t highest_priority = highest_prio();
	#ifdef DEBUG
	    printf("in dispatch_nosave.\n");
	#endif

	// remove the highest priority task
	cur_tcb = runqueue_remove(highest_priority);

	// context switch
	ctx_switch_half(&cur_tcb->context);
}


/**
 * @brief Context switch to the highest priority task that is not this task -- 
 * and save the current task but don't mark is runnable.
 *
 * There is always an idle task to switch to.
 */
void dispatch_sleep(void)
{
	// get highest priority job and context switch
	uint8_t highest_priority = highest_prio();
	tcb_t* tmp_tcb = cur_tcb;
	tcb_t *next_tcb = runqueue_remove(highest_priority);

	#ifdef DEBUG
	printf("in dispatch_sleep: one task finished, switch to another.\n");
	printf("next tcb's lr: %d\n", next_tcb->context.lr);
	#endif 

	cur_tcb = next_tcb;

	// if remove the idle task, put it on the runqueue again
	if (next_tcb -> cur_prio == 63) {
		runqueue_add(next_tcb, 63);
	}

	ctx_switch_full(&next_tcb->context, &tmp_tcb->context);
}

/**
 * @brief Returns the priority value of the current task.
 */
uint8_t get_cur_prio(void)
{
	return cur_tcb->cur_prio;
}

/**
 * @brief Returns the TCB of the current task.
 */
tcb_t* get_cur_tcb(void)
{
	return cur_tcb;
}
