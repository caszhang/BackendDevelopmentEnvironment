// All rights reserved.
// Author: zhangguoqiang01 <80176975@qq.com>
// Fixed size, lock-free, support multithreaded, circular queue
// especially suitable for one producer and mutiple consumer
// if one producer, Single_Push and Single_Pop is better
// otherwise, use Multi_Func


#include <stdio.h>
#include <sched.h>

#include "lock_free_queue.h"

template <class T>
LockFreeQueue<T>::LockFreeQueue()
{
    m_read_idx = 0;
    m_write_idx = 0;
    m_max_read_idx = 0;
    m_size = 0;
    m_data = NULL;
}

template <class T>
LockFreeQueue<T>::~LockFreeQueue()
{}

template <class T>
bool LockFreeQueue<T>::Init(uint32_t size)
{
    if (size <= 0) {
        return false;
    }
    m_size = size + 1;
    m_data = new T[m_size];
    if (NULL == m_data) {
        return false;
    }
    return true;
}

template <class T>
uint32_t LockFreeQueue<T>::GetIdx(uint32_t idx)
{
    return (idx % m_size);
}

template <class T>
bool LockFreeQueue<T>::Multi_Push(const T& node)
{
    uint32_t cur_read_idx;
    uint32_t cur_write_idx;
    uint32_t new_write_idx;
    do {
        cur_read_idx = m_read_idx;
        cur_write_idx = m_write_idx;
        new_write_idx = GetIdx(cur_write_idx + 1);
        if (new_write_idx == GetIdx(cur_read_idx)) {
            // full
            return false;
        }
    } while (!CAS(&m_write_idx, cur_write_idx, new_write_idx));
    m_data[cur_write_idx] = node;

    while (!CAS(&m_max_read_idx, cur_write_idx, new_write_idx)) {
        sched_yield();
    }
    return true;
}

template <class T>
bool LockFreeQueue<T>::Single_Push(const T& node)
{
    uint32_t cur_read_idx;
    uint32_t cur_write_idx;
    uint32_t new_write_idx;
    cur_read_idx = m_read_idx;
    cur_write_idx = m_write_idx;
    new_write_idx = GetIdx(cur_write_idx + 1);
    if (new_write_idx == GetIdx(cur_read_idx)) {
        // full
        return false;
    }
    m_data[cur_write_idx] = node;
    // atomic m_write_idx = new_write_idx
    __sync_val_compare_and_swap(&m_write_idx, m_write_idx, new_write_idx);
    return true;
}

template <class T>
bool LockFreeQueue<T>::Multi_Pop(T& node)
{
    uint32_t cur_read_idx;
    uint32_t cur_max_read_idx;
    uint32_t new_read_idx;
    do {
        cur_read_idx = m_read_idx;
        cur_max_read_idx = m_max_read_idx;
        new_read_idx = GetIdx(cur_read_idx + 1);
        if (cur_read_idx == cur_max_read_idx) {
            // empty or not commit
            return false;
        }
        node = m_data[cur_read_idx];
    } while (!CAS(&m_read_idx, cur_read_idx, new_read_idx));
    return true;
}

template <class T>
bool LockFreeQueue<T>::Single_Pop(T& node)
{
    uint32_t cur_read_idx;
    uint32_t cur_max_read_idx;
    uint32_t new_read_idx;
    do {
        cur_read_idx = m_read_idx;
        cur_max_read_idx = m_write_idx;
        new_read_idx = GetIdx(cur_read_idx + 1);
        if (cur_read_idx == cur_max_read_idx) {
            // empty or not commit
            return false;
        }
        node = m_data[cur_read_idx];
    } while (!CAS(&m_read_idx, cur_read_idx, new_read_idx));
    return true;
}

template <class T>
void LockFreeQueue<T>::Release()
{
    if (NULL != m_data) {
        delete[] m_data;
        m_data = NULL;
    }
    return;
}
