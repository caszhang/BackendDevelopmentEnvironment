// Author: zhangguoqiang01 <80176975@qq.com>
#include "tcp_server.h"
const int32_t kMaxListenNum = 4096;
const int32_t kSendTimeout = 2000;
const int32_t kRecvTimeout = 2000;
const int32_t kTimeWaitOut = 1000;
const int32_t kMaxEpollListenNum = 20;
TcpServer *g_tcp_server = NULL;

void* listen_thread(void *param)
{
    TcpServer *tcp_server = (TcpServer *)param;
    // the number 20 has no real meaning, >0 is ok
    int32_t listen_epoll_fd = epoll_create(kMaxEpollListenNum);
    int32_t listen_socket = tcp_server->GetSocket();
    struct epoll_event ev_register;
    ev_register.events  = EPOLLIN;
    ev_register.data.fd = listen_socket;
    epoll_ctl(listen_epoll_fd, EPOLL_CTL_ADD, listen_socket, &ev_register);
    struct epoll_event event;
    
    // only handle listen socket
    socklen_t clilen;
    struct sockaddr_in clientaddr;
    int ret = 0;
    while (tcp_server->IsRunning()) {
        int32_t event_num = epoll_wait(listen_epoll_fd, &event, 1, kTimeWaitOut);
        if (event_num > 0) {
            int sock = accept(listen_socket, (sockaddr*)&clientaddr, &clilen);
            if (sock > 0) {
                tcp_server->SetNonBlocking(sock);
                ev_register.data.fd = sock;
                ev_register.events = EPOLLIN | EPOLLONESHOT; 
                // register to worker pool's epoll
                ret = epoll_ctl(tcp_server->GetWorkEpollFd(sock), EPOLL_CTL_ADD, sock, &ev_register);
                if (ret < 0) {
                    if (errno == EBADF) {
                        perror("EBADF"); 
                    } else if (errno == EEXIST) {
                        perror("EEXIST"); 
                    } else if (errno == EINVAL) {
                        perror("EINVAL"); 
                    } else if (errno == ENOENT) {
                        perror("ENOENT"); 
                    } else if (errno == ENOMEM) {
                        perror("ENOMEM"); 
                    } else if (errno == ENOSPC) {
                        perror("ENOSPC"); 
                    } else if (errno == EPERM) {
                        perror("EPERM"); 
                    }
                }
            }
        }
    }
    close(listen_epoll_fd);
    return NULL;
}

void* work_thread(void *param)
{
    int32_t work_epoll_fd = *((int32_t*)param);
    TcpServer *tcp_server = g_tcp_server;
    struct epoll_event ev_register;
    struct epoll_event event;
    while (tcp_server->IsRunning()) {
        int32_t event_num = epoll_wait(work_epoll_fd, &event, 1, kTimeWaitOut);
        if (event_num > 0) {
            int sock = event.data.fd;

            if ((event.events & EPOLLERR) ||
                (event.events & EPOLLHUP) ||
                (!(event.events & EPOLLIN))) {
                tcp_server->CloseConnection(sock);
                continue;
            }

            if (event.events & EPOLLOUT) {
                if (tcp_server->HandleWriteEvent(sock) == -1) {
                    tcp_server->CloseConnection(sock);
                    continue;
                }
            }

            if (event.events & EPOLLIN) {
                if (tcp_server->HandleReadEvent(sock) == -1) {
                    tcp_server->CloseConnection(sock);
                    continue;
                }
            }

            ev_register.events = EPOLLIN | EPOLLONESHOT;
            if (tcp_server->HasToWrite(sock)) {
                ev_register.events |= EPOLLOUT;
            }
            ev_register.data.fd = sock;
            epoll_ctl(work_epoll_fd, EPOLL_CTL_MOD, sock, &ev_register);
        }
    }

    return NULL;
}

TcpServer::TcpServer()
{
    m_socket = -1;
    m_listen_host[0] = '\0';
    m_listen_port = 0;
    m_continue = false;
    m_work_epoll_fd = NULL;
    m_work_epoll_fd_idx = 0;
    m_work_epoll_fd_size = 0;
    m_work_thread = NULL;
    m_workers_per_epoll = 0;
    m_conn_map = NULL;
    //m_work_socket_list = NULL;
}

