// Author: zhangguoqiang01 <80176975@qq.com>
// store the free session
#ifndef SESSION_POOL_H_
#define SESSION_POOL_H_
#include <stdint.h>
#include "common_queue.cc"

template <class S>
class SessionPool
{
public:
    SessionPool();
    ~SessionPool();
public:
    bool Init(int32_t pool_size);
    void Release();
public:
    bool GetSession(S **session);
    bool ReleaseSession(S *session);
public:
    int32_t GetSize();
    int32_t GetNodeNum();
public:
    S *m_session_list;
    CommonQueue<S*> *m_session_queue;
};
#endif // SESSION_POOL_H_
