// Copyright (c) 2013, Baidu Inc.
// All rights reserved.
// Author: zhangguoqiang01 <zhangguoqiang01@baidu.com>
// Fixed size, support multithreaded, circular queue
// optimize: each mutex for head & tail, 2013
#ifndef COMMON_QUEUE_H_
#define COMMON_QUEUE_H_
#include <pthread.h>
#include <stdint.h>
#include <sys/time.h>
#include <string.h>
#include <assert.h>
#include <semaphore.h>
#include <errno.h>

template <class T>
class CommonQueue
{
public:
    CommonQueue();
    virtual ~CommonQueue();
public:
    bool Init(uint32_t size);
    bool Release();
public:
    bool WaitTillPush(const T& node);
    bool WaitTillPop(T& node);
    bool WaitTimePush(const T& node, uint32_t usec);
    bool WaitTimePop(T& node, uint32_t usec);
public:
    uint32_t GetSize();
    uint32_t GetNodeNum();
private:
    T *m_data;
    uint32_t m_tail;
    uint32_t m_head;
    uint32_t m_size;
    pthread_mutex_t m_w_mutex;
    pthread_mutex_t m_r_mutex;
    sem_t m_work_sem;
    sem_t m_free_sem;
    //uint32_t m_node_num;
};
#endif // COMMON_QUEUE_H_
