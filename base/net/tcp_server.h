// Author: zhangguoqiang01 <80176975@qq.com>
// generally speaking, there are four kinds of net modle:
// 1,one thread per connection
// 2,one thread which belongs to thread pool per connection 
// 3,asynchronous IO + IO multiplexing
// 4,leader + follower
// the file belongs to 3 & 4, the listen thread is leader, work thread is follwer
#ifndef  __TCP_SERVER_H_
#define  __TCP_SERVER_H_
#include "dsalgo/hashmap.cc"
#include "dsalgo/head.h"
#include "os/lock.h"

const int32_t kDefaultMaxEpollFdSize = 8;
const int32_t kDefaultMaxWorkersPerEpoll = 1;
const int32_t kMaxListenHostSize = 24;
const int32_t kMaxConnectionBufSize = 1024 * 64;
const int32_t kMaxConnectionNum = 65536;
const int32_t kMaxBucketNum = 1024;
struct Connection {
    int32_t m_socket;
    int32_t m_work_epoll_idx;
    int32_t m_using;
    char m_read_buf[kMaxConnectionBufSize];
    int32_t m_read_off;
    char m_write_buf[kMaxConnectionBufSize];
    int32_t m_write_off;

    Connection() {
        m_socket = -1;
        m_work_epoll_idx = -1;
        m_using = 0;
        m_read_buf[0] = '\0';
        m_read_off = 0;
        m_write_buf[0] = '\0';
        m_write_off = 0;
    }
};

class TcpServer
{
public:
    TcpServer();
    virtual ~TcpServer();
    bool Init(const char *ipv4, const uint16_t port, const int32_t work_epoll_fd_size = kDefaultMaxEpollFdSize, const int32_t workers_per_epoll = kDefaultMaxWorkersPerEpoll);
    void Release();
    void SetNonBlocking(int &sock);
    bool InitNet();
    bool CheckParameters(const char *ipv4, const uint16_t port, const int32_t work_epoll_fd_size, const int32_t workers_per_epoll) const;
    int32_t GetSocket() const;
    bool IsRunning() const;
    int32_t GetWorkEpollFd(int socket);
    bool InitWorkEpollFd();
    int32_t HandleReadEvent(int32_t sock);
    int32_t HandleWriteEvent(int32_t sock);
    void CloseConnection(int32_t sock);
    bool HasToWrite(int32_t sock);
    virtual int32_t DoRecv(int32_t socket, const char * buf, int32_t len);
    int32_t DoSend(int32_t sock, char *data, int32_t len);
    void Start();
    void Stop();
private:
    int32_t m_socket;
    char m_listen_host[kMaxListenHostSize];
    uint16_t m_listen_port;
    pthread_t m_listen_thread;//join
    bool m_continue;
    int32_t *m_work_epoll_fd;
    int32_t m_work_epoll_fd_idx;
    int32_t m_work_epoll_fd_size;
    pthread_t *m_work_thread;
    int32_t m_workers_per_epoll;
    //Connection m_connection[kMaxConnectionNum];
    Mutex m_conn_mutex[kMaxConnectionNum];
    //Mutex m_send_mutex;
    HashMap<int32_t, Connection> *m_conn_map;
    std::vector<int32_t> m_work_socket_list;
};
#endif  //__TCP_SERVER_H_

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
