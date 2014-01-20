// Author: zhangguoqiang01 <80176975@qq.com>


#ifndef  __MUTEX_H_
#define  __MUTEX_H_
#include "dsalgo/head.h"

class Mutex
{
public:
    Mutex();
    ~Mutex();
    bool Lock();
    bool Unlock();
    pthread_mutex_t* GetMutex();
private:
    pthread_mutex_t m_mutex;
};
#endif  //__MUTEX_H_

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
