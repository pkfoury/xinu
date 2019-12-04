/* xsh_count.c - xsh_count */

#include <xinu.h>
#include <stdio.h>

/*------------------------------------------------------------------------
 * xhs_count - count bytes from stdin
 *------------------------------------------------------------------------
 */
shellcmd xsh_count(int nargs, char *args[])
{
	if (nargs != 1) {
		fprintf(stderr, "%s: invalid arguments\n", args[0]);
		return 1;
	}

	int32 counter = 0;
	while ( getc(stdin) != (char)EOF) {
		counter++;
	}
	fprintf(stdout, "%d bytes were received\n", counter); 

	return 0;
}
