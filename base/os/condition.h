/***************************************************************************
 * 
 * Copyright (c) 2014 80176975@qq.com, Inc. All Rights Reserved
 * 
 **************************************************************************/
 
 
 
/**
 * @file condition.h
 * @author zhangguoqiang01(80176975@qq.com)
 * @date 2014/01/11 17:24:32
 * @brief 
 *  
 **/




#ifndef  __CONDITION_H_
#define  __CONDITION_H_
#include "mutex.h"

class Condition 
{
    public:
    explicit Condition(Mutex &mutex);
    virtual ~Condition();
    void Wait();
    void Notify();
    void NotifyAll();
    private:
    Mutex& m_mutex;
    pthread_cond_t m_cond;
};
#endif  //__CONDITION_H_

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
