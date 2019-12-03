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

	return OK;
}