/* send.c - send, sendblk */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  send  -  Pass a message to a process and start recipient if waiting
 *------------------------------------------------------------------------
 */
syscall	send(
	  pid32		pid,		/* ID of recipient process	*/
	  umsg32	msg		/* Contents of message		*/
	)
{
	intmask	mask;			/* Saved interrupt mask		*/
	struct	procent *prptr;		/* Ptr to process's table entry	*/

	mask = disable();
	if (isbadpid(pid)) {
		restore(mask);
		return SYSERR;
	}

	prptr = &proctab[pid];
	if (prptr->prhasmsg) {
		restore(mask);
		return SYSERR;
	}
	prptr->prmsg = msg;		/* Deliver message		*/
	prptr->prhasmsg = TRUE;		/* Indicate message is waiting	*/

	/* If recipient waiting or in timed-wait make it ready */

	if (prptr->prstate == PR_RECV) {
		ready(pid);
	} else if (prptr->prstate == PR_RECTIM) {
		unsleep(pid);
		ready(pid);
	}
	restore(mask);		/* Restore interrupts */
	return OK;
}

/*------------------------------------------------------------------------
 *  sendblk  -  Pass a message to a process and block until it is received
 *------------------------------------------------------------------------
 */
syscall sendblk(
		pid32 pid, /* ID of recipient process	*/
		umsg32 msg /* Contents of message		*/
)
{
	intmask mask;		/* Saved interrupt mask		*/
	struct procent *prptr; /* Ptr to process's table entry	*/
	mask = disable();

	if (isbadpid(pid)) {
		restore(mask);
		return SYSERR;
	}

	prptr = &proctab[pid];
	if(prptr->prhasmsg) {
		restore(mask);
		return SYSERR;
	}

	prptr->prmsg = msg;			/* Deliver message		*/
	prptr->prhasmsg = TRUE; /* Indicate message is waiting	*/

	if (prptr->prstate == PR_RECV) {
		ready(pid);
	} else if(prptr->prstate == PR_RECTIM) {
		unsleep(pid);
		ready(pid);
	} else {
		struct procent *senderproc = &proctab[currpid];
		prptr->blockedsender = currpid; // store pid of blocked sender
		prptr->hasblockedsender = TRUE;
		senderproc->prstate = PR_SENDING;
		resched();
	}

	restore(mask); // restore interrupts
	return OK;
}

/*------------------------------------------------------------------------
 *  sendcallback  -  Pass a message to a process using its callback function
 *------------------------------------------------------------------------
 */
syscall	sendcb(
		pid32	pid,		/* ID of recipient process	*/
		umsg32	msg,		/* Contents of message		*/
		uint32 (* cb) ()	/* Callback pointer			*/
	)
{
	intmask	mask;			/* Saved interrupt mask		*/
	struct	procent *prptr;		/* Ptr to process's table entry	*/

	mask = disable();
	if (isbadpid(pid)) {
		restore(mask);
		return SYSERR;
	}

	prptr = &proctab[pid];
	if (prptr->prhasmsg) {
		restore(mask);
		return SYSERR;
	}
	prptr->prmsg = msg;		/* Deliver message		*/
	prptr->prhasmsg = TRUE;		/* Indicate message is waiting	*/
	prptr->hascb = TRUE;
	prptr->cb = cb;

	/* If recipient waiting or in timed-wait make it ready */

	if (prptr->prstate == PR_RECV) {
		ready(pid);
	} else if (prptr->prstate == PR_RECTIM) {
		unsleep(pid);
		ready(pid);
	}
	restore(mask);		/* Restore interrupts */
	return OK;
}
