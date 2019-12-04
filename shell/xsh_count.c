/* xsh_count.c - xsh_count */

#include <xinu.h>
#include <stdio.h>

/*------------------------------------------------------------------------
 * xhs_count - count bytes from stdin
 *------------------------------------------------------------------------
 */
shellcmd xsh_count(int nargs, char *args[])
{
	int32	i;			/* walks through args array	*/

	if ( nargs != 1 ) {
		fprintf(stderr, "%s: invalid arguments\n", args[0]);
		return 1;
	}

	


	return 0;
}
