/***************************************************************************
 * 
 * Copyright (c) 2013 80176975@qq.com, Inc. All Rights Reserved
 * 
 **************************************************************************/
 
 
 
/**
 * @file Lock.h
 * @author zhangguoqiang01(80176975@qq.com)
 * @date 2013/12/17 21:43:26
 * @brief 
 *  
 **/




#ifndef  __LOCK_H_
#define  __LOCK_H_
#include "mutex.h"
class Lock
{
public:
    Lock(Mutex& mutex);
    ~Lock();
private:
    Mutex& m_mutex; 
};
#endif  //__LOCK_H_

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
