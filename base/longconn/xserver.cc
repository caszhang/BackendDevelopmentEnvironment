/*******************************************************************************
 * Author :          guoqiang
 * Email :           guoqiang@foxea.com
 * Last modified :   2017-03-29 19:41
 * Filename :        xserver.cc
 * Description :     
 * *****************************************************************************/
#include "xserver.h"

void safe_close(int fd) 
{
    int ret = close(fd);
    while (ret != 0) {
        if (errno != EINTR || errno == EBADF) {
            break;
        }
        ret = close(fd);
    }
}

int accept_client(int fd, struct sockaddr_in *s_in) 
{
    int cfd = -1;
    do {
        socklen_t len = sizeof(struct sockaddr_in);
        cfd = accept(fd, (struct sockaddr *) s_in, &len);
        if (cfd < 0) {
            if (errno == EINTR) {
                continue;
            }
            fprintf(stderr, "cannot accpet, errno: %d %m\n", errno);
            break;
        }
        unsigned long non_blocking = 1;
        if (ioctl(cfd, FIONBIO, &non_blocking) != 0) {
            fprintf(stderr, "cannot set nonblocking, client_fd: %d errno: %d %m\n", cfd, errno);
            safe_close(cfd);
            cfd = -1;
        }
        return cfd;
    } while (-1);
    return -1;
}


int open_sever_socket(const char *ip, short port, int backlog) 
{
    int fd = -1;
    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
        fprintf(stderr, "cannot open server socket, errno: %d %m\n", errno);
        return -1;
    }

    unsigned long non_blocking = 1;
    if (ioctl(fd, FIONBIO, &non_blocking) != 0) {
        fprintf(stderr, "cannot set nonblocking, errno: %d %m\n", errno);
        safe_close(fd);
        return -1;
    }
    int flag_reuseaddr = 1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (char*) &flag_reuseaddr, sizeof(flag_reuseaddr));

    struct sockaddr_in addr;
    socklen_t addrlen = sizeof(addr);
    memset(&addr, 0, addrlen);
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = ip == NULL ? INADDR_ANY : inet_addr(ip);
    addr.sin_port = htons(port);

    if (bind(fd, (const struct sockaddr *) &addr, addrlen) != 0) {
        fprintf(stderr, "cannot bind, port: %d errno: %d %m\n", port, errno);
        safe_close(fd);
        return -1;
    }

    if (listen(fd, backlog) != 0) {
        fprintf(stderr, "cannot listen, port: %d errno: %d %m\n", port, errno);
        safe_close(fd);
        return -1;
    }

    return fd;
}

CONN_LIST* init_conn_list(int32_t size) 
{
    CONN_LIST *lst = new CONN_LIST;
    CONN *array = new CONN[size + 1];
    lst->m_array = array;

    if (NULL == lst || NULL == array) {
        return NULL; 
    }

    for (int i = 0; i < size; i++) {
        array[i].m_idx = i;
        array[i].m_next = &array[i + 1];
    }
    array[size].m_idx = size;
    array[size].m_next = NULL;

    lst->m_head = &array[0];
    lst->m_tail= &array[size];

    return lst;
}

CONN* get_free_conn(CONN_LIST *list)
{
    if (list->m_head != list->m_tail) {
        CONN *item = list->m_head;
        list->m_head = list->m_head->m_next;
        return item;
    } else {
        return NULL;
    }
}

void put_free_conn(CONN_LIST *list, CONN * &conn)
{
    list->m_tail->m_next = conn;
    list->m_tail = conn;
}


void close_conn(CONN *conn) 
{
    conn->m_in_buf_len = 0;
    bufferevent_disable(conn->m_bufev, EV_READ | EV_WRITE);
    safe_close(conn->m_fd);
    put_free_conn(conn->m_owner->m_conn_list, conn);
}

void *launch(void *arg)
{
    XServer *server = (XServer*)arg;
    server->StartServer();
    return NULL;
}

void *start_worker(void *arg) 
{
    WORKER *worker = (WORKER*) arg;
    SERVER *server = worker->m_server;
    
    pthread_mutex_lock(&server->m_start_lock);
    ++server->m_start_worker_num;
    pthread_cond_signal(&server->m_start_cond);
    pthread_mutex_unlock(&server->m_start_lock);
    
    // printf("start_worker:%d\n", s->m_start_worker_num);
    event_base_loop(worker->m_base, 0);
    fprintf(stdout, "start_worker error or loop exit, thread_id: %lu\n", worker->m_thread_id);
    return NULL;
}

