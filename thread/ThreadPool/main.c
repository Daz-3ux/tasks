#include "ThreadPool.h"


void taskFunc(void *arg)
{
    int num = *(int*)arg;
    printf("thread %ld is working, number = %d\n", \
        pthread_self(), num);
    sleep(1);
}

int main(int argc, char **argv)
{
    //创建线程池(最小线程数,最大线程数,任务队列容量)
    ThreadPool *pool = ThreadPoolCreate(3, 10, 100);
    for(int i = 0; i < 100; i++){
        int *num = (int*)malloc(sizeof(int));
        *num = i+100;
        threadPoolAdd(pool, taskFunc, num);
    }

    sleep(10);

    threadPoolDestroy(pool);

    return 0;
}