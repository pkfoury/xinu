/*  main.c  - main */

#include <xinu.h>

volatile int debugmem = 0;

process main(void) {
  int i = 0, size = 8;
  debugmem = 1;
  mymeminit();

  for (i = 0; i < 20; i++) {
    if (myfreemem(mygetmem(size), size) == SYSERR) {
      uprintf("Failed to allocate / free memory \n");
    }
  }

  // char * res = mygetmem(size);
  // kprintf("initial getmem\n");

  // uint32 status = myfreemem(res, size);
  // kprintf("freemem %d\n", status);

  // res = mygetmem(size);
  // kprintf("getmem 0x%08X\n", res);


  return OK;
}
