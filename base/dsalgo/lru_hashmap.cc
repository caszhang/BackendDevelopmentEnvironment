// Author: zhangguoqiang01 <80176975@qq.com>
// support lru, especially suitable for 64-bit system
#ifndef LRU_HASHMAP_CC_H_
#define LRU_HASHMAP_CC_H_
#include "lru_hashmap.h"

template <class K, class V>
LruHashMap<K, V>::LruHashMap()
{
    m_max_entries = 0;
    m_table = NULL;
    m_table_size = 0;
    m_node_num = 0;
    m_entry_array = NULL;
    m_entry_queue = NULL;
    m_header = NULL;
}

template <class K, class V>
LruHashMap<K, V>::~LruHashMap()
{}

template <class K, class V>
bool LruHashMap<K, V>::Init(int32_t size)
{
    if (size <= 0) {
        return false;
    }
    m_max_entries = size;

    m_entry_queue = new EntryQueue<int32_t>;
    m_entry_queue->Init(m_max_entries + 2);

    m_entry_array = new Entry<K, V>[m_max_entries + 2];
    for (int32_t i = 0; i < m_max_entries + 2; i++) {
        m_entry_queue->Push(i);
    }


    if (size > kMaxTableSize) {
        size = kMaxTableSize;
    }
    m_table_size = size;
    m_table = new int32_t[m_table_size];
    for (int32_t i = 0; i < m_table_size; ++i) {
        m_table[i] = -1;
    }

    int32_t idx = -1;
    m_entry_queue->Pop(idx);
    if (-1 == idx) {
        return false;
    }
    m_header = &m_entry_array[idx];
    m_header->before = idx;
    m_header->after = idx;
    m_header->index = idx;

    return true;
}

template <class K, class V>
bool LruHashMap<K, V>::Reset()
{
    m_entry_queue->Reset();
    for (int32_t i = 0; i < m_max_entries + 2; i++) {
        m_entry_queue->Push(i);
        m_entry_array[i].next = -1;
        m_entry_array[i].before = -1;
        m_entry_array[i].after = -1;
        m_entry_array[i].index = -1;
    }


    for (int32_t i = 0; i < m_table_size; ++i) {
        m_table[i] = -1;
    }

    int32_t idx = -1;
    m_entry_queue->Pop(idx);
    if (-1 == idx) {
        return false;
    }
    m_header = &m_entry_array[idx];
    m_header->before = idx;
    m_header->after = idx;
    m_header->index = idx;

    m_node_num = 0;
    return true;
}

template <class K, class V>
bool LruHashMap<K, V>::Get(K key, V &value, bool is_lru)
{
    int32_t idx = GetBucketIndex(GetHashCode(key));
    for (int32_t e = m_table[idx]; e != -1; e = m_entry_array[e].next) {
        if (m_entry_array[e].key == key) {
            value = m_entry_array[e].value;
            RecordAccess(&m_entry_array[e], is_lru);
            return true;
        }
    }

    return false;
}

template <class K, class V>
bool LruHashMap<K, V>::Get(K key, V* &value, bool is_lru)
{
    int32_t idx = GetBucketIndex(GetHashCode(key));
    for (int32_t e = m_table[idx]; e != -1; e = m_entry_array[e].next) {
        if (m_entry_array[e].key == key) {
            value = &(m_entry_array[e].value);
            RecordAccess(&m_entry_array[e], is_lru);
            return true;
        }
    }

    return false;
}

template <class K, class V>
bool LruHashMap<K, V>::Put(K key, V value, bool is_lru)
{
    int32_t idx = GetBucketIndex(GetHashCode(key));
    for (int32_t e = m_table[idx]; e != -1; e = m_entry_array[e].next) {
        if (m_entry_array[e].key == key) {
            m_entry_array[e].value = value;
            RecordAccess(&m_entry_array[e], is_lru);
            return true;
        }
    }
    if (!AddEntry(key, value, idx)) {
        return false;
    }
    return true;
}

template <class K, class V>
bool LruHashMap<K, V>::RecordAccess(Entry<K, V> *e, bool is_lru)
{
    if (is_lru) {
        AccessRemove(e);
        AccessAddBefore(e);
    }
    return true;
}


