// All rights reserved.
// Author: zhangguoqiang01 <80176975@qq.com>
// LinkedList for hashmap
#include "linkedlist.h"

template <class Key, class Value>
LinkedList<Key, Value>::LinkedList()
{
    head = NULL;
}

template <class Key, class Value>
LinkedList<Key, Value>::~LinkedList()
{}

template <class Key, class Value>
bool LinkedList<Key, Value>::Find(Key key, Value* &value)
{
    value = NULL;
    Node<Key, Value> *cur = head;
    while (NULL != cur) {
        if (cur->key == key) {
            value = &(cur->value);
            return true;
        }
        cur = cur->next;
    }
    return false;
}

template <class Key, class Value>
bool LinkedList<Key, Value>::Find(Key key, Value &value)
{
    Node<Key, Value> *cur = head;
    while (NULL != cur) {
        if (cur->key == key) {
            value = cur->value;
            return true;
        }
        cur = cur->next;
    }
    return false;
}

template <class Key, class Value>
int32_t LinkedList<Key, Value>::Insert(Key key, Value value)
{
    if (NULL == head) {
        head = new Node<Key, Value>;
        head->key = key;
        head->value = value;
        head->next = NULL;
        return 1;
    }

    Node<Key, Value> *cur = head;
    while (NULL != cur) {
        if (cur->key == key) {
            cur->value = value;
            return 2;
        }
        cur = cur->next;
    }
    Node<Key, Value> *temp = new Node<Key, Value>;
    temp->key = key;
    temp->value = value;
    temp->next = head;
    head = temp;
    return 1;
}

template <class Key, class Value>
bool LinkedList<Key, Value>::Delete(Key key)
{
    if (NULL == head) {
        return false;
    }
    if (head->key == key) {
        Node<Key, Value> *pre = head;
        head = head->next;
        delete pre;
        pre = NULL;
        return true;
    }
    Node<Key, Value> *pre = head;
    Node<Key, Value> *cur = head->next;
    while (NULL != cur) {
        if (cur->key == key) {
            pre->next = cur->next;
            delete cur;
            cur = NULL;
            return true;
        }
        pre = pre->next;
        cur = cur->next;
    }

    return false;
}

template <class Key, class Value> 
bool LinkedList<Key, Value>::Release()
{
    Node<Key, Value> *pre = head;
    Node<Key, Value> *next = NULL;
    while (NULL != pre) {
        next = pre->next;
        delete pre;
        pre = next;
    }

    return true;
}
