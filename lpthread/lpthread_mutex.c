#include "lpthread.h"
#include <stdlib.h>
#include <stdio.h>
void destroy(struct lpthread_mutex_t  *);

/* initial the mutex lock */
int lpthread_mutex_init(lpthread_mutex_t *mutex, const lpthread_mutexattr_t *mutexattr)
{
  printf("Inicializando Mutex \n");
  operationInProgress = 1;
  lpthread_mutex_t m = *mutex;
  
  m.initialized = 1;
  m.locked = 0;
  m.exist = 1;
  m.holder = -1; //holder represents the tid of the thread that is currently holding the mutex
  m.queue = malloc(sizeof(head_t*));;
  initQueue(m.queue);

  *mutex = m;
  operationInProgress = 0;
  //printf("Mutex initialized !\n");
  return 0;
};

/* aquire the mutex lock */
int lpthread_mutex_lock(lpthread_mutex_t *mutex)
{
    lpthread_mutex_t m = *mutex;
    int alive = m.exist;
    if(!alive || !mutex){
    
    printf("Lock: Mutex no existente: Violación de segmento (`core' generado) \n");
     
    kill(getpid(),SIGINT);
     
  }
  
  //printf("Mutex lock !\n");
  if(!mainContextInitialized)
  {
    initializeGarbageContext();
    initializeMainContext();
  }
  if(!mutex->initialized)
  {return -1;}

  unsigned long long now = ticks;
  //printf("now: %lld\n",now);
  while(ticks < now + 2){
    //printf("ticks: %lld\n",ticks);
    //printf("%d\n",ticks < now + 2);
    //my_sleep(1);
  }
  //printf("FInally\n");
  operationInProgress = 1;

  
  //printf("Lock status: %d\n",mutex->locked);
 // Atomic test and set the lock
  while(__atomic_test_and_set((volatile void *)&mutex->locked,__ATOMIC_RELAXED))
  {
    //printf("Mutex is locked!\n");
    operationInProgress = 1;
    enqueue(mutex->queue, currentThread);
    currentThread->status = MUTEX_WAIT;
    //we need to set notFinished to zero before going to scheduler
    operationInProgress = 0;
    //printf("RAISE Mutex is locked!\n");
    raise(SIGUSR2);
    //printf("RAISED Mutex is locked!\n");
  }

  if(!mutex->initialized)
  {
    mutex->locked = 0;
    return -1;
  }

  mutex->holder = currentThread->tid;
  
  operationInProgress = 0;
  //printf("Mutex locked !\n");

  printf("Mutex Bloqueado con exito \n");
  return 0;
};

/* try aquire the mutex lock */
int lpthread_mutex_trylock(lpthread_mutex_t *mutex)
{

  lpthread_mutex_t m = *mutex;
    int alive = m.exist;

    if(!alive){
    
    printf("Trylock: Mutex no existente: Violación de segmento (`core' generado)  \n");
     
    kill(getpid(),SIGINT);
     
  }
  
  

  //printf("Mutex lock !\n");
  if(!mainContextInitialized)
  {
    initializeGarbageContext();
    initializeMainContext();
  }
  if(!mutex->initialized)
  {return -1;}

  unsigned long long now = ticks;
  //printf("now: %lld\n",now);
  while(ticks < now + 2){
    //printf("ticks: %lld\n",ticks);
    //printf("%d\n",ticks < now + 2);
    //my_sleep(1);
  }
  //printf("FInally\n");
  operationInProgress = 1;

  
  //printf("Lock status: %d\n",mutex->locked);
  // If not acquired the lock then return
  if (__atomic_test_and_set((volatile void *)&mutex->locked,__ATOMIC_RELAXED))
  {
    operationInProgress = 0;
    return 1;
  }

  if(!mutex->initialized)
  {
    mutex->locked = 0;
    return -1;
  }

  mutex->holder = currentThread->tid;
  
  operationInProgress = 0;
  //printf("Mutex locked !\n");
  return 0;
   printf("Mutex tratado de bloquear con exito \n");
};

/* release the mutex lock */
int lpthread_mutex_unlock(lpthread_mutex_t *mutex)
{
  lpthread_mutex_t m = *mutex;
    int alive = m.exist;
  
    if(!alive){
    
    printf("Unlock: Mutex no existente: Violación de segmento (`core' generado)  \n");
     
    kill(getpid(),SIGINT);
     
  }
  
  //printf("Mutex unlock !\n");
  if(!mainContextInitialized)
  {
    initializeGarbageContext();
    initializeMainContext();
  }
  
  operationInProgress = 1;

  if(!mutex->initialized || !mutex->locked || mutex->holder != currentThread->tid)
  {return -1;}

  mutex->locked = 0;
  mutex->holder = -1;

  tcb* muThread;
  

  while(!TAILQ_EMPTY(mutex->queue))
  {
    muThread = dequeue(mutex->queue);
    if(muThread != NULL) 
      enqueue(&runningQueue, muThread);
  }
  operationInProgress = 0;
  
  //printf("Mutex unlocked !\n");
 printf("Mutex desbloqueado con exito \n");
  return 0;
  
};

/* destroy the mutex */
int lpthread_mutex_destroy(lpthread_mutex_t *mutex)
{
  operationInProgress = 1;
  lpthread_mutex_t m = *mutex;
  //Prevent threads from accessing while mutex is in destruction process
  m.initialized = 0;
  operationInProgress = 0;
  m.exist = 0;
  //If mutex still locked, wait for thread to release lock
  while(m.locked)
  {
    printf("Mutex still locked while removing!\n");
    raise(SIGUSR2);
    exit(1);
  }

  tcb *muThread;
  while(!TAILQ_EMPTY(m.queue))
  {
    muThread = dequeue(m.queue);
    if(muThread != NULL) 
      enqueue(&runningQueue, muThread);
  }
  *mutex = m;
  printf("Mutex destruido \n");
  sleep(0.1);
  return 0;
  //se libera la memoria
   
  
};

