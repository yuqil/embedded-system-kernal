/*
 * Author: Yuqi Liu <yuqil@andrew.cmu.edu>
 *         Wei chen <wc2@andrew.cmu.edu>
 */

#include "main.h"
#include <exports.h>
#include <arm/psr.h>
#include <arm/exception.h>
#include <arm/interrupt.h>
#include <arm/timer.h>
#include "bits/swi.h"
#include "bits/fileno.h"
#include "bits/errno.h"
#include "arm/timer.h"
#include "arm/reg.h"
#include "arm/interrupt.h"
#include "address.h"

/* 
 * description: to wire in swi or irq handler
 * input: begin address, 0x8 for swi, 0x18 for irq
 *		  new handler
 *	      array pointer to store old contents of the memory address
 * return:
 */
void wireHandler(unsigned int* vector, void* new_handler, unsigned int* old_handler) {
    unsigned int jump_command = *(vector);
    
    // judge if it is ldr pc instruction
    if (((jump_command & 0xfffff000) != 0xe59ff000) && ((jump_command & 0xfffff000) != 0xe51ff000)) {
        puts("The SWI vector's instruction is unrecognized\n");
        return;
    }

    // calculate offset
    int sign = (jump_command & 0x00800000) >> 23;
    int offset = jump_command & 0x00000fff;
    if (sign == 0) offset *= -1;

    // calculate uboot swi handler
    unsigned int* jump_table = (unsigned int*)((char*)vector + 0x08 + offset);
    unsigned int* swi_handler = (unsigned int*) *(jump_table);

    //save old swi handler
    old_handler[0] = (unsigned int)swi_handler;
    old_handler[1] = *(swi_handler);
    old_handler[2] = *(swi_handler + 1);

    // hijack swi handler
    *(swi_handler) = 0xe51ff004;
    *(swi_handler + 1) = (unsigned)new_handler;
}
