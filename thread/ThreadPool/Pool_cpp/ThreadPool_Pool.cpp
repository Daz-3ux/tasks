#include "ThreadPool.h"

void my_error(const char *str, const int line)
{
    std::cout << str << " in " << line << std::endl;
}

ThreadPool::ThreadPool(int minNum, int maxNum)
{
    // 实例化任务队列
    m_taskQ = new TaskQueue;
do{
    m_minNum = minNum;
    m_maxNum = maxNum;
    m_busyNum = 0;
    m_liveNum = minNum;

    // 根据线程的最大上限给线程数组分配内存
    m_threadIDs = new pthread_t[maxNum];
    if(m_threadIDs == nullptr)
    {
        my_error("new error", __LINE__);
        break;
    }
    // 初始化
    memset(m_threadIDs, 0, sizeof(pthread_t)*maxNum);
    if(pthread_mutex_init(&m_lock, NULL) != 0 || 
        pthread_cond_init(&m_Empty, NULL) != 0){
            my_error("init error", __LINE__);
            break;
    }

    // 创建线程
    for(int i =0; i < minNum; i++){
        pthread_create(&m_threadIDs[i], NULL, worker, this);
        std::cout << "创建子线程, ID: " << std::to_string(m_threadIDs[i]) << std::endl;
    }
    pthread_create(&m_managerID, NULL, manager, this);
}while(0);
}


ThreadPool::~ThreadPool()
{
    m_shutdown = 1;
    pthread_join(m_managerID, NULL);
    for(int i = 0; i < m_liveNum; i++){
        pthread_cond_signal(&m_Empty);
    }

    if(m_taskQ){
        delete m_taskQ;
    }
    if(m_threadIDs){
        delete []m_threadIDs;
    }
    pthread_mutex_destroy(&m_lock);
    pthread_cond_destroy(&m_Empty);
}

void ThreadPool::addTask(Task task)
{
    if(m_shutdown){
        return;
    }
    //添加任务到任务队列(任务队列中有锁)
    m_taskQ->addTask(task);
    //唤醒工作线程
    pthread_cond_signal(&m_Empty);
}

int ThreadPool::getLiveNumber()
{
    int threadNum = 0;
    pthread_mutex_lock(&m_lock);
    threadNum = m_liveNum;
    pthread_mutex_unlock(&m_lock);
    return threadNum;
}

int ThreadPool::getBusyNumber()
{
    int threadNum = 0;
    pthread_mutex_lock(&m_lock);
    threadNum = m_busyNum;
    pthread_mutex_unlock(&m_lock);
    return threadNum;
}

void ThreadPool::threadExit()
{
    pthread_t tid = pthread_self();
    for(int i = 0; i < m_maxNum; i++){
        if(m_threadIDs[i] == tid){
            std::cout << "threadExit(): thread " << std::to_string(pthread_self()) <<
            " exiting.." << std::endl;
            m_threadIDs[i] = 0;
            break;
        }
    }
    pthread_exit(NULL);
}

void *ThreadPool::worker(void *arg)
{
    ThreadPool *pool = static_cast<ThreadPool*>(arg);
    
    while(true){
        //访问任务队列
        pthread_mutex_lock(&pool->m_lock);
        while(pool->m_taskQ->taskNumber() == 0 &&
        !pool->m_shutdown){
            std::cout << "thread" << std::to_string(pthread_self()) << " waiting" << std::endl;
            //解除阻塞
            pthread_cond_wait(&pool->m_Empty, &pool->m_lock);

            //解除阻塞后,判断是否需要销毁线程
            if(pool->m_exitNum > 0){
                pool->m_exitNum--;
                if(pool->m_liveNum > pool->m_minNum){
                    pool->m_liveNum--;
                    pthread_mutex_unlock(&pool->m_lock);
                    pool->threadExit();
                }
            }
        }

    // 判断线程池是否关闭
    if(pool->m_shutdown){
        pthread_mutex_unlock(&pool->m_lock);
        pool->threadExit();
    }

    // 取出一个任务
    Task task = pool->m_taskQ->takeTask();
    pool->m_busyNum++;
    pthread_mutex_unlock(&pool->m_lock);
    // 执行任务
    std::cout<< "thread " << std::to_string(pthread_self()) << " start working" << std::endl;
    task.function(task.arg);
    //delete task.arg;
    task.arg = nullptr;

    // 任务处理结束
    std::cout << "thread " << std::to_string(pthread_self()) << "end working" << std::endl;
    pthread_mutex_lock(&pool->m_lock);
    pool->m_busyNum--;
    pthread_mutex_unlock(&pool->m_lock);
    }

    return nullptr;
}

void *ThreadPool::manager(void *arg)
{
    ThreadPool *pool = static_cast<ThreadPool*>(arg);

    while(!pool->m_shutdown){
        // 每5秒检测一次
        sleep(5);
        // 取出线程池中的任务数和线程总数
        pthread_mutex_lock(&pool->m_lock);
        int queueSize = pool->m_taskQ->taskNumber();
        int liveNum = pool->m_liveNum;
        int busyNum = pool->m_busyNum;
        pthread_mutex_unlock(&pool->m_lock);

        const int NUMBER = 2;

        //创建线程
        if(queueSize > liveNum && liveNum < pool->m_maxNum){
            pthread_mutex_lock(&pool->m_lock);
            int num = 0;
            for(int i = 0; i < pool->m_maxNum && num < NUMBER &&
            pool->m_liveNum < pool->m_maxNum; i++){
                if(pool->m_threadIDs[i] == 0){
                    pthread_create(&pool->m_threadIDs[i], NULL, worker, pool);
                    num++;
                    pool->m_liveNum++;
                }
            }
            pthread_mutex_unlock(&pool->m_lock);
        }

        //销毁多余线程
        if(busyNum * 2 < liveNum && liveNum > pool->m_minNum){
            pthread_mutex_lock(&pool->m_lock);
            pool->m_exitNum = NUMBER;
            pthread_mutex_unlock(&pool->m_lock);
            for(int i = 0; i < NUMBER; i++){
                pthread_cond_signal(&pool->m_Empty);
            }
        }
    }
    return nullptr;
}