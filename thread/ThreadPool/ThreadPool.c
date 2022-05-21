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
    int queueCapacity;              //最大容量
    int queueSize;                  //当前任务个数
    int queueBegin;                 //队头 ： 取数据
    int queueEnd;                   //队尾 ： 放数据

    pthread_t managerID;            //管理者线程ID
    pthread_t *threadIDs;           //工作线程ID
    int minNum_Threads;              //最小线程 个数
    int maxNum_Threads;              //最大线程 个数
    int busyNum_Threads;             //忙线程 个数
    int liveNum_Threads;             //存活线程 个数
    int exitNum_Threads;             //要销毁的线程 个数

    pthread_mutex_t mutexPool;      //大锁：锁线程池
    pthread_mutex_t mutexBusy;      //小锁：锁忙线程个数(此变量最常用)
    pthread_cond_t Full;         //任务队列是否满
    pthread_cond_t Empty;        //任务队列是否空

    int shutdown;                   //是否销毁线程池：销毁为1，不销毁为0
};

ThreadPool *ThreadPoolCreate(int min, int max, int queueSize)
{
    
    ThreadPool *pool = (ThreadPool *)malloc(sizeof(ThreadPool));
do{
    //线程
    if(pool == NULL){
        my_error("malloc error",__LINE__);
        break;
    }

    pool->threadIDs = (pthread_t *)malloc(sizeof(pthread_t)*max);
    if(pool->threadIDs == NULL){
        my_error("malloc error",__LINE__);
        break;
    }
    memset(pool->threadIDs, 0, sizeof(pthread_t)*max);
    pool->maxNum_Threads = max;
    pool->minNum_Threads = min;
    pool->liveNum_Threads = min;//相等于最小数
    pool->busyNum_Threads = 0;
    pool->exitNum_Threads = 0;

    //锁与条件变量
    if(
    pthread_mutex_init(&pool->mutexPool, NULL) != 0 ||
    pthread_mutex_init(&pool->mutexBusy, NULL) != 0 ||
    pthread_cond_init(&pool->Empty, NULL) != 0 ||
    pthread_cond_init(&pool->Full, NULL) != 0
    ){
        my_error("mutex/cond error",__LINE__);
        break;
    }

    //任务队列
    pool->taskQ = (Task*)malloc(sizeof(Task)*queueSize);
    if(pool->taskQ == NULL){
        my_error("malloc error",__LINE__);
    }
    pool->queueCapacity = queueSize;
    pool->queueSize = 0;
    pool->queueBegin = 0;
    pool->queueEnd = 0;

    //flag
    pool->shutdown = 0;

    //创建线程
    pthread_create(&pool->managerID, NULL, manager, pool);
    for(int i = 0; i < min; i++){
        pthread_create(&pool->threadIDs[i], NULL, worker, pool);
    }
    return pool;
}while(0);

//销毁资源:从do...while中异常跳出
    if(pool && pool->threadIDs) free(pool->threadIDs);
    if(pool && pool->taskQ)     free(pool->taskQ);
    if(pool)                    free(pool);

    return NULL;
}


void *worker(void* arg)
{
    ThreadPool *pool = (ThreadPool*)arg;
    while(1){
        pthread_mutex_lock(&pool->mutexPool);
        // 判断当前任务队列是否为空
        while(pool->queueSize == 0 && !pool->shutdown){
            pthread_cond_wait(&pool->Empty, &pool->mutexPool);
            if(pool->exitNum_Threads > 0){
                pool->exitNum_Threads--;
                if(pool->liveNum_Threads > pool->minNum_Threads){
                    pool->liveNum_Threads--;
                    pthread_mutex_unlock(&pool->mutexPool);
                    threadExit(pool);
                }
            }
        }

        // 判断线程池是否已关闭
        if(pool->shutdown == 1){
            pthread_mutex_unlock(&pool->mutexPool);
            //pthread_mutex_unlock(&pool->mutexPool);
            threadExit(pool);
        }

        //从任务队列取出一个任务
        Task task;
        task.func = pool->taskQ[pool->queueBegin].func;
        task.arg = pool->taskQ[pool->queueBegin].arg;

        // 移动头结点
        pool->queueBegin = (pool->queueBegin + 1) % pool->queueCapacity;
        pool->queueSize--;

        //解锁
        pthread_cond_signal(&pool->Full);
        pthread_mutex_unlock(&pool->mutexPool);

        printf("thread %ld start working\n",pthread_self());

        pthread_mutex_lock(&pool->mutexBusy);
        pool->busyNum_Threads++;
        pthread_mutex_unlock(&pool->mutexBusy);

        task.func(task.arg);
        free(task.arg);
        task.arg = NULL;

        printf("thread %ld end working\n",pthread_self());
        pthread_mutex_lock(&pool->mutexBusy);
        pool->busyNum_Threads--;
        pthread_mutex_unlock(&pool->mutexBusy);
    }

    return NULL;
}


