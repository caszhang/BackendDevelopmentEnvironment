// Author: zhangguoqiang01 <80176975@qq.com>
#ifndef STACK_H_
#define STACK_H_
#include <stdio.h>
#include <stdint.h>

template <class E>
class Stack
{
public:
    Stack();
    ~Stack();
public:
    bool Init(int32_t size);
    bool Release();
public:
    bool IsFull();
    bool IsEmpty();
    bool GetTop(E& element);
    bool Push(E element);
    bool Pop(E& element);
    bool Clear();
    int32_t GetSize();
private:
    int32_t m_top;
    int32_t m_size;
    E *m_data;
};
#endif