void accept_action(int fd, short event, void *arg) 
{
    XServer *x_server = (XServer*) arg;
    struct sockaddr_in s_in;
    int cfd = accept_client(fd, &s_in);
    if (cfd == -1) {
        fprintf(stderr, "client accept error, errno: %d %m\n", errno);
        return;
    }
    int retry = 0;
    do {
        WORKER* worker = x_server->m_workers[x_server->m_work_idx++ % x_server->m_work_num];
        x_server->m_work_idx = x_server->m_work_idx % x_server->m_work_num;
        CONN *conn = get_free_conn(worker->m_conn_list);
        if (conn == NULL) {
            continue;
        }
        conn->m_fd = cfd;
        conn->m_cip = *(uint32_t *) &s_in.sin_addr;
        conn->m_cport = (uint16_t) s_in.sin_port;

        write(worker->m_notifed_wfd, (char *) &conn->m_idx, 4);

        // printf("client connect\n");
        return;
    } while (++retry < x_server->m_work_num / 2);//half retry times is enough
   
    // to busy, can not process
    safe_close(cfd);

    // to erase warning
    event = 0;
}

void receive_notify_action(int fd, short event, void *arg) 
{
    WORKER *worker = (WORKER*) arg;
    uint32_t idx;
    if (read(fd, &idx, 4) != 4) {
        fprintf(stderr, "notify_pipe read error, errno: %d %m\n", errno);
        return;
    }
    bufferevent_setfd(worker->m_conn_list->m_array[idx].m_bufev, worker->m_conn_list->m_array[idx].m_fd);
    bufferevent_enable(worker->m_conn_list->m_array[idx].m_bufev, EV_READ | EV_WRITE);
    
    // to erase warning
    event = 0;
}

void on_conn_read(struct bufferevent *be, void *arg) 
{
    // printf("called\n");
    CONN *conn = (CONN*) arg;
    // read util no data
    while (true) {
        int free_size = 0;
        while (true) {
            free_size = kConnBufLen - conn->m_in_buf_len;
            if (0 == free_size) {
                break;
            }
            int len = bufferevent_read(be, conn->m_in_buf + conn->m_in_buf_len, free_size);
            if (len <= 0) {
                break;
            }
            conn->m_in_buf_len += len;
        }
        conn->m_in_buf[conn->m_in_buf_len] = '\0';
        // has data to process
        if (conn->m_in_buf_len > 0) {
            if (conn->m_owner->m_server->m_xserver->CheckValid(conn)) {
                conn->m_owner->m_server->m_xserver->Process(conn);
                conn->m_in_buf_len = 0;
            } else {
                close_conn(conn);
            }
        }
        // full when read just now, need to read again
        if (0 != free_size) {
            break; 
        }
    }
}

void on_conn_err(struct bufferevent *be, short event, void *arg) 
{
    CONN *conn = (CONN*) arg;
    if (event & EVBUFFER_TIMEOUT) {
        // timeout
    } else if (event & EVBUFFER_EOF) {
        // close
    } else if (event & EVBUFFER_ERROR) {
        // error
    }
    // printf("on_conn_err\n");
    close_conn(conn);
    be = NULL;
}


XServer::XServer()
{
    m_server = NULL;
    m_workers = NULL;
    m_work_num = 0;
    m_work_idx = 0;
}

XServer::~XServer()
{
}

