/* myfreemem.c - myfreemem */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  myfreemem  -  cache recently freed blocks
 *------------------------------------------------------------------------
 */
syscall myfreemem(char *blkaddr, /* Pointer to memory block	*/
                  uint32 nbytes  /* Size of block in bytes	*/
) {
  intmask mask; /* Saved interrupt mask		*/
  mask = disable();

  struct memblklist *list;
  struct memblk *freeblock;
  bool8 fallback = TRUE;

  if ((nbytes == 0) || ((uint32)blkaddr < (uint32)minheap) ||
      ((uint32)blkaddr > (uint32)maxheap)) {
    return SYSERR;
  }

  int i;
  for (i = 0; i < 20; i++) {
    list = &cache[i];
    if (list->blksize == nbytes || list->blksize == 0) {  // if exact list or empty list exists
      kprintf("inserting into cache\n");
      if (list->blksize == 0) {
        list->blksize = nbytes;
      }
      fallback = FALSE;
      freeblock = (struct memblk *)blkaddr;
      freeblock->mnext = list->head;
      list->head = freeblock;
      break;
    }
  }

  // if cache is full, fallback to freemem()
  if (fallback) {
    freemem(blkaddr, nbytes);
  }

  restore(mask);
  return OK;
}
