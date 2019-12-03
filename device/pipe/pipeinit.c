// pipeinit.c - pipeinit

#include <xinu.h>

/*------------------------------------------------------------------------
 *  pipeinit  -  Initialize the data structure and create csem and psem
 *------------------------------------------------------------------------
 */

struct pipe pipe;

devcall pipeinit() {

	pipe.psem = semcreate(PIPESIZE);
	pipe.csem = semcreate(0);

	pipe.head = 0;
	pipe.tail = 0;

	pipe.open = FALSE;

	return OK;
}