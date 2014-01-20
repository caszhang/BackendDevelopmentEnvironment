// Author: zhangguoqiang01 <80176975@qq.com>

#ifndef SKIPLIST_H_
#define SKIPLIST_H_
#include <stdint.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define MAX_SKIP_LEVEL 6
#define LEVEL_WT 6
template <class K, class V>
struct SkipListNode {
    K key;
    V value;
    int32_t level;
    SkipListNode *forward[MAX_SKIP_LEVEL];
};

template <class K, class V>
class SkipList
{
public:
    SkipList();
    ~SkipList();
public:
    bool Init();
    bool Insert(K key, V value);
    bool Delete(K key);
    int32_t GetLevel();
    bool Find(K key, V &value);
    void PrintInfo();
    bool Release();
    int32_t GetSize();
public:
    SkipListNode<K, V> *m_head;
    int32_t m_size;
};
#endif
