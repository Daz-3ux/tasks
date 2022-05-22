#include "ThreadPool.h"

void taskFunc(void *arg)
{
    int num = *(int *)arg;
    std::cout << "Thread" << num << " is working, numer = " << std::to_string(pthread_self()) << std::endl;
    sleep(1);
}

int main(int argc, char **argv)
{
    ThreadPool pool(3, 10);
    for(int i = 0; i < 100; i++){
        int *num = new int(i+100);
        pool.addTask(Task(taskFunc, num));
    }

    sleep(10);

    return 0;
}