/*
 * main.h: declare functions used in kmain.c
 *
 * Author: Yuqi Liu <yuqil@andrew.cmu.edu>
 *         Wei chen <wc2@andrew.cmu.edu>
 */
#include "types.h"


/* 
 * description: to wire in swi or irq handler
 * input: begin address, 0x8 for swi, 0x18 for irq
 *		  new handler
 *	      array pointer to store old contents of the memory address
 * return:
 */
void wireHandler(unsigned int* vector, void* new_handler, unsigned int* old_handler);

/* 
 * description: recover the corrupted 8-byte address
 * input: array pointer
 * return:
 */
void recover(unsigned int* old_handler);


/* 
 * description: new swi handler
 * input: swi number
 *		  parameters for systems call
 *        process tima and sleep system call
 * return:
 */
void c_swi_handler(unsigned swi_num, unsigned *regs);


/* 
 * description: new irq handler
 * input:
 * return:
 */
void c_irq_handler();

/* 
 * description: init irq handler
 * input:
 * return:
 */
void init_irq();


/* 
 * description: init timer
 * input:
 * return:
 */
void init_timer();


/* 
 * description: system call read
 * input: file descriptor
 *		  buffer to store read data
 *	      read count
 * return: read bytes
 */
ssize_t my_read(int fd, void* buf, size_t count);


/* 
 * description: system call write
 * input: file descriptor
 *		  data to write
 *	      write count
 * return: write bytes
 */
ssize_t my_write(int fd, const void* buf, size_t count);

// assemble codes

// swi handler
void my_swi_handler();

// irq handler
void my_irq_handler();
void irq_wrapper();

// load user program and set stack
int ld_usr_pro(int argc, char* argv[]);

// exit
void exit(int status);

// enable irq
void enable_irq();

// set irq stack
void set_irq_stack(unsigned int pt);
