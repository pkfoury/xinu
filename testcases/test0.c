/*  test0.c  - main */

#include <xinu.h>

process	main(void)
{

	resume(create(type, INITSTK, INITPRIO, "Print A", 1, 'A'));
	resume(create(type, INITSTK, INITPRIO, "Print B", 1, 'B'));
	resume(create(type, INITSTK, INITPRIO, "Print C", 1, 'C'));
    
}
