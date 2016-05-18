/**
 * @file device.c
 *
 * @brief Implements simulated devices.
 * @author Wei Chen <wc2@andrew.cmu.edu>, Yuqi Liu<yuqil@andrew.cmu.edu>
 * @date 2015.12.2
 */

#include <types.h>
#include <assert.h>

#include <task.h>
#include <sched.h>
#include <device.h>
#include <arm/reg.h>
#include <arm/psr.h>
#include <arm/exception.h>

/**
 * @brief Fake device maintainence structure.
 * Since our tasks are periodic, we can represent 
 * tasks with logical devices. 
 * These logical devices should be signalled periodically 
 * so that you can instantiate a new job every time period.
 * Devices are signaled by calling dev_update 
 * on every timer interrupt. In dev_update check if it is 
 * time to create a tasks new job. If so, make the task runnable.
 * There is a wait queue for every device which contains the tcbs of
 * all tasks waiting on the device event to occur.
 */

struct dev
{
	tcb_t* sleep_queue;
	unsigned long   next_match;
};
typedef struct dev dev_t;

/* devices will be periodically signaled at the following frequencies */
const unsigned long dev_freq[NUM_DEVICES] = { 100, 200, 300, 400};


static dev_t devices[NUM_DEVICES];

/**
 * @brief Initialize the sleep queues and match values for all devices.
 */
void dev_init(void)
{
	int i = 0;
	for (i = 0; i < NUM_DEVICES; i++){
		devices[i].sleep_queue = 0;
		devices[i].next_match = dev_freq[i];
	}
}


/**
 * @brief Puts a task to sleep on the sleep queue until the next
 * event is signalled for the device.
 *
 * @param dev  Device number.
 */
void dev_wait(unsigned int dev __attribute__((unused)))
{
	disable_interrupts();
	tcb_t* prev_executed_tcb = get_cur_tcb();
	#ifdef DEBUG
	     printf("in dev_wait: finish excuting, call dispatch_sleep\n");
	#endif
    
	// add to sleepqueue
	if (devices[dev].sleep_queue == 0){
		devices[dev].sleep_queue = prev_executed_tcb;
	} else{
		prev_executed_tcb->sleep_queue = devices[dev].sleep_queue;
		devices[dev].sleep_queue = prev_executed_tcb;
	}

	// switch to the highest priority task in runqueue
	dispatch_sleep();
	enable_interrupts();
}


/**
 * @brief Signals the occurrence of an event on all applicable devices. 
 * This function should be called on timer interrupts to determine that 
 * the interrupt corresponds to the event frequency of a device. If the 
 * interrupt corresponded to the interrupt frequency of a device, this 
 * function should ensure that the task is made ready to run 
 */
void dev_update(unsigned long millis __attribute__((unused)))
{
	int i = 0;
	disable_interrupts();
	for (i = 0; i < NUM_DEVICES; i++){
		// if the job is woke up, add to run queue
		if (millis == devices[i].next_match){
			tcb_t* woke_tcb = devices[i].sleep_queue;
			while (woke_tcb != 0){
				// make task ready to run
				runqueue_add(woke_tcb, woke_tcb->cur_prio);
				woke_tcb = woke_tcb->sleep_queue;
			}
			devices[i].sleep_queue = 0;

			// update next-match
			devices[i].next_match = millis + dev_freq[i];
		} 
	}

	// check there is higher priority task, if so switch and save context
	dispatch_save();
	enable_interrupts();
}

