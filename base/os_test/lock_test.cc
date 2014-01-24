// Author: zhangguoqiang01 <80176975@qq.com>
#include "gtest/gtest.h"
#include "os/lock.h"
#include "os/thread.h"

struct MutexComposition {
    Mutex *m_mutex;
    int32_t m_value;

    MutexComposition() {
        m_mutex = NULL;
        m_value = 0; 
    }

    MutexComposition(Mutex *mutex, int32_t value)
        :m_mutex(mutex),
        m_value(value)
    {}

    MutexComposition & operator = (const MutexComposition &mutex_composition) {
        m_mutex = mutex_composition.m_mutex;
        m_value = mutex_composition.m_value; 
        return *this;
    }
};

void increase(void *param)
{
    MutexComposition *mutex_composition = reinterpret_cast<MutexComposition*>(param);
    for (int i = 0; i < 200; i++) {
        Lock temp_lock(*(mutex_composition->m_mutex));
        mutex_composition->m_value++;
        usleep(100);
    }
}

void decrease(void *param)
{
    MutexComposition *mutex_composition = reinterpret_cast<MutexComposition*>(param);
    for (int i = 0; i < 100; i++) {
        Lock temp_lock(*(mutex_composition->m_mutex));
        mutex_composition->m_value--;
        usleep(100);
    }
}

TEST(LockTest, MultiThreadTest) 
{
    Mutex mutex;
    int value = 0;
    MutexComposition mutex_composition(&mutex, value);
    Thread increase_thread(increase, &mutex_composition);
    Thread decrease_thread(decrease, &mutex_composition);
    increase_thread.Start();
    decrease_thread.Start();
    increase_thread.Stop();
    decrease_thread.Stop();

    EXPECT_EQ(100, mutex_composition.m_value);
}
/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
