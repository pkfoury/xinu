/* limit.c - limit */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  limit  -  limit number of slices a process can consume
 *------------------------------------------------------------------------
 */
syscall	tlimit(
	  pid32		pid, // pid of process to limit
		uint32 limit // time limit
	)
{
	intmask		mask;		/* Saved interrupt mask		*/
	mask = disable();

	if (isbadpid(pid) || limit < 0) {
		restore(mask);
		return SYSERR;
	}

	if(pid == currpid) {
		limit += 1;
	}

	struct procent *prptr = &proctab[pid];
	prptr->nslices = limit;

	restore(mask);
	return OK;
}
