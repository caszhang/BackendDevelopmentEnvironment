/***************************************************************************
 * 
 * Copyright (c) 2014 80176975@qq.com, Inc. All Rights Reserved
 * 
 **************************************************************************/
 
 
 
/**
 * @file thread.h
 * @author zhangguoqiang01(80176975@qq.com)
 * @date 2014/01/17 11:39:43
 * @brief 
 *  
 **/




#ifndef  __THREAD_H_
#define  __THREAD_H_
#include "dsalgo/head.h"

typedef void (*ThreadFunc)(void *);

class Thread
{
    public:
        Thread(ThreadFunc &func);
        virtual ~Thread();
        static void* Routine(void *param);
        void Start();
        void Stop();
    public:
        ThreadFunc m_func;
    private:
        pthread_t m_thread;
};
#endif  //__THREAD_H_

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
