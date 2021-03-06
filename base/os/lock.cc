// Author: zhangguoqiang01 <80176975@qq.com>
#include "lock.h"

Lock::Lock(Mutex& mutex):m_mutex(mutex)
{
    m_mutex.Lock();
}

Lock::~Lock()
{
    m_mutex.Unlock();
}

#define Lock(x) static_assert(false, "missing var name, like Lock lock(x)")

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
