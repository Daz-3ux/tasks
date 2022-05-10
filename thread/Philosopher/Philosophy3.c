/*
控制奇数编号哲学家先拿右筷子再拿左筷子
控制偶数编号哲学家先拿左筷子再拿右筷子
哲学家编号为1-5
筷子编号为0-4

即使五个线程同时申请筷子，也只有筷子1，3出现竞争
总有一个线程可以申请到另外一只筷子，保证程序运行
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
#include <stdint.h>

#define N 5
sem_t max;
sem_t chopsticks[N];
int philosopher[N+1] = {1, 2, 3, 4, 5};//五个哲学家的编号

void *PHILO(void* arg);

int main(int argc, char **argv)
{
    sem_init(&max, 0 ,4);//最多允许四个哲学家用餐
    
    pthread_t philo[N];//五个哲学家线程
    for(int i = 0; i < N; i++){
        sem_init(&chopsticks[i], 0, 1);//每只筷子最多一个人使用
        pthread_create(&philo[i], NULL, PHILO, &philosopher[i]);
    }

    for(int i = 0; i < N; i++){
        pthread_join(philo[i], NULL);
        sem_destroy(&chopsticks[i]);
    }
    sem_destroy(&max);
}

void *PHILO(void *arg)
{
    while(1){
        //哲学家按序号顺时针就坐，筷子按序号顺时针摆放(哲学家1的左手边为筷子0，右手边为4)
        int i = *(int*)arg;
        int left = i-1;
        int right;
        if(i == 1){
            right = 4;
        }else{
            right = i - 2;
        }

        printf("哲学家 %d 在思考...\n", i);
        sleep((rand() % 5));
        printf("哲学家 %d 饿了\n", i);
        sem_wait(&max);//要吃饭了，申请一个吃饭资格资源量

        if(i % 2 != 0){//奇数哲学家，先拿右筷子再拿左筷子
            sem_wait(&chopsticks[right]);
            printf("奇数哲学家 %d 先拿起了右筷子 %d, 等待左筷子 %d\n",i, right, left);

            sem_wait(&chopsticks[left]);
            printf("奇数哲学家 %d 等到了左筷子 %d 开始吃饭\n", i, left);
            sleep((rand() % 5));

            sem_post(&chopsticks[right]);
            printf("奇数哲学家 %d 放下了右筷子 %d\n",i , right);
            sem_post(&max);
            sem_post(&chopsticks[left]);
            printf("奇数哲学家 %d 放下了左筷子 %d 结束用餐\n",i, left);

            sleep((rand() % 3));
        }else{//偶数哲学家，先拿左筷子再拿右筷子
            sem_wait(&chopsticks[left]);
            printf("偶数哲学家 %d 先拿起了左筷子 %d, 等待右筷子 %d\n",i, left, right);

            sem_wait(&chopsticks[right]);
            printf("偶数哲学家 %d 等到了右筷子 %d 开始吃饭\n", i, right);
            sleep((rand() % 5));

            sem_post(&chopsticks[left]);
            printf("偶数哲学家 %d 放下了左筷子 %d\n",i , left);
            sem_post(&max);
            sem_post(&chopsticks[right]);
            printf("偶数哲学家 %d 放下了右筷子 %d 结束用餐\n",i, right);

            sleep((rand() % 3));
        }
    }

    return NULL;
}