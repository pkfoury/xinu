/* mymeminit.c - mymeminit */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  mymeminit  -  initialize memory cache
 *------------------------------------------------------------------------
 */

void mymeminit(void)
{
	int i;
	for (i = 0; i < 20; i++)
	{
		cache[i].size = 0;
		cache[i].list = (struct blocklist *)NULL;
	}
	return;
}
