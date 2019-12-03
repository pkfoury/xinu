// pipegetc.c - pipegetc

#include <xinu.h>

/*------------------------------------------------------------------------
 *  pipegetc  -  Wait on csem; extract a byte from the pipe; signal psem
 *------------------------------------------------------------------------
 */

int32 pipegetc(
		struct dentry *devptr /* Entry in device switch table	*/
)
{
	char ch; // character to return

	// if (pipe.open == FALSE) // if buffer not opened, throw error
	// 	return EOF;

	if (pipe.head >= PIPESIZE) { // wrap around buffer if needed
		pipe.head = 0;
	}

	wait(pipe.csem);
	ch = pipe.buf[pipe.head];
	pipe.head++;
	signal(pipe.psem);

	return ch;
}