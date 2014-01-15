// All rights reserved.
// Author: zhangguoqiang01 <80176975@qq.com>
// Fixed size, lock-free, support multithreaded, circular queue
// especially suitable for one producer and mutiple consumer
// if one producer, Single_Push and Single_Pop is better
// otherwise, use Multi_Func
#ifndef CAS_COMMON_DS_LOCK_FREE_QUEUE_H
#define CAS_COMMON_DS_LOCK_FREE_QUEUE_H
#pragma once
#include <stdint.h>

#define CAS(addr, old_val, new_val) __sync_bool_compare_and_swap(addr, old_val, new_val)
template <class T>
class LockFreeQueue
{
public:
    LockFreeQueue();
    ~LockFreeQueue();
public:
    bool Init(uint32_t size);
    void Release();
    bool Multi_Push(const T& node);
    bool Single_Push(const T& node);
    uint32_t GetIdx(uint32_t idx);
    bool Multi_Pop(T& node);
    bool Single_Pop(T& node);
private:
    uint32_t m_read_idx;
    uint32_t m_write_idx;
    uint32_t m_max_read_idx;
    uint32_t m_size;
    T *m_data;
};
#endif // CAS_COMMON_DS_LOCK_FREE_QUEUE_H
