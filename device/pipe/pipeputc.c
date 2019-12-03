// pipeputc.c - pipeputc

#include <xinu.h>

/*------------------------------------------------------------------------
 *  pipeputc  -  Wait on psem; deposit a byte; signal csem
 *------------------------------------------------------------------------
 */

devcall pipeputc(
		struct dentry *devptr, /* Entry in device switch table	*/
		char *ch // ch to deposit
)
{
	if(pipe.open == FALSE) // if buffer not opened, throw error
		return SYSERR;

	if(pipe.tail >= pipe.buf[NBYTES * 32] ) {
		pipe.tail = pipe.head;
	}

	wait(pipe.psem); // wait on producer sema
	*pipe.tail = ch;
	pipe.tail++;
	signal(pipe.csem); // signal consumer sema

	return OK;
}