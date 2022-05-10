#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
#include <stdint.h>

#define N 5
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int philosopher[N+1] = {0, 1, 2, 3, 4};//五个哲学家的编号

void *PHILO(void* arg);

int main(int argc, char **argv)
{   
    pthread_t philo[N];//五个哲学家线程
    for(int i = 0; i < N; i++){
        pthread_create(&philo[i], NULL, PHILO, &philosopher[i]);
    }
    for(int i = 0; i < N; i++){
        pthread_join(philo[i], NULL);
    }
    pthread_mutex_destroy(&mutex);

    return 0;
}

void *PHILO(void *arg)
{
    while(1){
        //哲学家按序号顺时针就坐，筷子按序号顺时针摆放(筷子0在哲学家0的左手边)
        int i = *(int*)arg;
        int left = i;
        int right;
        if(i == 0){
            right = 4;
        }else{
            right = i - 1;
        }

        printf("哲学家 %d 在思考...\n", i);
        sleep((rand() % 10));
        printf("哲学家 %d 饿了\n", i);
        
        pthread_mutex_lock(&mutex);//使用互斥锁，只有拿起两个筷子均可使用才能拿起
        printf("哲学家 %d 拿起了左筷子 %d 和右筷子 %d 开始就餐\n", i, left, right);
        sleep((rand() % 10));//吃完休息一会
        printf("哲学家 %d 就餐完毕, 放下了左筷子%d 和右筷子 %d\n", i, left, right);
        pthread_mutex_unlock(&mutex); 

        sleep((rand() % 3));
    }

    return NULL;
}