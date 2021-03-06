#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
#include <stdint.h>

#define N 5

int i0, i1, i2, i3, i4;

int philosopher[N] = {0, 1, 2, 3, 4};//五个哲学家的编号
pthread_mutex_t mutex[N];

void *PHILO(void* arg);

int main(int argc, char **argv)
{   
    pthread_t philo[N];//五个哲学家线程
    for(int i = 0; i < N; i++){
        pthread_mutex_init(&mutex[i], NULL);
        pthread_create(&philo[i], NULL, PHILO, &philosopher[i]);
    }

    for(int i = 0; i < N; i++){
        pthread_join(philo[i], NULL);
        pthread_mutex_destroy(&mutex[i]);
    }

    return 0;
}

void *PHILO(void *arg)
{
    while(1){
        //哲学家按序号顺时针就坐，筷子按序号顺时针摆放(筷子0在哲学家0的右手边)
        int i = *(int*)arg;
        int left,right;

        if(i < 4){
            right = i;
            left = i + 1;
            if(i == 2){
                right = 3;
                left = 2;
            }
        }
        if(i == 4){
            left = 4;
            right = 0;
        }

        pthread_mutex_lock(&mutex[right]);//先给右手加锁
        if(pthread_mutex_trylock(&mutex[left]) == 0){//再尝试给左手加锁
            //左右锁齐全，开始就餐
            printf("哲学家 %d 正在就餐\n", i);
            switch (i){
                case 0: i0++; break;
                case 1: i1++; break;
                case 2: i2++; break;
                case 3: i3++; break;
                case 4: i4++; break;
                default: break;
            }
            printf("哲学家分别就餐次数: %d %d %d %d %d\n", i0, i1, i2, i3, i4);
            pthread_mutex_unlock(&mutex[left]);
        }

        pthread_mutex_unlock(&mutex[right]);
        

        //sleep( rand() % 3);
    }
    return NULL;
}