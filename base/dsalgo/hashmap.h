// Author: zhangguoqiang01 <80176975@qq.com>

#ifndef HASHMAP_H_
#define HASHMAP_H_
#include <stdint.h>
#include "linkedlist.cc"
template <class Key, class Value>
class HashMap
{
public:
    HashMap();
    ~HashMap();
public:
    bool Init(int32_t bucket_num);
    bool Release();
public:
    bool Find(Key key, Value &value);
    bool Find(Key key, Value* &value);
    int32_t Insert(Key key, Value value);
    bool Delete(Key key);
    int32_t GetBucketIndex(uint32_t hascode);
public:
    static const int32_t kMaxBucketNum = 1 << 30;
public:
    uint32_t GetHashCode(Key);
    uint32_t GetSize();
private:
    uint32_t m_size;
    int32_t m_bucket_num;
    LinkedList<Key, Value> *m_array;
};
#endif
