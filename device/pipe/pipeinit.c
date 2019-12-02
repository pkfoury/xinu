// pipeinit.c - pipeinit

#include <xinu.h>

/*------------------------------------------------------------------------
 *  pipeinit  -  Initialize the data structure and create csem and psem
 *------------------------------------------------------------------------
 */

struct pipe pipe;

devcall pipeinit() {


	pipe.psem = semcreate(1);
	pipe.csem = semcreate(0);

	pipe.head = pipe.buf;
	pipe.tail = pipe.buf;

	pipe.open = FALSE;

	return OK;
}