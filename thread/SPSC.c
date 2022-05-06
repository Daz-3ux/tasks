#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>

// Single-producer , single-consumer Queue
struct SPSCQueue {
    int num;
    struct SPSCQueue *next;
}typedef SPSCQueue;

SPSCQueue *SPSCQueueInit(int capacity);//初始化函数
void SPSCQueuePush(SPSCQueue *queue, void *s);//入队
void *SPSCQueuePop(SPSCQueue *queue);//出队
void SPSCQueueDestory(SPSCQueue *queue);//反初始化
void *producer(void *arg);
void *consumer(void *arg);
void my_error(char *string, int line);

SPSCQueue *head, *tail;//队列头与尾
pthread_mutex_t mylock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t mycond = PTHREAD_COND_INITIALIZER;

int main(int argc, char *argv[])
{
    pthread_t pid;
    pthread_t cid;
    SPSCQueue *queue = NULL;
    head = SPSCQueueInit(1);//创建队列

    int check;
    if((check = pthread_create(&pid, NULL, producer, NULL)) != 0){
        my_error("creat error",__LINE__);
    }
    if((check = pthread_create(&cid, NULL, consumer, NULL)) != 0){
        my_error("creat error",__LINE__);
    }
    SPSCQueueInit(1);

    if((check = pthread_join(pid, NULL)) != 0){
        my_error("join error",__LINE__);
    }
    if((check = pthread_join(cid, NULL)) != 0){
        my_error("join error",__LINE__);
    }
    
    SPSCQueueDestory(queue);
}

void *producer(void *queue)
{
    int check;
    while(1){//一直生产
        if((check = pthread_mutex_lock(&mylock)) != 0){
            my_error("lock error",__LINE__);
        }
        SPSCQueuePush(head, (void*)(intptr_t)(rand() % 10));//x86_64架构转换一下指针长度
        if((check = pthread_mutex_unlock(&mylock)) != 0){
            my_error("unlock error",__LINE__);
        }

        if((check = pthread_cond_signal(&mycond)) != 0){//只用唤醒一个，用signal性能更高
            my_error("cond_signal error",__LINE__);
        }
        sleep(rand() % 3);
    }
    return NULL;
}

void *consumer(void *queue)
{
    while(1){//一直消费
        pthread_mutex_lock(&mylock);
        while(head -> next == NULL){
            pthread_cond_wait(&mycond, &mylock);
        }
        SPSCQueuePop(tail);
        pthread_mutex_unlock(&mylock);

        sleep(rand() % 6);
    }
    return NULL;
}

SPSCQueue *SPSCQueueInit(int capacity)
{
    head = (SPSCQueue*)malloc(sizeof(SPSCQueue) * capacity);
    if(head == NULL){
        my_error("malloc failed",__LINE__);
    }
    head -> next = NULL;
    tail = head;
    return head;
}

void SPSCQueuePush(SPSCQueue *queue, void *s)
{
    SPSCQueue *new = (SPSCQueue *)malloc(sizeof(SPSCQueue));
    if(new == NULL){
        my_error("malloc failed",__LINE__);
    }
    new -> num = (int)(intptr_t)(s);
    new -> next = NULL;
    printf("Producer: num = %d, tid = %ld\n", new -> num, pthread_self());
    tail -> next = new;
    tail = new;//尾插
}

void *SPSCQueuePop(SPSCQueue *queue)
{
    SPSCQueue *old = NULL;
    if(head -> next != NULL){
        if(head -> next == tail){
            tail = head;
        }
        old = head -> next;
        head -> next = head -> next -> next;//删除头节点，先进先出
        printf("consumer: num = %d, tid = %ld\n", old -> num, pthread_self());
        free(old);
    }
    return NULL;
}

void SPSCQueueDestory(SPSCQueue *queue)
{
    free(queue);
    int check;
    if((check = pthread_mutex_destroy(&mylock)) != 0){
        my_error("mutex destroy error",__LINE__);
    }
    if((check = pthread_cond_destroy(&mycond)) != 0){
        my_error("cond destroy error",__LINE__);
    }
}

void my_error(char *string, int line)
{//myerror("malloc", __LINE__);
    fprintf(stderr, "Line:%d,error:\n", line);
    fprintf(stderr, "%s:%s\n", string, strerror(errno));
}