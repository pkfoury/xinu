// pipeputc.c - pipeputc

#include <xinu.h>

/*------------------------------------------------------------------------
 *  pipeputc  -  Wait on psem; deposit a byte; signal csem
 *------------------------------------------------------------------------
 */

devcall pipeputc(
		struct dentry *devptr, /* Entry in device switch table	*/
		char ch // ch to deposit
)
{
	if(pipe.open == FALSE) // if buffer not opened, throw error
		return SYSERR;

	if(pipe.tail >= PIPESIZE ) {
		pipe.tail = 0;
	}

	wait(pipe.psem); // wait on producer sema
	pipe.buf[pipe.tail] = (int32)ch;
	pipe.tail++;
	signal(pipe.csem); // signal consumer sema

	return OK;
}