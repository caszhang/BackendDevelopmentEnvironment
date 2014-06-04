/*******************************************************************************
 * Author :          guoqiang
 * Email :           guoqiang@foxea.com
 * Last modified :   2014-06-03 19:39
 * Filename :        simple_server.h
 * Description :     
 * *****************************************************************************/
#ifndef __SIMPLE_SEVER_H_
#define __SIMPLE_SEVER_H_

#include "dsalgo/head.h"
#include "os/thread_base.h"
#include "os/thread_pool.h"

struct ThreadArgument {
    void *m_instance;
    int m_socket;

    ThreadArgument & operator = (ThreadArgument &item) {
        m_instance = item.m_instance;
        m_socket = item.m_socket;
        return *this;
    }
};

class SimpleServer:public ThreadBase
{
public:
    SimpleServer();
    virtual ~SimpleServer();
    bool Init(const char *ip, const uint16_t port, int threads_num = 8);
    bool InitNet();
    void SetNonBlocking(int sock);
    bool InitEpoll();
    void Release();
    virtual void Run();
    virtual void UnRun();
    virtual void ProcessSocket(int socket) = 0;
private:
    char m_ip[32];
    uint16_t m_port;
    int32_t m_socket;
    int32_t m_epoll_fd;
    bool m_continue;
    int m_threads_num;
    ThreadPool *m_thread_pool;
};
#endif // __SIMPLE_SEVER_H_