bool XServer::Init(const char *ip, const int32_t port, const uint16_t work_num, const uint32_t conn_num_per_worker, const int32_t read_timeout, const int32_t write_timeout)
{
    if (NULL == ip || port < 1024 || work_num < 1 || conn_num_per_worker < 1) {
        return false;
    }
    // init server
    m_server = new SERVER();
    m_server->m_xserver = this;
    snprintf(m_server->m_ip, 32, "%s", ip);
    m_server->m_port = port;
    m_server->m_base = event_init();
    if (m_server->m_base == NULL) {
        fprintf(stderr, "InitServer event base error, errno: %m\n");
        return false;
    }

    // init workers
    m_work_num = work_num;
    m_workers = new WORKER*[work_num];
    for (uint16_t i = 0; i < work_num; i++) {
        m_workers[i] = new WORKER; 
        int fds[2];
        if (pipe(fds)) {
            fprintf(stderr, "init_workers pipe error, errno: %d %m\n", errno);
            return false;
        }
        m_workers[i]->m_notified_rfd = fds[0];
        m_workers[i]->m_notifed_wfd = fds[1];
        m_workers[i]->m_base = event_init();
        if (m_workers[i]->m_base == NULL) {
            fprintf(stderr, "init_workers event base error, errno: %d %m\n", errno);
            return false;
        }

        event_set(&m_workers[i]->m_notify_event, m_workers[i]->m_notified_rfd, EV_READ | EV_PERSIST, receive_notify_action, m_workers[i]);
        event_base_set(m_workers[i]->m_base, &(m_workers[i]->m_notify_event));
        if (event_add(&m_workers[i]->m_notify_event, 0) == -1) {
            fprintf(stderr, "init_workers add event error, errno: %d %m\n", errno);
            return false;
        }

        m_workers[i]->m_conn_list = init_conn_list(conn_num_per_worker);
        if (NULL == m_workers[i]->m_conn_list) {
            fprintf(stderr, "init_workers conn_list error, errno: %d %m\n", errno);
            event_base_free(m_workers[i]->m_base);
            delete m_workers[i];
            m_workers[i] = NULL;
            return false;
        }

        CONN *p = m_workers[i]->m_conn_list->m_head;
        while (p != NULL) {
            p->m_bufev = bufferevent_new(-1, on_conn_read, NULL, on_conn_err, p);
            bufferevent_base_set(m_workers[i]->m_base, p->m_bufev);
            bufferevent_settimeout(p->m_bufev, read_timeout, write_timeout);
            p->m_owner = m_workers[i];
            p = p->m_next;
        }
        m_workers[i]->m_server = m_server;
    }
    // init workers end

    pthread_mutex_init(&m_server->m_start_lock, NULL);
    pthread_cond_init(&m_server->m_start_cond, NULL);

    return true;
}

bool XServer::Start()
{

    if (pthread_create(&m_thread_id, NULL, launch, this) != 0) {
        fprintf(stderr, "launch create thread error, errno: %d %m\n", errno);
        return false;
    }
    return true;
}

bool XServer::StartServer()
{
    StartWorkers();

    m_server->m_fd = open_sever_socket(m_server->m_ip, m_server->m_port, 1024);
    if (m_server->m_fd < 0) {
        fprintf(stderr, "open server socket error, errno: %d %m\n", errno);
        return false;
    }
    event_set(&m_server->m_listen_event, m_server->m_fd, EV_READ | EV_PERSIST, accept_action, this);
    event_base_set(m_server->m_base, &m_server->m_listen_event);
    if (event_add(&m_server->m_listen_event, 0) == -1) {
        fprintf(stderr, "start server add listen event error, errno: %d %m\n", errno);
        return false;
    }
    event_base_loop(m_server->m_base, 0);
    fprintf(stdout, "StartServer error or loop exit, thread_id: %lu\n", m_thread_id);
    return true;
}

bool XServer::StartWorkers()
{
    for (uint16_t i = 0; i < m_work_num; i++) {
        if (pthread_create(&m_workers[i]->m_thread_id, NULL, start_worker, m_workers[i]) != 0) {
            fprintf(stderr, "start_workers create thread error, errno: %d %m\n", errno);
            return false;
        }
    } 

    // to make sure all workers start to work ,then finish StartWorkers func called
    while (m_server->m_start_worker_num < m_work_num) {
        // printf("StartWorkers:%d\n", m_server->m_start_worker_num);
        pthread_mutex_lock(&m_server->m_start_lock);
        pthread_cond_wait(&m_server->m_start_cond, &m_server->m_start_lock);
        pthread_mutex_unlock(&m_server->m_start_lock);
    }
   
    return true;
}

bool XServer::Stop()
{

    if (NULL != m_workers) {
        for (uint16_t i = 0; i < m_work_num; i++) {
            event_base_loopbreak(m_workers[i]->m_base);
        }
    }
    
    if (NULL != m_server) {
        event_base_loopbreak(m_server->m_base);
    }

    pthread_join(m_thread_id, NULL);
 
    return true;
}

bool XServer::Release()
{
    if (NULL != m_server) {
        event_base_free(m_server->m_base);
        delete m_server;
        m_server = NULL;
    }

    if (NULL != m_workers) {
        for (uint16_t i = 0; i < m_work_num; i++) {
            event_base_free(m_workers[i]->m_base);
            delete m_workers[i]; 
        }
        delete[] m_workers;
        m_workers = NULL;
    }
    return true;
}

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
