// All rights reserved.
// Author: zhangguoqiang01 <80176975@qq.com>

#include "skiplist.h"

template <class K, class V>
SkipList<K, V>::SkipList()
{
    m_head = NULL;
    m_size = 0;
}

template <class K, class V>
SkipList<K, V>::~SkipList()
{}

template <class K, class V> 
bool SkipList<K, V>::Init()
{
    m_head = new SkipListNode<K, V>;
    m_head->level = MAX_SKIP_LEVEL - 1;
    for (int32_t i = 0; i < MAX_SKIP_LEVEL; ++i) {
        m_head->forward[i] = NULL;
    }
    srand((unsigned)time(NULL)); 
    return true;
}

template <class K, class V> 
bool SkipList<K, V>::Insert(K key, V value)
{
    if (NULL == m_head) {
        return false;
    }
    SkipListNode<K, V> *cur = m_head;
    SkipListNode<K, V> *update[MAX_SKIP_LEVEL];
    for (int32_t i = cur->level; i >= 0; i--) {
        while (cur->forward[i] != NULL && key > cur->forward[i]->key) {
            cur = cur->forward[i];
        }
        update[i] = cur;
    }
    if (NULL != cur->forward[0]) {
        cur = cur->forward[0];
    }
    if (cur != m_head && key == cur->key) {
        cur->value = value;
        return true;
    }
    else {
        //new
        SkipListNode<K, V> * new_node = new SkipListNode<K, V>;
        new_node->key = key;
        new_node->value = value;
        new_node->level = GetLevel();
        //printf("%d\n", new_node->level);
        for (int32_t i = 0; i < MAX_SKIP_LEVEL; ++i) {
            if (i <= new_node->level) {
                new_node->forward[i] = update[i]->forward[i];
                update[i]->forward[i] = new_node;
                continue;
            }
            new_node->forward[i] = NULL;
        }
        m_size++;
    }
    return true;
}

template <class K, class V>
int32_t SkipList<K, V>::GetLevel()
{
   int32_t level = 0;
   for (int32_t i = 0; i < MAX_SKIP_LEVEL - 1; i++) {
       level += (rand() % 10) > LEVEL_WT ? 1 : 0; 
   }
   return level;
}

template <class K, class V> 
bool SkipList<K, V>::Find(K key, V &value)
{
    SkipListNode<K, V> *cur = m_head;
    for (int32_t i = cur->level; i >= 0; i--) {
        while (cur->forward[i] != NULL && key > cur->forward[i]->key) {
            cur = cur->forward[i];
        }
    }
    if (NULL != cur->forward[0]) {
        cur = cur->forward[0];
    }
    if (cur != m_head && key == cur->key) {
        value = cur->value;
        return true;
    }
    return false;
}

template <class K, class V>
bool SkipList<K, V>::Delete(K key)
{
    if (NULL == m_head) {
        return false;
    }
    SkipListNode<K, V> *cur = m_head;
    SkipListNode<K, V> *update[MAX_SKIP_LEVEL];
    for (int32_t i = cur->level; i >= 0; i--) {
        while (cur->forward[i] != NULL && key > cur->forward[i]->key) {
            cur = cur->forward[i];
        }
        update[i] = cur;
    }
    if (NULL != cur->forward[0]) {
        cur = cur->forward[0];
    }
    if (cur != m_head && key == cur->key) {
        for (int32_t i = 0; i <= m_head->level; ++i) {
            if (update[i]->forward[i] != cur) {
                break;
            }
            update[i]->forward[i] = cur->forward[i];
        }
        delete cur;
        cur = NULL; 
        m_size--;
        return true;
    }
    return false;
}

template <class K, class V>
bool SkipList<K, V>::Release()
{
    SkipListNode<K, V> *cur = m_head;
    SkipListNode<K, V> *next = NULL;
    while (cur != NULL) {
        next = cur->forward[0];
        delete cur;
        cur = next;
    }
    m_head = NULL;
    return true;
}

template <class K, class V> 
void SkipList<K, V>::PrintInfo()
{
    SkipListNode<K, V> *cur = NULL;
    for (int32_t i = 0; i <= m_head->level; ++i) {
       cur = m_head->forward[i];
       printf("level[%d]:", i);
       while (cur != NULL) {
           printf("%u->", (uint32_t)cur->key);
           cur = cur->forward[i];
       }
       printf("NULL\n");
    }
}

template <class K, class V>
int32_t SkipList<K, V>::GetSize()
{
    return m_size;
}
