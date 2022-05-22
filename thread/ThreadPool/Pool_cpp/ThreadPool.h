#ifndef _THREADPOOL_H
#define _THREADPOOL_H

#include <iostream>
#include <pthread.h>
#include <queue>
#include <cstring>
#include <unistd.h>

// 定义任务结构体
using callback = void(*)(void*);

struct Task
{
    Task()
    {
        function = nullptr;
        arg = nullptr;
    }
    Task(callback f, void *arg)
    {
        function = f;
        this->arg = arg;
    }
    callback function;
    void *arg;
};

// 任务队列
class TaskQueue
{
public:
    TaskQueue();
    ~TaskQueue();

    // 添加任务
    void addTask(Task &task);
    void addTask(callback func, void *arg);

    // 取出一个任务
    Task takeTask();

    // 获取当前队列中的任务个数
    inline int taskNumber()
    {
        return m_queue.size();
    }

private:
    pthread_mutex_t m_mutex;// 互斥锁
    std::queue<Task> m_queue;//任务队列
};

// 线程池
class ThreadPool
{
public:
    ThreadPool(int min, int max);
    ~ThreadPool();

    // 添加任务
    void addTask(Task task);
    // 获取忙线程个数
    int getBusyNumber();
    // 获取存活线程个数
    int getLiveNumber();

private:
    // 工作的线程的任务函数
    static void *worker(void *arg);
    // 管理者线程的任务函数
    static void *manager(void *arg);
    // 线程退出函数
    void threadExit();

private:
    pthread_mutex_t m_lock;
    pthread_cond_t m_Empty;
    
    pthread_t *m_threadIDs;
    pthread_t m_managerID;

    TaskQueue *m_taskQ;
    int m_minNum;
    int m_maxNum;
    int m_busyNum;
    int m_liveNum;
    int m_exitNum;

    bool m_shutdown = false;
};


void my_error(const char *str, const int line);


#endif