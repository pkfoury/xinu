/* printer.c -  type, kerneltype, print, kernelprint */

#include <xinu.h>

/*------------------------------------------------------------------------
 * type - print char endlessly
 *------------------------------------------------------------------------
 */
void type(
		char c /* character input */
)
{
	while (go == FALSE)
	{
		kprintf("Busy watiting\n");
	}
	while (1)
		putc(stdout, c);
}

/*------------------------------------------------------------------------
 * kerneltype - print char endlessly
 *------------------------------------------------------------------------
 */
void kerneltype(
		char c /* character input */
)
{
	while (go == FALSE)
	{
		kprintf("Busy watiting\n");
	}
	while (1)
		kputc(c);
}

/*------------------------------------------------------------------------
 * print - print character endlessly
 *------------------------------------------------------------------------
 */
void print(
		char c /* character input */
)
{
	while (go == FALSE)
	{
		kprintf("Busy watiting\n");
	}
	char str[50];
	int i;
	for (i = 0; i < 50; i++)
	{
		str[i] = c;
	}
	str[i] = '\0';
	while (1)
		uprintf("%s", str);
}

/*------------------------------------------------------------------------
 * kernelprint - print character endlessly
 *------------------------------------------------------------------------
 */
void kernelprint(
		char c /* character input */
)
{
	while (go == FALSE)
	{
		kprintf("Busy watiting\n");
	}
	char str[50];
	int i;
	for (i = 0; i < 50; i++)
	{
		str[i] = c;
	}
	str[i] = '\0';
	while (1)
		kprintf("%s", str);
}
