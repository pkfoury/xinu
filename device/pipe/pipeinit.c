// pipeinit.c - pipeinit

#include <xinu.h>

/*------------------------------------------------------------------------
 *  pipeinit  -  Initialize pipe data structure
 *------------------------------------------------------------------------
 */

devcall pipeinit() {

	pipe->buff = getmem(4);

	pipe->head = pipe->buff;
	pipe->tail = pipe->buff;

	pipe->psem = semcreate(1);
	pipe->csem = semcreate(0);

	return OK;
}