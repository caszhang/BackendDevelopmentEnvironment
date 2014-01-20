// Author: zhangguoqiang01 <80176975@qq.com>
#include "gtest/gtest.h"
#include "dsalgo/lock_free_queue.cc"

const int32_t kMaxSize = 6553600;

void* Producer(void *param)
{
    LockFreeQueue<int32_t> *queue = (LockFreeQueue<int32_t>*)param;
    printf("push timebegin:%lu\n", time(NULL));
    for (int32_t i = 0; i < kMaxSize; i++) {
        queue->Single_Push(i);
    }
    printf("push timeend:%lu\n", time(NULL));
    return NULL;
}

void* Consumer(void *param)
{
    LockFreeQueue<int32_t> *queue = (LockFreeQueue<int32_t>*)param;
    int32_t temp;
    printf("pop timebegin:%lu\n", time(NULL));
    for (int32_t i = 0; i < kMaxSize; i++) {
        queue->Single_Pop(temp);
    }
    printf("pop timeend:%lu\n", time(NULL));
    return NULL;
}

TEST(LockFreeQueueTest, TestTimeConsume)
{
    LockFreeQueue<int32_t> *queue = new LockFreeQueue<int32_t>();
    queue->Init(kMaxSize);
    pthread_t m_producer;
    pthread_t m_consumer;
    pthread_create(&m_producer, NULL , Producer, queue);
    pthread_create(&m_consumer, NULL, Consumer, queue);
    pthread_join(m_producer, NULL);
    pthread_join(m_consumer, NULL);
    queue->Release();
    delete queue;
}


