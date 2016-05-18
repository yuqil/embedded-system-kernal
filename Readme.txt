[Lab 4 Documentation]
Author:
Wei Chen<wc2@andrew.cmu.edu>
Yuqi Liu<yuqil@andrew.cmu.edu>

[ 1. UB TEST ]
(1) ub_test.c:
    Calculate cpu utilization and compare with ub bound. 

/* input: sorted tcb array, 
 * output 0:fail, 1:success
 */

int assign_schedule(task_t** tasks  __attribute__((unused)), size_t num_tasks  __attribute__((unused)))

(2) will return error code if cannot be scheduled.


[ 2. Mutex ]
(1) mutex.c
    mutex_lock: if one task acquires the lock, its priority increase to 0
    mutex_unlock: if the task releases the lock, return to its native priority

(2) will return error code if called dev_wait while holding lock


[ 3. Self-designed test cases ] 
(1) dagger: only two tasks, the UB test will fail (set B = T), so the program’s output should print “Hmmm”.
            This program tests our program can effectively handle unschedulable tasks case
(2) simple mutex: check lock logic, two tasks, one with T = 100 and priority of 1, another one with T=
                  200 and priority of 2. Task2 will sleep 120ms while holding the mutex. During this time, task2 
                  should not be interrupted by task1 while acquiring the lock because it has priority of 0.

    The output should be : 121 112 112 112 ….
    This program tests our highest priority inheritance protocol.
    
(3) stress: check for complicated context switch and scheduler logic with more tasks than dagger. Has same output as part 1.
(4) cyclone: check for context switch and scheduler with different sleep time. This has the same output as part 1.


