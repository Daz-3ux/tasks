#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX 10//缓冲区大小
#define DataType int

// Single-producer , single-consumer Queue
struct SPSCQueue {
    int front;
    int end;
    DataType data[MAX];
} typedef SPSCQueue;

SPSCQueue *SPSCQueueInit(int capacity);//初始化函数
void SPSCQueuePush(SPSCQueue *queue, void *s);//入队
void *SPSCQueuePop(SPSCQueue *queue);//出队
void SPSCQueueDestory(SPSCQueue *queue);//反初始化
void *producer(SPSCQueue *queue);
void *consumer(SPSCQueue *queue);

SPSCQueue *queue = NULL;
pthread_mutex_t mylock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t mycond = PTHREAD_COND_INITIALIZER;

int main(int argc, char *argv[])
{
    SPSCQueueInit(MAX);
}

void *producer(SPSCQueue *queue)
{

    return NULL;
}

void *consumer(SPSCQueue *queue)
{

    return NULL;
}

SPSCQueue *SPSCQueueInit(int capacity)
{
    queue = (SPSCQueue*)malloc(sizeof(SPSCQueue) * capacity);
    return queue;
}

void SPSCQueuePush(SPSCQueue *queue, void *s)
{

}

void *SPSCQueuePop(SPSCQueue *queue)
{

    return NULL;
}

void SPSCQueueDestory(SPSCQueue *queue)
{
    free(queue);
    int error;
    if ((error = pthread_mutex_destroy(&mylock)) != 0) {
        fprintf(stderr, "Failed to destroy mylock : %s\n", strerror(error));
    }
    if((error = pthread_cond_destroy(&mycond)) != 0){
        fprintf(stderr, "Failed to destroy mycond : %s\n", strerror(error));
    }
}