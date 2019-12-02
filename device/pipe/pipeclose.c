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
	return OK;
}