// All rights reserved.
// Author: zhangguoqiang01 <80176975@qq.com>
// store the free session
#ifndef SESSION_POOL_CC_H_
#define SESSION_POOL_CC_H_
#include "session_pool.h"

template <class S>
SessionPool<S>::SessionPool()
{
    m_session_list = NULL;
    m_session_queue = NULL;
}

template <class S>
SessionPool<S>::~SessionPool()
{}

template <class S>
bool SessionPool<S>::Init(int32_t pool_size)
{
    m_session_list = new S[pool_size];
    m_session_queue = new CommonQueue<S*>;
    if (!m_session_queue->Init(pool_size)) {
        return false;
    }
    for (int32_t i = 0; i < pool_size; ++i) {
        m_session_queue->WaitTillPush(&m_session_list[i]);
    }
    return true;
}

template <class S>
bool SessionPool<S>::GetSession(S **session)
{
    if (NULL == session || false == m_session_queue->WaitTillPop(*session)) {
        return false;
    }
    return true;
}

template <class S>
bool SessionPool<S>::ReleaseSession(S *session)
{
    if (NULL == session || false == m_session_queue->WaitTillPush(session)) {
        return false;
    }
    return true;
}

template <class S>
void SessionPool<S>::Release()
{
    if (NULL != m_session_queue) {
        m_session_queue->Release();
        delete m_session_queue;
        m_session_queue = NULL;
    }
    if (NULL != m_session_list) {
        delete[] m_session_list;
        m_session_list = NULL;
    }
    return;
}

template <class S>
int32_t SessionPool<S>::GetSize()
{
    return static_cast<int32_t>(m_session_queue->GetSize());
}

template <class S>
int32_t SessionPool<S>::GetNodeNum()
{
    return static_cast<int32_t>(m_session_queue->GetNodeNum());
}

#endif // SESSION_POOL_CC_H_
