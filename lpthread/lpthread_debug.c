#include "lpthread.h"
// Debug print all queues
void lpthread_print_queues(){
    printf("\n----------Running Queue--------------\n");
    printQueue(&runningQueue);
    printf("\n---------AllThreads Queue------------\n");
    printQueue(&allThreads);
    printf("\n---------Current Thread--------------\n");
    printf("Thread: %ld\n",currentThread->tid);
    printf("\n---------Previous Thread-------------\n");
    printf("Thread: %ld\n\n\n",prevThread->tid);
}