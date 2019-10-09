/* mymeminit.c - mymeminit */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  mymeminit  -  initialize memory cache
 *------------------------------------------------------------------------
 */

struct memblklist cache[20];

void mymeminit(void) {
  int i;
  for (i = 0; i < 20; i++) {
    cache[i].blksize = 0;
    cache[i].head = (struct memblk *)NULL;
  }

  return;
}
