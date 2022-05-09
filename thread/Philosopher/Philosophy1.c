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
int philosopher[N+1] = {0, 1, 2, 3, 4};//五个哲学家的编号

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
        sem_wait(&max);//要吃饭了，申请一个吃饭资格资源量
        sem_wait(&chopsticks[left]);//申请左筷子的使用权
        printf("哲学家 %d 拿起了左筷子 %d, 等待右筷子%d\n", i, left, right);
        sem_wait(&chopsticks[right]);//
        printf("哲学家 %d 拿起了右筷子 %d, 开始吃饭\n", i, right);

        sleep((rand() % 10));//吃完休息一会
        
        sem_post(&chopsticks[left]);//哲学家放下左筷子
        printf("哲学家 %d 放下了左筷子%d\n", i, left);

        sem_post(&max);//此时哲学家手中只有一只筷子了，可以释放吃饭资格资源量了
        sem_post(&chopsticks[right]);//哲学家放下右筷子
        printf("哲学家 %d 吃完了，放下了右筷子%d\n", i, right);

        sleep((rand() % 3));
    }

    return NULL;
}