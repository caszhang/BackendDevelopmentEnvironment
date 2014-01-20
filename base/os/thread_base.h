// Author: zhangguoqiang01 <80176975@qq.com>
// base class for class needing thread
#ifndef THREAD_BASE_H_
#define THREAD_BASE_H_
#pragma once
#include <stdio.h>

#include <assert.h>
#include <pthread.h>
class ThreadBase
{
public:
    ThreadBase();
    virtual ~ThreadBase();
public:
    void Start();
    void Stop();
    static void* Routine(void *param);
    virtual void Run() = 0;
    virtual void UnRun() = 0;
private:
    pthread_t m_thread;
};

#endif // THREAD_BASE_H_
