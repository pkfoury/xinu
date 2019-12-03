// pipeclose.c - pipeclose

#include <xinu.h>

/*------------------------------------------------------------------------
 *  pipeclose  -  Wait on psem; deposit EOF in the buffer; signal csem
 *------------------------------------------------------------------------
 */

devcall pipeclose(
	struct dentry *devptr /* Entry in device switch table	*/
)
{
	if(pipe.open == FALSE)
		return SYSERR;

	pipe.open = FALSE;

	wait(pipe.psem);
	pipe.buf[pipe.tail] = EOF; // place EOF on end of buffer
	signal(pipe.csem);

	return OK;
}