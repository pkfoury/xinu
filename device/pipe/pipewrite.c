// pipewrite.c - pipewrite

#include <xinu.h>

/*------------------------------------------------------------------------
 *  pipewrite  -  Wait on psem; deposit a byte; signal csem
 *------------------------------------------------------------------------
 */

devcall pipewrite(
		struct dentry *devptr, /* Entry in device switch table	*/
		char *byte						 // byte to deposit
)
{

	// call pipeputc N times

	return OK;
}