void *manager(void * arg)
{
    ThreadPool *pool = (ThreadPool*)arg;
    while(pool->shutdown == 0){
        // 每3s检测一次
        sleep(3);
        
        // 去除线程池中任务的数量和当前线程的数量
        pthread_mutex_lock(&pool->mutexPool);
        int queueSize = pool->queueSize;
        int liveNum = pool->liveNum_Threads;
        pthread_mutex_unlock(&pool->mutexPool);

        // 取出忙线程个数
        pthread_mutex_lock(&pool->mutexBusy);
        int busyNum = pool->busyNum_Threads;
        pthread_mutex_unlock(&pool->mutexBusy);

        //添加线程
        //任务个数 > 存活线程个数 && 存活线程个数 < 最大线程数
        if(queueSize > liveNum && liveNum < pool->maxNum_Threads){
            pthread_mutex_lock(&pool->mutexPool);
            int counter = 0;
            for(int i = 0; 
            i < pool->maxNum_Threads &&
            counter < NUM && 
            pool->liveNum_Threads < pool->maxNum_Threads;i++){
                if(pool->threadIDs[i] == 0){
                    pthread_create(&pool->threadIDs[i], NULL, worker, pool);
                    counter++;
                    pool->liveNum_Threads++;
                }
            }
            pthread_mutex_unlock(&pool->mutexPool);
        }

        // 销毁线程
        // 忙的线程个数*2 < 存活线程数 && 存活的线程 > 最小线程数
        if(busyNum*2 < liveNum && liveNum > pool->minNum_Threads){
            pthread_mutex_lock(&pool->mutexPool);
            pool->exitNum_Threads = NUM;
            pthread_mutex_unlock(&pool->mutexPool);
            //唤醒堵塞线程使其自杀
            for(int i = 0; i < NUM; i++){
                pthread_cond_signal(&pool->Empty);
            }
        }
    }
}

int threadPoolDestroy(ThreadPool *pool)
{
    if(pool == NULL){
        return -1;
    }

    // 关闭线程池
    pool->shutdown = 1;
    // 阻塞回收管理者线程
    pthread_join(pool->managerID, NULL);
    // 唤醒阻塞的消费者线程
    for(int i = 0; i < pool->liveNum_Threads; i++){
        pthread_cond_signal(&pool->Empty);//每次只能自杀一个,唤醒多了也没用
    }
    //释放堆内存
    if(pool->taskQ){
        free(pool->taskQ);
    }
    if(pool->threadIDs){
        free(pool->threadIDs);
    }

    pthread_mutex_destroy(&pool->mutexPool);
    pthread_mutex_destroy(&pool->mutexBusy);
    pthread_cond_destroy(&pool->Empty);
    pthread_cond_destroy(&pool->Full);

    free(pool);
    pool = NULL;

    return 0;
} 

void threadPoolAdd(ThreadPool * pool, void(*func)(void*), void*arg)
{
    pthread_mutex_lock(&pool->mutexPool);
    while(pool->queueSize == pool->queueCapacity &&
    pool->shutdown == 0){
        //队列满,阻塞生产者线程
        pthread_cond_wait(&pool->Full, &pool->mutexPool);
    }
    if(pool->shutdown == 1){
        //若需要销毁线程池
        pthread_mutex_unlock(&pool->mutexPool);
        return;
    }

    //添加任务
    pool->taskQ[pool->queueEnd].func = func;
    pool->taskQ[pool->queueEnd].arg = arg;
    pool->queueEnd = (pool->queueEnd+1) % pool->queueCapacity;
    pool->queueSize++;

    pthread_cond_signal(&pool->Empty);//有任务了,唤醒因空队列而阻塞的线程
    pthread_mutex_unlock(&pool->mutexPool);
}

int threadPoolBusyNum(ThreadPool* pool)
{
    pthread_mutex_lock(&pool->mutexBusy);
    int busyNum = pool->busyNum_Threads;
    pthread_mutex_unlock(&pool->mutexBusy);
    return busyNum;
}

int threadPoolAliveNum(ThreadPool* pool)
{
    pthread_mutex_lock(&pool->mutexPool);
    int aliveNum = pool->liveNum_Threads;
    pthread_mutex_unlock(&pool->mutexPool);
    return aliveNum;
}

void threadExit(ThreadPool *pool)
{
    pthread_t tid = pthread_self();
    for(int i = 0; i < pool->maxNum_Threads; i++){
        if(pool->threadIDs[i] == tid){
            pool->threadIDs[i] = 0;
            printf("threadExit() called, %ld exiting\n", tid);
            break;
        }
    }

    pthread_exit(NULL);
}

void my_error(const char *str, const int line)
{
    printf("%s in %d\n", str,line);
}



