#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>

// Single-producer , single-consumer Queue
struct MPMCQueue {
    int num;
    struct MPMCQueue *next;
}typedef MPMCQueue;

MPMCQueue *MPMCQueueInit(int capacity);//初始化函数
void MPMCQueuePush(MPMCQueue *queue, void *s);//入队
void *MPMCQueuePop(MPMCQueue *queue);//出队
void MPMCQueueDestory(MPMCQueue *queue);//反初始化
void *producer(void *arg);
void *consumer(void *arg);
void my_error(char *string, int line);

MPMCQueue *head, *tail;//队列头与尾
pthread_mutex_t mylock;
pthread_cond_t mycond;

#define MULTI 6//多线程个数

int main(int argc, char *argv[])
{
    pthread_t pid[MULTI];
    pthread_t cid[MULTI];
    MPMCQueue *queue = NULL;
    head = MPMCQueueInit(1);//创建队列

    int check;
    for(int i = 0; i < MULTI; i++){
        if((check = pthread_create(&pid[i], NULL, producer, NULL)) != 0){
            my_error("creat error",__LINE__);
            }
        if((check = pthread_create(&cid[i], NULL, consumer, NULL)) != 0){
            my_error("creat error",__LINE__);
        }
    }
    MPMCQueueInit(1);

    for(int i = 0;i < MULTI; i++){
        if((check = pthread_join(pid[i], NULL)) != 0){
            my_error("join error",__LINE__);
        }
        if((check = pthread_join(cid[i], NULL)) != 0){
            my_error("join error",__LINE__);
        }
    }
    MPMCQueueDestory(queue);
}

void *producer(void *queue)
{
    int check;
    while(1){//一直生产
        if((check = pthread_mutex_lock(&mylock)) != 0){
            my_error("lock error",__LINE__);
        }
        MPMCQueuePush(head, (void*)(intptr_t)(rand() % 10));//x86_64架构转换一下指针长度
        if((check = pthread_mutex_unlock(&mylock)) != 0){
            my_error("unlock error",__LINE__);
        }

        if((check = pthread_cond_broadcast(&mycond)) != 0){//唤醒所有堵塞在条件变量的线程
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
        MPMCQueuePop(tail);
        pthread_mutex_unlock(&mylock);

        sleep(rand() % 6);
    }
    return NULL;
}

MPMCQueue *MPMCQueueInit(int capacity)
{
    head = (MPMCQueue*)malloc(sizeof(MPMCQueue) * capacity);
    if(head == NULL){
        my_error("malloc failed",__LINE__);
    }
    head -> next = NULL;
    tail = head;
    return head;
}

void MPMCQueuePush(MPMCQueue *queue, void *s)
{
    MPMCQueue *new = (MPMCQueue *)malloc(sizeof(MPMCQueue));
    if(new == NULL){
        my_error("malloc failed",__LINE__);
    }
    new -> num = (int)(intptr_t)(s);
    new -> next = NULL;
    printf("Producer: num = %d, tid = %ld\n", new -> num, pthread_self());
    tail -> next = new;
    tail = new;//尾插
}

void *MPMCQueuePop(MPMCQueue *queue)
{
    MPMCQueue *old = NULL;
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

void MPMCQueueDestory(MPMCQueue *queue)
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