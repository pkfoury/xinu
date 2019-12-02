// pipeopen.c - pipeopen

#include <xinu.h>

/*------------------------------------------------------------------------
 *  pipeopen  -  open pipe
 *------------------------------------------------------------------------
 */

devcall pipeopen(
		struct dentry *devptr, /* Entry in device switch table	*/
		char *name,						 /* Unused for a pipe	*/
		char *mode						 /* Unused for a pipe	*/
)
{

	return OK;
}