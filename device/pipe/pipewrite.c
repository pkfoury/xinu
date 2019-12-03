// pipewrite.c - pipewrite

#include <xinu.h>

/*------------------------------------------------------------------------
 *  pipewrite  -  Wait on psem; deposit a byte; signal csem
 *------------------------------------------------------------------------
 */

devcall pipewrite(
		struct dentry *devptr, /* Entry in device switch table	*/
		char *buf,						 // byte to deposit
		int32 count // number of chars to write
)
{

	// call pipereadc count times
	int i = 0;
	for(; i < count; i++) {
		pipeputc(devptr, *buf++);
	}

	return buf;
}