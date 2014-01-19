/***************************************************************************
 * 
 * Copyright (c) 2014 80176975@qq.com, Inc. All Rights Reserved
 * 
 **************************************************************************/
 
 
 
/**
 * @file thread.cc
 * @author zhangguoqiang01(80176975@qq.com)
 * @date 2014/01/17 11:46:42
 * @brief 
 *  
 **/
#include "thread.h"

Thread::Thread(ThreadFunc &func)
    :m_func(func)
{
}

Thread::~Thread()
{}

void* Thread::Routine(void *param)
{
    Thread *t = reinterpret_cast<Thread*>(param);
    t->m_func(param);
    return NULL;
    //pthread_exit(NULL);
}

void Thread::Start()
{
    int ret = pthread_create(&m_thread, NULL, Thread::Routine, this);
    assert(0 == ret);
    return;
}

void Thread::Stop()
{
    pthread_join(m_thread, NULL);
    return;
}


/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
