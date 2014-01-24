// Author: zhangguoqiang01 <80176975@qq.com>
#include "gtest/gtest.h"
#include "os/condition.h"
#include "os/lock.h"
#include "os/thread.h"

struct MutexComposition {
    Mutex *m_mutex;
    int32_t m_value;
    Condition *m_produce;
    Condition *m_consume;

    MutexComposition() {
        m_mutex = NULL;
        m_value = 0; 
        m_produce = NULL;
        m_consume = NULL;
    }

    MutexComposition(Mutex *mutex, int32_t value, Condition *produce, Condition *consume)
        :m_mutex(mutex),
        m_value(value),
        m_produce(produce),
        m_consume(consume)
    {}

    MutexComposition & operator = (const MutexComposition &mutex_composition) {
        m_mutex = mutex_composition.m_mutex;
        m_value = mutex_composition.m_value; 
        m_produce = mutex_composition.m_produce;
        m_consume = mutex_composition.m_consume;
        return *this;
    }
};

void produce(void *param)
{
    MutexComposition *mutex_composition = reinterpret_cast<MutexComposition*>(param);
    for (int i = 0; i < 100; i++) {
        Lock temp_lock(*(mutex_composition->m_mutex));
        // to avoid spurious wake up
        while (mutex_composition->m_value > 0) {
            mutex_composition->m_produce->Wait(); 
        }
        mutex_composition->m_value++;
        mutex_composition->m_consume->Notify();
    }
}

void consume(void *param)
{
    MutexComposition *mutex_composition = reinterpret_cast<MutexComposition*>(param);
    for (int i = 0; i < 100; i++) {
        Lock temp_lock(*(mutex_composition->m_mutex));
        while (mutex_composition->m_value < 1) {
            mutex_composition->m_consume->Wait(); 
        }
        mutex_composition->m_value--;
        mutex_composition->m_produce->Notify();
    }
}

TEST(ConditionTest, MultiThreadTest)
{
    Mutex mutex;
    int value = 0;
    Condition produce_cond(mutex);
    Condition consume_cond(mutex);
    MutexComposition mutex_composition(&mutex, value, &produce_cond, &consume_cond);
    Thread produce_thread(produce, &mutex_composition);
    Thread consume_thread(consume, &mutex_composition);
    produce_thread.Start();
    consume_thread.Start();
    produce_thread.Stop();
    consume_thread.Stop();
    EXPECT_EQ(0, mutex_composition.m_value);
}
/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
