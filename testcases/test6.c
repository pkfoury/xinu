/*  test6.c  - main */

#include <xinu.h>

bool8 go = FALSE;

process	main(void)
{
	pid32 a, b, c;
	resume(a = create(print, INITSTK, 10, "Print A", 1, 'A'));
	resume(b = create(print, INITSTK, 10, "Print B", 1, 'B'));
	resume(c = create(print, INITSTK, 10, "Print C", 1, 'C'));
	resume(a);
	resume(b);
	resume(c);
	tlimit(a, 10);
	tlimit(b, 10);
	tlimit(c, 10);
  go = TRUE;

}
