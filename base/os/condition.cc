// Author: zhangguoqiang01 <80176975@qq.com>
#include "condition.h"

Condition::Condition(Mutex &mutex):m_mutex(mutex)
{
    pthread_cond_init(&m_cond, NULL);
}

Condition::~Condition()
{
    pthread_cond_destroy(&m_cond);
}

void Condition::Wait()
{
    pthread_cond_wait(&m_cond, m_mutex.GetMutex());
}

void Condition::Notify()
{
    pthread_cond_signal(&m_cond);
}

void Condition::NotifyAll()
{
    pthread_cond_broadcast(&m_cond);
}


/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
