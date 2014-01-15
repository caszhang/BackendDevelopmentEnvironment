// All rights reserved.
// Author: zhangguoqiang01 <80176975@qq.com>
// LinkedList for hashmap

#ifndef LINKEDLIST_H_
#define LINKEDLIST_H_
#include <stdio.h>
#include <stdint.h>
template <class Key, class Value>
struct Node {
    Key key;
    Value value;
    Node *next;
};

template <class Key, class Value>
class LinkedList
{
public:
    LinkedList();
    ~LinkedList();
public:
    bool Release();
    bool Find(Key key, Value &value);
    bool Find(Key key, Value* &value);
    int32_t Insert(Key key, Value value);
    bool Delete(Key key);
private:
    Node<Key, Value> *head;
};
#endif
