// piperead.c - piperead

#include <xinu.h>

/*------------------------------------------------------------------------
 *  piperead  -  Wait on csem; read one byte; signal psem
 *------------------------------------------------------------------------
 */

devcall piperead(
		struct dentry *devptr, /* Entry in device switch table	*/
		int32 *buf, // byte to read
		int32 count // number of chars to read
)
{

	// call pipereadc count times
	int i = 0;
	for(; i < count; i++) {
		*buf++ = pipegetc(devptr);
	}

	return buf;
}