TcpServer::~TcpServer()
{}

bool TcpServer::Init(const char *ipv4, const uint16_t port, const int32_t work_epoll_fd_size, const int32_t workers_per_epoll)
{
    if (!CheckParameters(ipv4, port, work_epoll_fd_size, workers_per_epoll)) {
        return false; 
    }
    m_conn_map = new HashMap<int32_t, Connection>;
    if (!m_conn_map->Init(kMaxBucketNum)) {
        return false; 
    }
    m_continue = true;
    g_tcp_server = this;

    snprintf(m_listen_host, kMaxListenHostSize, "%s", ipv4);
    m_listen_port = port;
    m_work_epoll_fd_size = work_epoll_fd_size;
    m_workers_per_epoll = workers_per_epoll;

    /*
    for (int i = 0; i < kMaxConnectionNum; i++) {
        m_connection[i].m_socket = i;
    }
    */

    if (!InitWorkEpollFd()) {
        return false; 
    }

    if (!InitNet()) {
        return false; 
    }

    //m_work_socket_list = new LinkedList<int32_t , char>;
    m_work_socket_list.clear();

    if (0 != pthread_create(&m_listen_thread, NULL, listen_thread, this)) {
        return false; 
    }
    return true;
}

bool TcpServer::InitWorkEpollFd()
{
    m_work_epoll_fd = new int32_t[m_work_epoll_fd_size];
    for (int i =  0; i < m_work_epoll_fd_size; i++) {
        m_work_epoll_fd[i] = epoll_create(kMaxEpollListenNum);
    }

    m_work_thread = new pthread_t[m_work_epoll_fd_size * m_workers_per_epoll];
    for (int i = 0; i < m_work_epoll_fd_size; i++) {
        for (int j = 0; j < m_workers_per_epoll; j++) {
            if (0 != pthread_create(&m_work_thread[i * m_workers_per_epoll + j], NULL, work_thread, &m_work_epoll_fd[i])) {
                return false; 
            }
        }
    }
    return true;
}

bool TcpServer::CheckParameters(const char *ipv4, const uint16_t port, const int32_t work_epoll_fd_size, const int32_t workers_per_epoll) const
{
    if (NULL == ipv4 || port < 1024 
        || work_epoll_fd_size < 1 
        || workers_per_epoll < 1) {
        return false; 
    }
    return true;
}

bool TcpServer::InitNet()
{
    m_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(m_socket == -1) {
        return false;
    }   

    struct timeval tv;
    tv.tv_sec  = kSendTimeout / 1000;
    tv.tv_usec = (kSendTimeout % 1000) * 1000;
    if(0 != setsockopt(m_socket, SOL_SOCKET, SO_SNDTIMEO, reinterpret_cast<char *>(&tv), sizeof(tv))) {
        return false;
    }

    tv.tv_sec  = kRecvTimeout / 1000;
    tv.tv_usec = (kRecvTimeout % 1000) * 1000;
    if(0 != setsockopt(m_socket, SOL_SOCKET, SO_RCVTIMEO, reinterpret_cast<char *>(&tv), sizeof(tv))) {
        return false;
    }

    int reuse = 1;
    if (0 != setsockopt(m_socket, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse))) {
        return false; 
    }

    SetNonBlocking(m_socket);

    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(struct sockaddr_in));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(m_listen_port);
    serv_addr.sin_addr.s_addr = inet_addr(m_listen_host);
    bind(m_socket, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    listen(m_socket, kMaxListenNum);

    return true;
}

void TcpServer::SetNonBlocking(int &sock)
{
    int opts = 0;
    opts = fcntl(sock, F_GETFL);
    if(opts < 0)
    {
        perror("fcntl(sock,GETFL)");
        assert(0);
    }
    opts = opts | O_NONBLOCK;
    if(fcntl(sock, F_SETFL, opts) < 0)
    {
        perror("fcntl(sock,SETFL,opts)");
        assert(0);
    }
    return;
}

bool TcpServer::IsRunning() const
{
    return m_continue;
}

