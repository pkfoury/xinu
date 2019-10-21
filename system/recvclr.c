/* recvclr.c - recvclr */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  recvclr  -  Clear incoming message, and return message if one waiting
 *------------------------------------------------------------------------
 */
umsg32	recvclr(void)
{
	intmask	mask;			/* Saved interrupt mask		*/
	struct	procent *prptr;		/* Ptr to process's table entry	*/
	umsg32	msg;			/* Message to return		*/

	mask = disable();
	prptr = &proctab[currpid];
	if (prptr->prhasmsg == TRUE) {
		msg = prptr->prmsg;	/* Retrieve message		*/
		prptr->prhasmsg = FALSE;/* Reset message flag		*/
	} else {
		msg = OK;
	}

	if(prptr->hasblockedsender == TRUE) {
		prptr->hasblockedsender = FALSE; // reset blocking send for receiving process
		pid32 sender = prptr->blockedsender; // get pid of blocked sender
		ready(sender);
	}

	restore(mask);
	return msg;
}
