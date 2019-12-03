// piperead.c - piperead

#include <xinu.h>

/*------------------------------------------------------------------------
 *  piperead  -  Wait on csem; read one byte; signal psem
 *------------------------------------------------------------------------
 */

devcall piperead(
		struct dentry *devptr, /* Entry in device switch table	*/
		char* byte // byte to read
)
{

	// call pipereadc N times

	return OK;
}