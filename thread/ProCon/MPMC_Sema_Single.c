/*
使用信号量完成生产者消费者问题
多生产者多消费者
总资源数为1
省略检查返回值
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

struct Node {
    int number;
    struct Node *next;
};

struct Node *head = NULL;
sem_t psem;//生产者信号量
sem_t csem;//消费者信号量


void *producer(void *arg);//生产者函数
void *consumer(void *arg);//消费者函数

int main(int argc, char **argv)
{
    pthread_t ptid[5];//生产者线程
    pthread_t ctid[5];//消费者线程

    sem_init(&psem, 0, 1);//初始化生产者信号量，生产者线程拥有1个信号灯
    sem_init(&csem, 0, 0);//初始化消费者信号量，消费者线程拥有0个信号灯
    //如此这般的意义是使生产者线程先运行

    for(int i = 0; i < 5; i++){//初始化线程
        pthread_create(&ptid[i], NULL, producer, NULL);
        pthread_create(&ctid[i], NULL, consumer, NULL);
    }
    
    sem_destroy(&psem);//反初始化信号量
    sem_destroy(&csem);
    
    for(int i = 0; i < 5; i++){//回收线程
        pthread_join(ptid[i], NULL);
        pthread_join(ctid[i], NULL);
    }

    return 0;
}

void *producer(void *arg)
{
    while(1){
        sem_wait(&psem);//生产者拿一个信号灯
        struct Node* pnew = (struct Node*)malloc(sizeof(struct Node));//创建一个新节点
        pnew -> number = rand() % 100;//初始化
        pnew -> next = head;//链接节点
        head = pnew;//头结点前移
        printf("Producer: number = %d, tid = %ld\n", pnew -> number, pthread_self());

        sem_post(&csem);//通知消费者消费
        sleep(rand() % 3);
    }

    return NULL;
}

void *consumer(void *arg)
{
    while(1){
        sem_wait(&csem);
        struct Node *pnode = head;//生产者先运行确保此处节点挪动逻辑不会出错
        printf("Consumer: number = %d, tid = %ld\n", pnode -> number, pthread_self());
        head = pnode -> next;
        free(pnode);

        sem_post(&psem);//通知生产者生产

        sleep(rand() % 3);
    }

    return NULL;
}