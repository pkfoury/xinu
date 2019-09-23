/* mutex.c */

#include <xinu.h>

sid32 mutex;
uint32 sum = 0;
uint32 lastpid = -1;

void mutex_init()
{
  mutex = semcreate(1);
}

void incrementer(
    uint32 val, // value to be added to sum
    uint32 iter // number of iterations
)
{
  int i;
  for (i = 0; i < iter; i++)
  {
    wait(mutex);
    sum = sum + val;
    lastpid = currpid;
    signal(mutex);
    sleepms(50);
  }
}

void monitor(void)
{
  while (1)
  {
    sleepms(800);
    wait(mutex);
    kprintf("sum=%d, lastpid=%d\n", sum, lastpid);
    signal(mutex);
  }
}