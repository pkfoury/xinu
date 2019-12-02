// pipeopen.c - pipeopen

#include <xinu.h>

/*------------------------------------------------------------------------
 *  pipeopen  -  Reset the csem and psem; reset the buffer pointers
 *------------------------------------------------------------------------
 */

devcall pipeopen(
		struct dentry *devptr, /* Entry in device switch table	*/
		char *name,						 /* Unused for a pipe	*/
		char *mode						 /* Unused for a pipe	*/
)
{

	semreset(pipe.psem, 1);
	semreset(pipe.csem, 0);

	pipe.head = pipe.buff;
	pipe.tail = pipe.buff;

	return OK;
}