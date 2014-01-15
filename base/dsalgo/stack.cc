// All rights reserved.
// Author: zhangguoqiang01 <80176975@qq.com>

#include "stack.h"

template <class E>
Stack<E>::Stack()
{
    m_data = NULL;
    m_top = 0;
}

template <class E>
Stack<E>::~Stack()
{
}

template <class E>
bool Stack<E>::Init(int32_t size)
{
    m_data = new E[size];
    if (NULL == m_data) {
        return false;
    }
    m_top = 0;
    m_size = size;
    return true;
}

template <class E>
bool Stack<E>::GetTop(E& element)
{
    if (false == IsEmpty()) {
        element = m_data[m_top - 1];
        return true;
    }
    return false;
}

template <class E>
bool Stack<E>::Push(E element)
{
    if (false == IsFull()) {
        m_data[m_top++] = element;
        return true;
    }
    return false;
}

template <class E>
bool Stack<E>::Pop(E& element)
{
    if (false == IsEmpty()) {
        element = m_data[--m_top];
        return true;
    }
    return false;
}

template <class E>
bool Stack<E>::IsFull()
{
    if (m_top == m_size) {
        return true;
    }
    return false;
}

template <class E>
bool Stack<E>::IsEmpty()
{
    if (0 == m_top) {
        return true;
    }
    return false;
}

template <class E>
bool Stack<E>::Clear()
{
    m_top = 0;
    return true;
}

template <class E>
int32_t Stack<E>::GetSize()
{
    return m_top;
}

template <class E>
bool Stack<E>::Release()
{
    m_top = 0;
    if (NULL != m_data) {
        delete[] m_data;
        m_data = NULL;
    }
    return true;
}
