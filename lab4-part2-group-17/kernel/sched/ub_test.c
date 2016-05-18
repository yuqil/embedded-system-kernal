/** @file ub_test.c
 * 
 * @brief The UB Test for basic schedulability
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date 2008-11-20
 */

//#define DEBUG 0

#include <sched.h>
#ifdef DEBUG
#include <exports.h>
#endif

 
/**
 * @brief Perform UB Test and reorder the task list.
 *
 * The task list at the end of this method will be sorted in order is priority
 * -- from highest priority (lowest priority number) to lowest priority
 * (highest priority number).
 *
 * @param tasks  An array of task pointers containing the task set to schedule.
 * @param num_tasks  The number of tasks in the array.
 *
 * @return 0  The test failed.
 * @return 1  Test succeeded.  The tasks are now in order.
 */

 // UB bound array for 0 - 64
double bound[64]= { 1.0, 0.828, 0.779, 0.756, 0.743, 0.734, 0.728, 0.724,  
	0.720, 0.718, 0.715, 0.714, 0.712, 0.711, 0.709, 0.708,
	0.707, 0.707, 0.706, 0.705, 0.705, 0.704, 0.704, 0.703,
    0.703, 0.702, 0.702, 0.702, 0.701, 0.701, 0.701, 0.701,
    0.700, 0.700, 0.700, 0.700, 0.700, 0.700, 0.699, 0.699,
    0.699, 0.699, 0.699, 0.699, 0.699, 0.698, 0.698, 0.698,
    0.698, 0.698, 0.698, 0.698, 0.698, 0.698, 0.698, 0.697, 
    0.697, 0.697, 0.697, 0.697, 0.697, 0.697, 0.697, 0.697
};


/* input: sorted tcb array, 
 * output 0:fail, 1:success
 */
int assign_schedule(task_t** tasks  __attribute__((unused)), size_t num_tasks  __attribute__((unused)))
{
	double u_k = bound[num_tasks - 1];
	double result = 0.0;

	// calculate Sum(Ci / Ti) i = 1, 2... k -1
	unsigned i = 0;
	for (i = 0; i < num_tasks - 1; i ++) {
		result += ((double) (* tasks)[i].C / (double) (* tasks)[i].T);
	}

	// calculate (Ck + Bk) / Tk
	result += (((double) (* tasks)[i].C + (double) (* tasks)[i].B) / (double) (* tasks)[i].T);

	if (result > u_k) return 0;

	return 1; 
}
	


