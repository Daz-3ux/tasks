#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>

struct SPSCQueue {
    void **my_queue;
    int capacity;
    int size;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
}typedef SPSCQueue;

SPSCQueue *SPSCQueueInit(int capacity);//初始化函数
void SPSCQueuePush(SPSCQueue *queue, void *s);//入队
void *SPSCQueuePop(SPSCQueue *queue);//出队SPSCQueue
void my_error(char *string, int line);

int main(int argc, char **argv)
{
    
}

SPSCQueue *SPSCQueueinit(int capacity)
{
    // 初始化结构体
    SPSCQueue *queue = (SPSCQueue *)malloc(sizeof(SPSCQueue));
    // 赋值
    queue->my_queue = (void**)malloc(capacity * sizeof(void*));
    queue->capacity = capacity;
    queue->size = 0;
    // 初始化锁与条件变量
    pthread_mutex_init(&queue->mutex, NULL);
    pthread_cond_init(&queue->cond, NULL);

    return queue;
}

void SPSCQueuePush(SPSCQueue *queue, void *s)
{   
    pthread_mutex_lock(&queue->mutex);
    // 还有剩余空间
    if(queue->size < queue->capacity){
        queue->my_queue[queue->size++] = s;
        pthread_mutex_unlock(&queue->mutex);
    }else{
        pthread_cond_signal(&queue->cond);
    }
}

void *SPSCQueuePop(SPSCQueue *queue){
    pthread_mutex_lock(&queue->mutex);
    while(queue->size == 0){
        //队列空
        pthread_cond_wait(&queue->cond, &queue->mutex);
    }
    queue->my_queue[--queue->size] = 0;
    //queue->size--;
    pthread_mutex_unlock(&queue->mutex);
    
    return queue;
}