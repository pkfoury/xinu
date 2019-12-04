/* xsh_gen.c - xsh_gen */

#include <xinu.h>
#include <stdio.h>

/*------------------------------------------------------------------------
 * xsh_gen - generate N bytes
 *------------------------------------------------------------------------
 */
shellcmd xsh_gen(int nargs, char *args[])
{
	int32	i;			/* walks through args array	*/

	if ( nargs != 2 || args[1] < 0) {
		fprintf(stderr, "%s: invalid arguments\n", args[0]);
		return 1;
	}

	int32 n = atoi(args[1]);
	for (i = 0; i < n; i++) {
		putc(stdout, 'c');
	}
	putc(stdout, '\n');
	close(stdout);

	return 0;
}