int32_t TcpServer::GetWorkEpollFd(int socket)
{
    // [m_work_epoll_fd_idx % m_work_epoll_fd_size][(m_work_epoll_fd_idx / m_work_epoll_fd_size) % m_workers_per_epoll]
    //int idx = (m_work_epoll_fd_idx % m_work_epoll_fd_size) * m_workers_per_epoll + (m_work_epoll_fd_idx / m_work_epoll_fd_size) % m_workers_per_epoll; 
    //m_work_epoll_fd_idx = (m_work_epoll_fd_idx + 1) % (m_work_epoll_fd_size * m_workers_per_epoll);
    
    int32_t epoll_fd = m_work_epoll_fd[m_work_epoll_fd_idx];
    /*
    m_connection[socket].m_using = 1;
    m_connection[socket].m_work_epoll_idx = m_work_epoll_fd_idx;
    */
    Connection conn;
    conn.m_using = 1;
    conn.m_work_epoll_idx = m_work_epoll_fd_idx;
    conn.m_socket = socket;
    m_conn_map->Insert(socket, conn);
    m_work_epoll_fd_idx = (m_work_epoll_fd_idx + 1) % m_work_epoll_fd_size;
    //m_work_socket_list->Insert(socket, 0);
    m_work_socket_list.push_back(socket);
    return epoll_fd;
}

int32_t TcpServer::GetSocket() const
{
    return m_socket;
}

int32_t TcpServer::DoSend(int32_t sock, char *data, int32_t len)
{
    Lock lock(m_conn_mutex[sock]);
    //Connection *conn = &m_connection[sock];
    //Lock lock(m_send_mutex);
    Connection *conn = NULL;
    if (!m_conn_map->Find(sock, conn)) {
        CloseConnection(sock);
        return -1;
    }

    if (conn->m_write_off > 0) {
        if (conn->m_write_off + len > kMaxConnectionBufSize) {
            CloseConnection(sock);
            return -1;
        }
        memcpy(conn->m_write_buf + conn->m_write_off, data, len);
        conn->m_write_off += len;
        return 0;
    } else {
        int ret = write(conn->m_socket, data, len);
        if (ret > 0) {
            if (ret == len) {
                return 0;
            }
            int left = len - ret;
            if (left > kMaxConnectionBufSize) {
                CloseConnection(sock);
                return -1;
            }
            memcpy(conn->m_write_buf, data + ret, left);
            conn->m_write_off = left;
        } else {
            if (errno != EINTR && errno != EAGAIN) {
                CloseConnection(sock);
                return -1;
            }
            if (len > kMaxConnectionBufSize) {
                CloseConnection(sock);
                return -1;
            }
            memcpy(conn->m_write_buf, data, len);
            conn->m_write_off = len;
        }
    }

    return 0;
}

int32_t TcpServer::DoRecv(int32_t socket, const char *buf, int32_t len)
{
    socket = -1;
    buf = NULL;
    return len;
}

int32_t TcpServer::HandleReadEvent(int32_t sock)
{
    //Connection *conn = &m_connection[sock];
    Connection *conn = NULL;
    if (!m_conn_map->Find(sock, conn)) {
        CloseConnection(sock);
        return -1;
    }

    if (conn->m_read_off >= kMaxConnectionBufSize) {
        // too much to read 
        return -1;
    }
    int recv_size = 0;
    bool is_break = false;
    while (true) {
        recv_size = read(sock, conn->m_read_buf + conn->m_read_off, kMaxConnectionBufSize - conn->m_read_off);
        if (recv_size > 0) {
            is_break = false;
            if (recv_size < (kMaxConnectionBufSize - conn->m_read_off)) {
                is_break = true; 
            }
            conn->m_read_off += recv_size;
            int ret = DoRecv(sock, (char*)(conn->m_read_buf), conn->m_read_off); 
            if (ret < 0 || ret > conn->m_read_off) {
                return -1;
            }
            int left = conn->m_read_off - ret;
            if (left > 0 && 0 != ret) {
                memmove(conn->m_read_buf, conn->m_read_buf + ret, left);
            }
            conn->m_read_off = left;
            if (conn->m_read_off >= kMaxConnectionBufSize) {
                // too much to read 
                return -1;
            } 
            if (is_break) {
                break; 
            }
        } else if (0 == recv_size) {
            // client closed
            return -1; 
        } else {
            if (errno == EINTR) {
                continue;
            } else if (errno == EAGAIN) {
                break; 
            } else {
                return -1; 
            }
        }
    }
    return 0;
}

