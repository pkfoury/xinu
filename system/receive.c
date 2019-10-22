/* receive.c - receive */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  receive  -  Wait for a message and return the message to the caller
 *------------------------------------------------------------------------
 */
umsg32	receive(void)
{
	intmask	mask;			/* Saved interrupt mask		*/
	struct	procent *prptr;		/* Ptr to process's table entry	*/
	umsg32	msg;			/* Message to return		*/

	mask = disable();
	prptr = &proctab[currpid];
	if (prptr->prhasmsg == FALSE) {
		prptr->prstate = PR_RECV;
		resched();		/* Block until message arrives	*/
	}
	msg = prptr->prmsg;		/* Retrieve message		*/
	prptr->prhasmsg = FALSE;	/* Reset message flag		*/
	
	if(prptr->hasblockedsender == TRUE) {
		prptr->hasblockedsender = FALSE; // reset blocking send for receiving process
		pid32 sender = prptr->blockedsender; // get pid of blocked sender
		ready(sender);
	}

	if(prptr->hascb == TRUE) {
		if(prptr->cb(msg) != OK) {
			restore(mask);
			return SYSERR;
		};
	}

	restore(mask);
	return msg;
}

/*------------------------------------------------------------------------
 *  receivecallback - save a callback function to be used by senders
 *------------------------------------------------------------------------
 */
syscall	receivecallback(
		callback c
	)
{
	return OK;
}
