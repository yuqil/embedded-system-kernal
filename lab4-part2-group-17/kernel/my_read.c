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
 * description: system call read
 * input: file descriptor
 *		  buffer to store read data
 *	      read count
 * return: read bytes
 */
ssize_t my_read(int fd, void* buf, size_t count){
	unsigned int rd_cnt = 0;
	char *cbuf = (char*)buf;
	char ch;
	if (fd != STDIN_FILENO){
		printf("should read from stdin.\n");
		return -EBADF;
	}
	if ((unsigned int)buf < 0xa0000000 ||
		 (unsigned int)buf + count >= 0xa4000000){
		printf("memory out of range.\n");
		return -EFAULT;
	}
	while (rd_cnt < count){
		ch = getc();
		if (ch == 4){	// EOT
			return rd_cnt;
		} else if (ch == 8 || ch == 127){	// delete
			rd_cnt--;
			puts("\b \b");
		} else if (ch == 10 || ch == 13){	// \r or \n
			cbuf[rd_cnt++] = '\n';
			putc('\n');
			return rd_cnt;
		} else{
			cbuf[rd_cnt++] = ch;
			putc(ch);
		}
	}
	return rd_cnt;
}
