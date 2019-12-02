// pipegetc.c - pipegetc

#include <xinu.h>

/*------------------------------------------------------------------------
 *  pipegetc  -  Wait on csem; extract a byte from the pipe; signal psem
 *------------------------------------------------------------------------
 */

devcall pipegetc(
		struct dentry *devptr /* Entry in device switch table	*/
)
{

	semreset(pipe.psem, 1);
	semreset(pipe.csem, 0);

	pipe.head = pipe.buff;
	pipe.tail = pipe.buff;

	return OK;
}