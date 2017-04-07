/*******************************************************************************
 * Author :          guoqiang
 * Email :           guoqiang@foxea.com
 * Last modified :   2017-03-29 19:32
 * Filename :        xserver.h
 * Description :     
 * *****************************************************************************/
#ifndef XSERVER_H_
#define XSERVER_H_
#include <arpa/inet.h>
#include <errno.h>
#include <event.h>
#include <evhttp.h>
#include <event2/event.h>
#include <event2/http.h>
#include <event2/buffer.h>
#include <event2/util.h>
#include <event2/keyvalq_struct.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

class XServer;
typedef struct _CONN CONN;
typedef struct _CONN_LIST CONN_LIST;
typedef struct _WORKER WORKER;
typedef struct _SERVER SERVER;
const uint16_t kConnBufLen = 2048;

struct _CONN {
    int m_idx;
    _CONN *m_next;
    struct bufferevent *m_bufev;
    WORKER *m_owner;
    uint32_t m_cip;
    uint16_t m_cport;
    int m_fd;
    char m_in_buf[kConnBufLen];
    uint16_t m_in_buf_len;

    _CONN() {
        m_idx = 0;
        m_next = NULL;
        m_bufev = NULL;
        m_owner = NULL;
        m_cip = 0;
        m_cport = 0;
        m_fd = -1;
        m_in_buf[0] = '\0';
        m_in_buf_len = 0;
    }

    ~_CONN() {
        m_idx = 0;
        m_next = NULL;
        m_owner = NULL;
        m_cip = 0;
        m_cport = 0;
        m_fd = -1;
        m_in_buf[0] = '\0';
        m_in_buf_len = 0;
        if (NULL != m_bufev) {
            bufferevent_free(m_bufev);
            m_bufev = NULL;
        }
    }
};

struct _CONN_LIST {
    CONN *m_head;
    CONN *m_tail;
    CONN *m_array;

    _CONN_LIST() {
        m_head = NULL;
        m_tail = NULL;
        m_array = NULL;
    }

    ~_CONN_LIST() {
        m_head = NULL;
        m_tail = NULL;
        if (NULL != m_array) {
            delete[] m_array;
            m_array = NULL;
        }
    }

};

struct _WORKER {
    struct event_base *m_base;
    struct event m_notify_event;
    int m_notified_rfd;
    int m_notifed_wfd;
    CONN_LIST *m_conn_list;
    pthread_t m_thread_id;
    SERVER *m_server;

    _WORKER() {
        m_base = NULL;
        m_notified_rfd = -1;
        m_notifed_wfd = -1;
        m_conn_list = NULL;
        m_server = NULL;
    }

    ~_WORKER() {
        m_base = NULL;
        m_notified_rfd = -1;
        m_notifed_wfd = -1;
        m_server = NULL;
        if (NULL != m_conn_list) {
            delete m_conn_list;
            m_conn_list = NULL;
        }
    }

};

struct _SERVER {
    int32_t m_fd;
    char m_ip[32];
    int32_t m_port;
    struct event_base *m_base;
    struct event m_listen_event;
    uint16_t m_start_worker_num;
    pthread_mutex_t m_start_lock;
    pthread_cond_t m_start_cond;
    XServer *m_xserver;
    
    _SERVER() {
        m_fd = -1;
        m_ip[0] = '\0';
        m_port = 0;
        m_base = NULL;
        m_start_worker_num = 0;
        m_xserver = NULL;
    }

    ~_SERVER() {
        m_fd = -1;
        m_ip[0] = '\0';
        m_port = 0;
        m_base = NULL;
        m_start_worker_num = 0;
        m_xserver = NULL;
    }

};

class XServer
{
public:
    XServer();
    virtual ~XServer();

public:
    bool Init(const char *ip, const int32_t port, const uint16_t work_num, const uint32_t conn_num_per_worker, const int32_t read_timeout, const int32_t write_timeout);
    bool Release();
    bool StartServer();
    bool StartWorkers();
    bool Start();
    bool Stop();
public:
    SERVER *m_server;
    WORKER **m_workers;
    uint16_t m_work_num;
    uint16_t m_work_idx;
    pthread_t m_thread_id;
public:
    virtual bool CheckValid(CONN *conn) = 0;
    virtual void Process(CONN *conn) = 0;
};
#endif // XSERVER_H_
