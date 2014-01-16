/***************************************************************************
 * 
 * Copyright (c) 2013 80176975@qq.com, Inc. All Rights Reserved
 * 
 **************************************************************************/
 
 
 
/**
 * @file mutex.cc
 * @author zhangguoqiang01(80176975@qq.com)
 * @date 2013/12/17 21:32:37
 * @brief 
 *  
 **/
#include "mutex.h"

Mutex::Mutex()
{
    pthread_mutex_init(&m_mutex, NULL);
}

Mutex::~Mutex()
{
     pthread_mutex_destroy(&m_mutex);
}

bool Mutex::Lock()
{
    if (pthread_mutex_lock(&m_mutex) != 0) {
        return false; 
    }
    return  true;
}

bool Mutex::Unlock()
{
    if (pthread_mutex_unlock(&m_mutex) != 0) {
        return false; 
    }
    return  true;
}

pthread_mutex_t* Mutex::GetMutex()
{
    return &m_mutex;
}

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
