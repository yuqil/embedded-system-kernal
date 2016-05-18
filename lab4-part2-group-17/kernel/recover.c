/*
 * Author: Yuqi Liu <yuqil@andrew.cmu.edu>
 *         Wei chen <wc2@andrew.cmu.edu>
 */

 #include "main.h"


/* 
 * description: recover the corrupted 8-byte address
 * input: array pointer
 * return:
 */
void recover(unsigned int* old_handler) {
	unsigned* old_handler_addr = ((unsigned*)(*old_handler));
	*(old_handler_addr) = *(old_handler + 1);
	*(old_handler_addr + 1) = *(old_handler + 2);
}
