// Author: zhangguoqiang01 <80176975@qq.com>

#include "hashmap.h"

template <class Key, class Value>
HashMap<Key, Value>::HashMap()
{
    m_size = 0;
    m_bucket_num = 0;
    m_array = NULL;
}

template <class Key, class Value>
HashMap<Key, Value>::~HashMap()
{}

template <class Key, class Value>
bool HashMap<Key, Value>::Init(int32_t bucket_num)
{
    if (bucket_num <= 0) {
        return false;
    }

    if (bucket_num > kMaxBucketNum) {
        bucket_num = kMaxBucketNum;
    }

    int32_t temp_num = 1;
    while (temp_num < bucket_num) {
        temp_num <<= 1;
    }
    m_bucket_num = temp_num;
    m_array = new LinkedList<Key, Value>[m_bucket_num];
    return true;
}

template <class Key, class Value>
bool HashMap<Key, Value>::Release()
{
    for (int32_t i = 0; i < m_bucket_num; i++) {
        m_array[i].Release();
    }
    delete[] m_array;
    m_array = NULL;
    return true;
}
template <class Key, class Value>
bool HashMap<Key, Value>::Find(Key key, Value* &value)
{
    value = NULL;
    uint32_t hashcode = GetHashCode(key);
    int32_t bucket_index = GetBucketIndex(hashcode);
    bool ret = m_array[bucket_index].Find(key, value);
    if (!ret) {
        return false;
    }
    return true;
}
template <class Key, class Value>
bool HashMap<Key, Value>::Find(Key key, Value &value)
{
    uint32_t hashcode = GetHashCode(key);
    int32_t bucket_index = GetBucketIndex(hashcode);
    bool ret = m_array[bucket_index].Find(key, value);
    if (!ret) {
        return false;
    }
    return true;
}

template <class Key, class Value>
int32_t HashMap<Key, Value>::Insert(Key key, Value value)
{
    uint32_t hashcode = GetHashCode(key);
    int32_t bucket_index = GetBucketIndex(hashcode);
    int32_t ret = m_array[bucket_index].Insert(key, value);
    if (1 == ret) {
        m_size++;
    }
    return ret;
}
template <class Key, class Value>
bool HashMap<Key, Value>::Delete(Key key)
{
    uint32_t hashcode = GetHashCode(key);
    int32_t bucket_index = GetBucketIndex(hashcode);
    bool ret = m_array[bucket_index].Delete(key);
    if (!ret) {
        return false;
    }
    m_size--; 
    return true;
}
template <class Key, class Value>
int32_t HashMap<Key, Value>::GetBucketIndex(uint32_t hascode)
{
    hascode ^= (hascode >> 20) ^ (hascode >> 12);
    uint32_t temp = hascode ^ (hascode >> 7) ^ (hascode >> 4);
    return temp & (m_bucket_num - 1);
}

template <class Key, class Value>
uint32_t HashMap<Key, Value>::GetHashCode(Key key)
{
    return (uint32_t)key;
}

template <class Key, class Value>
uint32_t HashMap<Key, Value>::GetSize()
{
    return m_size;
}
