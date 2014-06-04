#include "simple_server.h"
const static int32_t kMaxListenNum = 1024;
const static int32_t kMaxEpollListenNum = 512;
const static int32_t kTimeWaitOut = 500;
const static int32_t kSendTimeout = 2000;
const static int32_t kRecvTimeout = 200;
void simple_server_process(void *param) 
{
    // printf("simple_server_process called\n");
    ThreadArgument *ta = (ThreadArgument*)param;
    SimpleServer *ss = (SimpleServer*)(ta->m_instance);
    ss->ProcessSocket(ta->m_socket);
    delete ta;
}

SimpleServer::SimpleServer()
{
    m_ip[0] = '\0';
    m_port = 0;
    m_socket = -1;
    m_epoll_fd = -1;
    m_continue = false;
    m_thread_pool = NULL;
}

SimpleServer::~SimpleServer()
{}

bool SimpleServer::Init(const char *ip, const uint16_t port, int threads_num)
{
    if (NULL == ip || strlen(ip) < 1 || port < 1024) {
        return false;
    }

    m_threads_num = threads_num;

    snprintf(m_ip, 32, "%s", ip);
    m_port = port;

    if (!InitNet()) {
        return false; 
    }

    m_thread_pool = new ThreadPool;
    if (!m_thread_pool->Init(m_threads_num)) {
        return false; 
    }
    m_thread_pool->Start();

    return true;
}

bool SimpleServer::InitNet()
{
    m_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(m_socket == -1) {
        return false;
    }   

    /*
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
    */

    SetNonBlocking(m_socket);
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(struct sockaddr_in));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(m_port);
    serv_addr.sin_addr.s_addr = inet_addr(m_ip);
    bind(m_socket, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    listen(m_socket, kMaxListenNum);

    if (!InitEpoll()) {
        return false;
    }

    m_continue = true;
    return true;
}

void SimpleServer::SetNonBlocking(int sock)
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

bool SimpleServer::InitEpoll()
{
    m_epoll_fd = epoll_create(kMaxEpollListenNum);
    struct epoll_event eevent;
    eevent.data.fd = m_socket;
    eevent.events = EPOLLIN | EPOLLET;
    epoll_ctl(m_epoll_fd, EPOLL_CTL_ADD, m_socket, &eevent);
    return true;
}

void SimpleServer::Run()
{
    int event_num = 0;
    int connfd = 0;
    socklen_t clilen;
    struct sockaddr_in clientaddr;
    struct epoll_event events[kMaxEpollListenNum];
    while (m_continue) {
        event_num = epoll_wait(m_epoll_fd, events, kMaxEpollListenNum, kTimeWaitOut);
        for(int i = 0; i < event_num; ++i) {
            // printf("SimpleServer::Run envent\n");
            if ((events[i].events & EPOLLERR) ||
                    (events[i].events & EPOLLHUP) ||
                    (!(events[i].events & EPOLLIN))) {
                epoll_ctl(m_epoll_fd, EPOLL_CTL_DEL, events[i].data.fd, NULL);
                close(events[i].data.fd);
                events[i].data.fd = -1;
                continue;
            } else if (events[i].data.fd == m_socket) {
                while (true) {
                    char hbuf[NI_MAXHOST];
                    char sbuf[NI_MAXSERV];
                    connfd = accept(m_socket,(sockaddr*)&clientaddr, &clilen);
                    if (connfd == -1)  
                    {  
                        if ((errno == EAGAIN) || (errno == EWOULDBLOCK))  
                        {  
                            /* We have processed all incoming 
                             * connections. */  
                            break;  
                        } else {
                            perror("connfd<0");
                            break;  
                        }  
                    } 
                    int flag = getnameinfo((sockaddr*)&clientaddr, clilen, hbuf, sizeof(hbuf), sbuf, sizeof(sbuf), NI_NUMERICHOST | NI_NUMERICSERV);
                    if (flag == 0) {
                        printf("Accepted connection on descriptor %d "  
                                "(host=%s, port=%s)\n", connfd, hbuf, sbuf);  
                    } 
                    // printf("called.....%s\n", inet_ntoa(clientaddr.sin_addr));
                    ThreadArgument *thread_argument = new ThreadArgument;
                    thread_argument->m_instance = (void*)this;
                    thread_argument->m_socket = connfd;
                    ThreadPoolTask thread_pool_task;
                    thread_pool_task.m_function = simple_server_process;
                    thread_pool_task.m_argument = (void*)thread_argument;
                    // printf("task add\n");
                    m_thread_pool->AddTask(thread_pool_task);
                }
            }
        }
    }
    return;
}

void SimpleServer::UnRun()
{
    m_continue = false;
}

void SimpleServer::Release()
{
    if (NULL != m_thread_pool) {
        m_thread_pool->Stop();
        m_thread_pool->Release();
        delete m_thread_pool;
        m_thread_pool = NULL;
    }

    if (m_socket > 0) {
        close(m_socket);
        m_socket = -1;
    }

    if (m_epoll_fd > 0) {
        close(m_epoll_fd);
        m_epoll_fd = -1;
    }
}


