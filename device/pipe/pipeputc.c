// pipeputc.c - pipeputc

#include <xinu.h>

/*------------------------------------------------------------------------
 *  pipeputc  -  Wait on psem; deposit a byte; signal csem
 *------------------------------------------------------------------------
 */

devcall pipeputc(
		struct dentry *devptr, /* Entry in device switch table	*/
		char *byte // byte to deposit
)
{
	return OK;
}