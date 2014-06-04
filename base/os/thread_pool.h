/*******************************************************************************
 * Author :          guoqiang
 * Email :           guoqiang@foxea.com
 * Last modified :   2014-06-04 00:12
 * Filename :        thread_pool.h
 * Description :     
 * *****************************************************************************/
#ifndef __THREAD_POOL_H_
#define __THREAD_POOL_H_
#include "dsalgo/common_queue.cc"
#include "dsalgo/head.h"
#include "thread.h"
struct ThreadPoolTask {
    void (*m_function)(void *);
    void *m_argument;

    ThreadPoolTask & operator = (const ThreadPoolTask &item) {
        m_function = item.m_function;
        m_argument = item.m_argument;
        return *this;
    }
};
typedef CommonQueue<ThreadPoolTask> ThreadPoolTaskQueue;

class ThreadPool
{
public:
    ThreadPool();
    virtual ~ThreadPool();
    bool Init(int threads_num);
    void Start();
    void AddTask(const ThreadPoolTask &task);
    void Stop();
    void Release();
private:
    Thread **m_threads;
    int m_threads_num;
public:
    ThreadPoolTaskQueue *m_task_queue;
    bool m_continue;
};

#endif // __THREAD_POOL_H_
