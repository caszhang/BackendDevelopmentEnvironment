// All rights reserved.
// Author: zhangguoqiang01 <80176975@qq.com>
// Element for lru hashmap, especially suitable for 64-bit system
#ifndef ENTRY_H_
#define ENTRY_H_
#include <stdio.h>
#include <stdint.h>

template <class K, class V>
class Entry
{
public:
    K key;
    V value;
    int32_t next;
    int32_t before;
    int32_t after;
    int32_t index;

    Entry() {
        next = -1;
        before = -1;
        after = -1;
        index = -1;
    }
};

template <class T>
class EntryQueue
{
public:
    T *m_data;
    uint32_t m_tail;
    uint32_t m_head;
    uint32_t m_size;

    EntryQueue() {
        m_data = NULL;
        m_tail = 0;
        m_head = 0;
        m_size = 0;
    }

    bool Init(uint32_t size) {
        m_size = size;
        m_data = new T[m_size];
        if (NULL == m_data) {
            return false;
        }
        memset(m_data, 0, m_size * sizeof(T));
        return true;
    }

    bool Release() {
        if (m_data) {
            delete[] m_data;
            m_data = NULL;
        }
        return true;
    }

    bool Push(const T& node) {
        m_data[m_tail++] = node;
        m_tail = m_tail % m_size;
        return true;
    }

    bool Pop(T& node) {
        node = m_data[m_head++];
        m_head = m_head % m_size;
        return true;
    }

    bool Reset() {
        m_tail = 0;
        m_head = 0;
        return true;
    }
};
#endif // ENTRY_H_
