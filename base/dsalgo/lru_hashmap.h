// Author: zhangguoqiang01 <80176975@qq.com>
// support lru, especially suitable for 64-bit system
#ifndef LRU_HASHMAP_H_
#define LRU_HASHMAP_H_
#include <stdint.h>
#include "entry.h"

template <class K, class V>
class LruHashMap
{
public:
    LruHashMap();
    ~LruHashMap();
public:
    bool Init(int32_t size);
    bool Put(K key, V value, bool is_lru);
    bool Remove(K key);
    bool Get(K key, V &value, bool is_lru);
    bool Get(K key, V* &value, bool is_lru);
    bool Reset();
    bool Release();
    bool GetLatestNodeValue(V* &value);
    bool IsFull();
    bool GetLatestNodeKey(K* &key);
    bool GetLatestNKeyValue(K** key, V** value, int *num);
private:
    bool RecordAccess(Entry<K, V> *e, bool is_lru);
    bool AddEntry(K key, V value, int32_t idx);
    bool AccessAddBefore(Entry<K, V> *e);
    bool AccessRemove(Entry<K, V> *e);
    int32_t GetBucketIndex(uint32_t hascode);
    uint32_t GetHashCode(K key);
public:
    int32_t GetSize();
    int32_t GetMaxNodeNum();
private:
    static const int32_t kMaxTableSize = 1 << 30;
    int32_t m_max_entries;
    int32_t *m_table;
    Entry<K, V> *m_entry_array;
    EntryQueue<int32_t> *m_entry_queue;
    int32_t m_table_size;
    int32_t m_node_num;
    Entry<K, V> *m_header;
};
#endif // LRU_HASHMAP_H_
