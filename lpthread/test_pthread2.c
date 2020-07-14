#include <stdio.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>


//#include "lpthread.h"
//#ifndef LPTHREAD

//#endif



void *functionC();
pthread_mutex_t mutex1 ;
int  counter = 0;

void *functionC()
{
   int i, n;
   time_t t;

   /* Intializes random number generator */
   srand((unsigned) time(&t));

   
   n = (rand() % 10 ) + 1;
   

   pthread_mutex_lock( &mutex1 );
   for (size_t i = 0; i < n; i++)
   {
      
      counter++;
      printf("Mi contador es: %d en la i: %ld de n: %d y mi id: %li \n" , counter,i,n,pthread_self());
      sleep(0.5);

   }
   
   pthread_mutex_unlock( &mutex1);
   pthread_mutex_destroy (&mutex1);
}

 int main()
{
   int rc1, rc2,rc3,rc4,rc5,rc6,rc7,rc8;
   pthread_t thread1,thread2,thread3,thread4,thread5,thread6,thread7,thread8;
   pthread_mutex_init(&mutex1,NULL);
   /* Create independent threads each of which will execute functionC */

   if( (rc1=pthread_create( &thread1, NULL, &functionC, NULL)) )
   {
      printf("Thread creation failed: %d\n", rc1);
   }

   if( (rc2=pthread_create( &thread2, NULL, &functionC, NULL)) )
   {
      printf("Thread creation failed: %d\n", rc2);
   }

   if( (rc3=pthread_create( &thread3, NULL, &functionC, NULL)) )
   {
      printf("Thread creation failed: %d\n", rc3);
   }

   if( (rc4=pthread_create( &thread4, NULL, &functionC, NULL)) )
   {
      printf("Thread creation failed: %d\n", rc4);
 
   }
   if( (rc5=pthread_create( &thread5, NULL, &functionC, NULL)) )
   {
      printf("Thread creation failed: %d\n", rc5);
   }

   if( (rc6=pthread_create( &thread6, NULL, &functionC, NULL)) )
   {
      printf("Thread creation failed: %d\n", rc6);
   }

   if( (rc7=pthread_create( &thread7, NULL, &functionC, NULL)) )
   {
      printf("Thread creation failed: %d\n", rc7);
   }

   if( (rc8=pthread_create( &thread8, NULL, &functionC, NULL)) )
   {
      printf("Thread creation failed: %d\n", rc8);

   }
   /* Wait till threads are complete before main continues. Unless we  */
   /* wait we run the risk of executing an exit which will terminate   */
   /* the process and all threads before the threads have completed.   */

   pthread_join( thread1, NULL);
   pthread_join( thread2, NULL); 
   pthread_join( thread3, NULL);
   pthread_join( thread4, NULL); 
   pthread_join( thread5, NULL);
   pthread_join( thread6, NULL); 
   pthread_join( thread7, NULL);
   pthread_join( thread8, NULL);


   printf("Counter value: %d\n",counter);
   exit(0);
 
}

