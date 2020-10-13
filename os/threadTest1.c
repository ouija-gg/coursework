// ThreadTest1.c
// this shows one thread counting up and one counting down

#include <stdio.h>
#include <pthread.h>

int count = 0;

void *start_thread_one()
{
   int i;
   for (i=1; i<50; i++)  {
      count = count + 1;
      printf("Value of count in thread one is:%d\n", count);
   }
   return NULL;
}

void *start_thread_two()
{
   int i;
   for (i=1; i<50; i++)  {
      count = count - 1;
      printf("Value of count in thread two is:%d\n", count);
   }
   return NULL;
}

int main ()
{
 
  pthread_t one, two;

  pthread_create(&one, NULL, &start_thread_one, NULL);
  pthread_create(&two, NULL, &start_thread_two, NULL);

  pthread_join(one, NULL);
  pthread_join(two, NULL);

  return(0);

}

