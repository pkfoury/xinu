/* mygetmem.c - mygetmem */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  mygetmem  -  get memory blocks from cache
 *------------------------------------------------------------------------
 */
char *mygetmem(uint32 nbytes  // size of memory to get
) {
  intmask mask; /* Saved interrupt mask		*/
  mask = disable();

  struct memblklist *list;
  struct memblk *block;

  int i;
  for (i = 0; i < 20; i++) {
    list = &cache[i];
    if (list->blksize == nbytes) {  // if list exists, get from list
      kprintf("list found\n");
      block = list->head;
      list->head = block->mnext;
      block->mnext = (struct memblk *)NULL;
      restore(mask);
      return (char *)block;
    }
  }

  // if not in cache, fallback to getmem()
  char *mem = getmem(nbytes);

  // if getmem() == SYSERR, clear cache and try again.
  if (mem == (char *)SYSERR) {
    kprintf("getmem SYSERR\n");
    for (i = 0; i < 20; i++) {
      list = &cache[i];
      list->blksize = 0;
      block = list->head;
      while (block) {
        freemem((char *)block, block->mlength);
        block = block->mnext;
      }
    }
    mem = getmem(nbytes);
  }

  restore(mask);
  return mem;
}
