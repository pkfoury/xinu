/*  test2.c  - main */

#include <xinu.h>

process	main(void)
{

	resume(create(print, INITSTK, INITPRIO, "Print A", 1, 'A'));
	resume(create(print, INITSTK, INITPRIO, "Print B", 1, 'B'));
	resume(create(print, INITSTK, INITPRIO, "Print C", 1, 'C'));
    
}
