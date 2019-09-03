/*  test1.c  - main */

#include <xinu.h>

process	main(void)
{

	resume(create(kerneltype, INITSTK, INITPRIO, "Print A", 1, 'A'));
	resume(create(kerneltype, INITSTK, INITPRIO, "Print B", 1, 'B'));
	resume(create(kerneltype, INITSTK, INITPRIO, "Print C", 1, 'C'));
    
}