template <class K, class V>
bool LruHashMap<K, V>::AddEntry(K key, V value, int32_t idx)
{
    int32_t new_idx = -1;
    m_entry_queue->Pop(new_idx);
    if (-1 == new_idx) {
        return false;
    }
    m_entry_array[new_idx].key = key;
    m_entry_array[new_idx].value = value;
    m_entry_array[new_idx].index = new_idx;
    m_entry_array[new_idx].next = m_table[idx];

    m_table[idx] = new_idx;

    AccessAddBefore(&m_entry_array[new_idx]);

    m_node_num++;

    int32_t eldest = m_header->after;
    if (m_node_num > m_max_entries) {
        Remove(m_entry_array[eldest].key);
    }
    return true;
}

template <class K, class V>
bool LruHashMap<K, V>::GetFutureRemoveKey(K &key, V &value)
{
    int32_t eldest = m_header->after;
    key = m_entry_array[eldest].key;
    value = m_entry_array[eldest].value;
    return true;
}

template <class K, class V>
bool LruHashMap<K, V>::IsFull()
{
    if (m_node_num >= m_max_entries) {
        return true;
    }
    return false;
}

template <class K, class V>
bool LruHashMap<K, V>::GetLatestNodeKey(K* &key)
{
    key = NULL;
    if (0 == m_node_num) {
        return false;
    }

    key = &(m_entry_array[m_header->before].key);
    return true;
}

template <class K, class V>
bool LruHashMap<K, V>::GetLatestNodeValue(V* &value)
{
    value = NULL;
    if (0 == m_node_num) {
        return false;
    }
    value = &(m_entry_array[m_header->before].value);
    return true;
}

template <class K, class V>
bool LruHashMap<K, V>::GetLatestNKeyValue(K** key, V** value, int *num)
{
    if (0 == m_node_num) {
        return false;
    }
    if (*num > m_node_num) {
        *num = m_node_num;
    }

    int before = m_header->before;
    for (int i = 0; i < *num; i++) {
        key[i] = &(m_entry_array[before].key);
        value[i] = &(m_entry_array[before].value);
        before = m_entry_array[before].before;
    }
    return true;
}

template <class K, class V>
bool LruHashMap<K, V>::AccessAddBefore(Entry<K, V> *e)
{
    e->after = m_header->index;
    e->before = m_header->before;
    m_entry_array[e->before].after = e->index;
    m_entry_array[e->after].before = e->index;
    return true;
}

template <class K, class V>
bool LruHashMap<K, V>::Remove(K key)
{
    int32_t idx = GetBucketIndex(GetHashCode(key));
    int32_t pre = m_table[idx];
    int32_t cur = pre;
    while (cur != -1) {
        int32_t next = m_entry_array[cur].next;
        if (m_entry_array[cur].key == key) {
            m_node_num--;
            if (pre == cur) {
                m_table[idx] = next;
            } else {
                m_entry_array[pre].next = next;
            }
            AccessRemove(&m_entry_array[cur]);
            m_entry_queue->Push(cur);
            return true;
        }
        pre = cur;
        cur = next;
    }
    return false;
}

template <class K, class V>
bool LruHashMap<K, V>::AccessRemove(Entry<K, V> *e)
{
    m_entry_array[e->before].after = e->after;
    m_entry_array[e->after].before = e->before;
    return true;
}

template <class K, class V>
int32_t LruHashMap<K, V>::GetBucketIndex(uint32_t hascode)
{
    hascode ^= (hascode >> 20) ^ (hascode >> 12);
    uint32_t temp = hascode ^ (hascode >> 7) ^ (hascode >> 4);
    return temp % m_table_size;
}

template <class K, class V>
uint32_t LruHashMap<K, V>::GetHashCode(K key)
{
    return static_cast<uint32_t>(key);
}

template <class K, class V>
bool LruHashMap<K, V>::Release()
{
    if (NULL != m_table) {
        delete[] m_table;
        m_table = NULL;
    }

    if (NULL != m_entry_array) {
        delete[] m_entry_array;
        m_entry_array = NULL;
    }

    if (NULL != m_entry_queue) {
        m_entry_queue->Release();
        delete m_entry_queue;
        m_entry_queue = NULL;
    }

    return true;
}

template <class K, class V>
int32_t LruHashMap<K, V>::GetSize()
{
    return m_node_num;
}

template <class K, class V>
int32_t LruHashMap<K, V>::GetMaxNodeNum()
{
    return m_max_entries;
}

#endif // LRU_HASHMAP_CC_H_
