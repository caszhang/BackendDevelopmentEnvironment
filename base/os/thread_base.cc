// All rights reserved.
// Author: zhangguoqiang01 <80176975@qq.com>
// base class for class needing thread
#include "thread_base.h"

ThreadBase::ThreadBase()
{}

ThreadBase::~ThreadBase()
{}

void* ThreadBase::Routine(void *param)
{
    ThreadBase *tb = (ThreadBase*)param;
    tb->Run();
    return NULL;
    //pthread_exit(NULL);
}

void ThreadBase::Start()
{
    int ret = pthread_create(&m_thread, NULL, ThreadBase::Routine, this);
    assert(0 == ret);
    return;
}

void ThreadBase::Stop()
{
    UnRun();
    pthread_join(m_thread, NULL);
    return;
}