int32_t TcpServer::HandleWriteEvent(int32_t sock)
{
    //Connection *conn = &m_connection[sock];
    Connection *conn = NULL;
    if (!m_conn_map->Find(sock, conn)) {
        CloseConnection(sock);
        return -1;
    }

    if (conn->m_write_off == 0) {
        return 0;
    }

    int write_len = write(conn->m_socket, conn->m_write_buf, conn->m_write_off);

    if (write_len == -1) {
        if (errno != EINTR && errno != EAGAIN) {
            return -1;
        }
    } else {
        int left = conn->m_write_off - write_len;
        if (left > 0) {
            memmove(conn->m_write_buf, conn->m_write_buf + write_len, left);
        }
        conn->m_write_off = left;
    }

    return 0;
}

void TcpServer::CloseConnection(int32_t sock)
{
    //Connection *conn = &m_connection[sock];
    Connection *conn = NULL;
    if (!m_conn_map->Find(sock, conn)) {
        //CloseConnection(sock);
        return;
    }

    struct epoll_event ev_register;
    conn->m_using = 0;
    conn->m_read_off = 0;
    conn->m_write_off = 0;
    epoll_ctl(m_work_epoll_fd[conn->m_work_epoll_idx], EPOLL_CTL_DEL, conn->m_socket, &ev_register);
    close(conn->m_socket);
    //m_conn_map->Delete(sock);
    //m_work_socket_list->Delete(sock);
    return;
}

bool TcpServer::HasToWrite(int32_t sock)
{
    //Connection *conn = &m_connection[sock];
    Connection *conn = NULL;
    if (!m_conn_map->Find(sock, conn)) {
        CloseConnection(sock);
        return -1;
    }

    if (conn->m_write_off > 0) {
        return true; 
    }
    return false;
}

void TcpServer::Start()
{}

void TcpServer::Stop()
{
    m_continue = false;

    for (int i = 0; i < m_work_epoll_fd_size; i++) {
        for (int j = 0; j < m_workers_per_epoll; j++) {
            pthread_join(m_work_thread[i * m_workers_per_epoll + j], NULL);
        }
    }

    pthread_join(m_listen_thread, NULL);
  
    /* 
    struct epoll_event ev_unreg; 
    for (int i = 0; i < kMaxConnectionNum; i++) {
        if (m_connection[i].m_using) { 
            epoll_ctl(m_work_epoll_fd[m_connection[i].m_work_epoll_idx], EPOLL_CTL_DEL, m_connection[i].m_socket, &ev_unreg);
            close(m_connection[i].m_socket);
            m_connection[i].m_socket = -1;
        }
    }
    */
    struct epoll_event ev_unreg; 
    std::vector<int32_t>::iterator it;
    Connection *conn = NULL;
    for (it = m_work_socket_list.begin(); it != m_work_socket_list.end(); ++it) {
        if (!m_conn_map->Find(*it, conn)) {
            continue; 
        }
        if (conn->m_using) {
            epoll_ctl(m_work_epoll_fd[conn->m_work_epoll_idx], EPOLL_CTL_DEL, conn->m_socket, &ev_unreg);
            close(conn->m_socket);
            conn->m_socket = -1;
        }
    }

    for (int i =  0; i < m_work_epoll_fd_size; i++) {
        close(m_work_epoll_fd[i]);
        m_work_epoll_fd[i] = -1;
    }

    close(m_socket);
    m_socket = -1;
}

void TcpServer::Release()
{
    /*
    if (NULL != m_work_socket_list) {
        m_work_socket_list->Release();
        delete m_work_socket_list;
        m_work_socket_list = NULL; 
    }
    */

    if (NULL != m_work_thread) {
        delete[] m_work_thread;
        m_work_thread = NULL; 
    }

    if (NULL != m_work_epoll_fd) {
        delete[] m_work_epoll_fd;
       m_work_epoll_fd = NULL; 
    }

    if (m_socket > 0) {
        close(m_socket);
        m_socket = -1;
    }

    if (NULL != m_conn_map) {
        m_conn_map->Release();
        delete m_conn_map;
        m_conn_map = NULL; 
    }
    return;
}

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
