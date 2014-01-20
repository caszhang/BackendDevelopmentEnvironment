// Author: zhangguoqiang01 <80176975@qq.com>
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
