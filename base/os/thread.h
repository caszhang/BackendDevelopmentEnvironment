// Author: zhangguoqiang01 <80176975@qq.com>

#ifndef  __THREAD_H_
#define  __THREAD_H_
#include "dsalgo/head.h"

typedef void (*ThreadFunc)(void *);
void* thread_routine(void *param);

class Thread
{
    public:
        //Thread(ThreadFunc &func, void *param);
        Thread(ThreadFunc func, void *param);
        virtual ~Thread();
        void Start();
        void Stop();
    public:
        ThreadFunc m_func;
        void* m_param;
    private:
        pthread_t m_thread;
};
#endif  //__THREAD_H_

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
