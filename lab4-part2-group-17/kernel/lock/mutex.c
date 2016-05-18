/**
 * @file mutex.c
 *
 * @brief Implements mutices.
 *
 * @author Harry Q Bovik < PUT YOUR NAMES HERE
 *
 * 
 * @date  
 */

//#define DEBUG_MUTEX

#include <lock.h>
#include <task.h>
#include <sched.h>
#include <bits/errno.h>
#include <arm/psr.h>
#include <arm/exception.h>
#ifdef DEBUG_MUTEX
#include <exports.h> // temp
#endif

mutex_t gtMutex[OS_NUM_MUTEX];

/* initial mutex */
void mutex_init()
{
	int i;
	for (i = 0; i < OS_NUM_MUTEX; i++){
		gtMutex[i].bAvailable = TRUE;
		gtMutex[i].pHolding_Tcb = 0;
		gtMutex[i].bLock = FALSE;
		gtMutex[i].pSleep_queue = 0;
	}
}

int mutex_create(void)
{
	int i = 0;

#ifdef DEBUG
	printf("in mutex create.\n");
#endif

	// disable interrupts
	disable_interrupts();
	for (i = 0; i < OS_NUM_MUTEX; i++){

		#ifdef DEBUG
		printf("%d mutex: %d\n", i, gtMutex[i].bAvailable);
		#endif

		if (gtMutex[i].bAvailable){
			gtMutex[i].bAvailable = 0;

			#ifdef DEBUG
			printf("successfully create mutex %d\n", i);
			#endif

			enable_interrupts();
			return i;
		}
	}
	enable_interrupts();
	return -ENOMEM;
}


/*  Function: mutex_lock
 *  input: mutex number
 *  output: execuate state, if <0, error
 */
int mutex_lock(int mutex  __attribute__((unused)))
{
	#ifdef DEBUG
	    printf("in mutex_lock.\n");
	#endif
	// check error case
	if (gtMutex[mutex].bAvailable)	return -EINVAL;
	if (mutex < 0 || mutex >= OS_NUM_MUTEX)	return -EINVAL;
	disable_interrupts();

	// check deadlock
	if (gtMutex[mutex].pHolding_Tcb == get_cur_tcb()){
		enable_interrupts();
		return -EDEADLOCK;
	}

	#ifdef DEBUG
	    printf("task %d is trying to get mutex\n", get_cur_tcb()->cur_prio);
	#endif

	// no one is holding the mutex
	if (gtMutex[mutex].bLock == FALSE){
		gtMutex[mutex].pHolding_Tcb = get_cur_tcb();
		gtMutex[mutex].bLock = TRUE;

		get_cur_tcb()->holds_lock++;

		#ifdef DEBUG
		    printf("task %d gets mutex\n", get_cur_tcb()->cur_prio);
		#endif
        
        //inherite the highest priority after get the mutex
        get_cur_tcb()->cur_prio = 0;
        
		enable_interrupts();
	} 

	// some one is holding the mutex
	// add current tcb into sleep queue and dispatch sleep
	else{
		if (gtMutex[mutex].pSleep_queue == 0){
			gtMutex[mutex].pSleep_queue = get_cur_tcb();
		} else{
			tcb_t *cur = gtMutex[mutex].pSleep_queue;
			while (cur->sleep_queue != 0)
				cur = cur->sleep_queue;
			cur->sleep_queue = get_cur_tcb();
		}

		#ifdef DEBUG
		    printf("task %d is waiting for mutex\n", get_cur_tcb()->cur_prio);
		#endif

		// switch to current highest priority    
		dispatch_sleep();
		enable_interrupts();
	}
	return 0;
}


/*  Function: mutex_unlock
 *  input: mutex number
 *  output: execuate state, if <0, error
 */
int mutex_unlock(int mutex  __attribute__((unused)))
{
	// error cases
	if (gtMutex[mutex].bAvailable == TRUE)	return -EINVAL;
	if (mutex < 0 || mutex >= OS_NUM_MUTEX)	return -EINVAL;
	disable_interrupts();
	if (gtMutex[mutex].pHolding_Tcb != get_cur_tcb()){
		enable_interrupts();
		return -EPERM;
	}

	// sleep queue is not empty, the first waiting task
	// will be woken up
	// the mutex will be handed over to this task
	if (gtMutex[mutex].pSleep_queue != 0){
		tcb_t *head = gtMutex[mutex].pSleep_queue;
		gtMutex[mutex].pSleep_queue = head->sleep_queue;
		head->sleep_queue = 0;
		gtMutex[mutex].pHolding_Tcb = head;
		get_cur_tcb()->holds_lock--;
		gtMutex[mutex].pHolding_Tcb->holds_lock++;
		runqueue_add(head, head->cur_prio);
	}
    
	// no one else is waiting for the mutex
	// release the mutex
	else{
		gtMutex[mutex].pHolding_Tcb = 0;
		gtMutex[mutex].bLock = 0;
		gtMutex[mutex].pSleep_queue = 0;
		get_cur_tcb()->holds_lock--;
	}
    
    // if it did not have any locks, return to its origin priority
    if (get_cur_tcb()->holds_lock == 0) {
        get_cur_tcb()->cur_prio = get_cur_tcb()->native_prio;
    }
    
	enable_interrupts();
	return 0;
}

