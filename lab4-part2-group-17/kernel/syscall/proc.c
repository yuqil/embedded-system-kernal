/** @file proc.c
 * 
 * @brief Implementation of `process' syscalls
 *
 * @author Mike Kasick <mkasick@andrew.cmu.edu>
 * @date   Sun, 14 Oct 2007 00:07:38 -0400
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date 2008-11-12
 */

#include <exports.h>
#include <bits/errno.h>
#include <config.h>
#include <kernel.h>
#include <syscall.h>
#include <sched.h>
#include <task.h>
#include <arm/reg.h>
#include <arm/psr.h>
#include <arm/exception.h>
#include <arm/physmem.h>
#include <device.h>

int task_create(task_t* tasks  __attribute__((unused)), size_t num_tasks  __attribute__((unused)))
{
	#ifdef DEBUG
	    printf("in task_create: creating tasks.\n");
	    printf("func2:%04x\n", tasks[1].lambda);
	#endif

	disable_interrupts();
	if (num_tasks >= OS_AVAIL_TASKS){
		enable_interrupts();
		return -EINVAL;
	}
	if (!valid_addr((void*)tasks, num_tasks, USR_START_ADDR, USR_END_ADDR)){
		enable_interrupts();
		return -EFAULT;
	}
	runqueue_init();           // initialize runqueue
	dev_init();                // initialize device sleeping queue
	task_t idle_task;          // create idle task
	sched_init(&idle_task);    // initial idle task
	sort_tasks(tasks, num_tasks);        // sort task according to priority

	// see if it can be scheduled
	if (assign_schedule(&tasks, num_tasks) == 0) {
		return -ESCHED;
	}

	allocate_tasks(&tasks, num_tasks);   // allocate tasks
	dispatch_nosave();                   // context switch to highest task
	return 0;
}

/* event_wait function, called after executation */
int event_wait(unsigned int dev  __attribute__((unused)))
{
	if (dev >= NUM_DEVICES){
		return -EINVAL;
	}
    if (get_cur_tcb()->holds_lock > 0) {
        return -EHOLDSLOCK;
    }
	dev_wait(dev);
	return 0;
}

/* An invalid syscall causes the kernel to exit. */
void invalid_syscall(unsigned int call_num  __attribute__((unused)))
{
	printf("Kernel panic: invalid syscall -- 0x%08x\n", call_num);

	disable_interrupts();
	while(1);
}
