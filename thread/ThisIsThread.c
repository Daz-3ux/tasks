#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define MAX 10
//输出结果与理想状态不一样是因为我的电脑是多核CPU，在多线程问题上具有特殊性
/**
 * @description: 
 * @param {*}
 * @return {*}
 */
//单核处理器输出的结果应是10行`Hello World！`

int main(int argc, char **argv)
{
    pthread_t t1,t2;

    void *print_hello(void *);

    pthread_create(&t1, NULL, print_hello, (void *)"Hello ");
    pthread_create(&t2, NULL, print_hello, (void *)"World!\n");

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    return 0;
}

void *print_hello(void *s)
{
    for(int i = 0; i < MAX; i++){
        printf("%s", (char*)s);
        fflush(stdout);
        sleep(1);
    }

    return NULL;
}