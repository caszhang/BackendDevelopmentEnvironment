// Author: zhangguoqiang01 <80176975@qq.com>
#include "thread.h"

void* thread_routine(void *param)
{
    Thread *t = reinterpret_cast<Thread*>(param);
    t->m_func(t->m_param);
    return NULL;
    //pthread_exit(NULL);
}


//Thread::Thread(ThreadFunc &func, void* param)
Thread::Thread(ThreadFunc func, void* param)
    :m_func(func), 
     m_param(param)
{
}

Thread::~Thread()
{}

void Thread::Start()
{
    int ret = pthread_create(&m_thread, NULL, thread_routine, this);
    assert(0 == ret);
    return;
}

void Thread::Stop()
{
    pthread_join(m_thread, NULL);
    return;
}


/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
