#ifndef _THREADPOOL_H
#define _THREADPOOL_H

#define NUM 2
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>


typedef struct ThreadPool ThreadPool;

// 创建线程池并初始化
ThreadPool *ThreadPoolCreate(int min, int max, int queueSize);

// 销毁线程池
int threadPoolDestroy(ThreadPool *pool);

// 给线程池添加任务
void threadPoolAdd(ThreadPool *pool, void(*func)(void*), void*arg);

// 获取线程池工作的线程的个数
int threadPoolBusyNum(ThreadPool *pool);
////这两个函数对线程池实现无意义,仅为用户提供接口
// 获取线程池活着的线程的个数
int threadPoolLiveNum(ThreadPool *pool);

//工作线程任务函数
void *worker(void *arg);

//管理者线程任务函数
void *manager(void *arg);

// 单个线程退出
void threadExit(ThreadPool* pool);

// 错误处理函数
void my_error(const char *str, const int line);

#endif // !_THREADPOOL_H