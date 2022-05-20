//定义结构体

#include "ThreadPool.h"

//任务结构体
typedef struct Task
{
    void (*func)(void *arg);//指针函数 / 泛型
    void *arg;
}Task;

// 线程池结构体
struct ThreadPool
{
    //任务队列
    Task *taskQ;
    int queueCapacity;              //容量
    int queueSize;                  //
    int queueBegin;                 //
    int queueEnd;                   //

    pthread_t managerID;            //
    pthread_t *threadIDs;           //
    int minNumThreads;              //
    int maxNumThreads;              //
    int busyNumThreads;             //
    int liveNumThreads;             //
    int exitNumThreads;             //

    pthread_mutex_t mutexPool;      //
    pthread_mutex_t mutexBusy;      //
    pthread_cond_t notFull;         //
    pthread_cond_t notEmpty;        //

    int shutdown;                   //
};

