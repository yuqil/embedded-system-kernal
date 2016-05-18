/*
 * Author: Yuqi Liu <yuqil@andrew.cmu.edu>
 *         Wei chen <wc2@andrew.cmu.edu>
 */
#include <exports.h>
#include "bits/swi.h"
#include "bits/fileno.h"
#include "bits/errno.h"
#include "main.h"


/* 
 * description: system call write
 * input: file descriptor
 *		  data to write
 *	      write count
 * return: write bytes
 */
ssize_t my_write(int fd, const void* buf, size_t count){
	char *cbuf = (char*)buf;
	unsigned wr_cnt = 0;
	if (fd != STDOUT_FILENO){
		printf("should write to std out.\n");
		return -EBADF;
	}
	if ((unsigned int)buf < 0xa0000000 ||
		 (unsigned int)buf + count >= 0xa4000000){
		printf("memory out of range.\n");
		return -EFAULT;
	}
	while (wr_cnt < count){
		if (cbuf[wr_cnt] == 0){
			return wr_cnt;
		} else{
			putc(cbuf[wr_cnt++]);
		}
	}
	return wr_cnt;
}
