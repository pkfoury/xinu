/* myfreemem.c - myfreemem */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  myfreemem  -  cache recently freed blocks
 *------------------------------------------------------------------------
 */
syscall	myfreemem(
	  char		*blkaddr,	/* Pointer to memory block	*/
	  uint32	nbytes		/* Size of block in bytes	*/
	)
{
	intmask	mask;			/* Saved interrupt mask		*/
	struct	memblk	*next, *prev, *block;
	uint32	top;

	mask = disable();
	// error checking
	



	restore(mask);
	return OK;
}
