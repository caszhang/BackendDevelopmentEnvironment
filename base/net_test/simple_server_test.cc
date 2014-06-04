// Author: zhangguoqiang01 <80176975@qq.com>
#include "gtest/gtest.h"
#include "net/tcp_client.h"
#include "net/simple_server.h"
#include "os/lock.h"
#include "os/thread.h"
const static int32_t kRecvTimeout = 200;

class SimpleServerInstance:public SimpleServer
{
public:
    virtual void ProcessSocket(int &socket) {
        struct timeval tv;
        tv.tv_sec  = kRecvTimeout / 1000;
        tv.tv_usec = (kRecvTimeout % 1000) * 1000;
        if(0 != setsockopt(socket, SOL_SOCKET, SO_RCVTIMEO, reinterpret_cast<char *>(&tv), sizeof(tv))) {
            return;
        }

        char recv_buf[8];
        int recv_len = 0;
        int buf_len = 0;
        while (true) {
            memset(recv_buf, 0, 8 * sizeof(recv_buf[0]));
            recv_len = recv(socket, recv_buf, 2, 0); 
            if (recv_len > 0) {
                recv_buf[recv_len] = '\0';
                printf("recv:[%s]\n", recv_buf);
            } else {
                if ((recv_len < 0) && (errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR)) {
                    continue; 
                }
                break;
            }
        }

        close(socket);
        socket = -1;
    }
};

const char *ip = "112.124.57.220";
const uint16_t port = 18606;

class SimpleServerTest:public testing::Test
{
public:
    virtual void SetUp() {
        m_simple_server_instance = new SimpleServerInstance;
        m_simple_server_instance->Init(ip, port); 
        m_simple_server_instance->Start();
    }
    virtual void TearDown() {
        m_simple_server_instance->Stop();
        m_simple_server_instance->Release();
        delete m_simple_server_instance;
        m_simple_server_instance = NULL; 
    }
public:
    SimpleServerInstance *m_simple_server_instance;
};

struct Composition {
    TcpClient *m_tc;
    int32_t m_count;
    int32_t m_sum;

    Composition() {
        m_tc = NULL;
        m_count = 0;
        m_sum = 0; 
    }
}; 


void send_func(void *param)
{
    Composition *c = reinterpret_cast<Composition*>(param);
    TcpClient *tc = c->m_tc;
    char send_buf[8];
    int i;
    for (i = 0; i < 100; i++) {
        memset(send_buf, 0, 8);
        snprintf(send_buf, 8, "%d", i);
        assert(tc->Send(send_buf, strlen(send_buf))); 
        printf("send:[%s]\n", send_buf);       
        c->m_count++;
    }
    return;
}


TEST_F(SimpleServerTest, TestCommon)
{
    TcpClient *tc_one = new TcpClient;
    tc_one->Init(ip, port);
    // sleep(1);
    TcpClient *tc_two = new TcpClient;
    tc_two->Init(ip, port);
    // sleep(1);
    TcpClient *tc_three = new TcpClient;
    tc_three->Init(ip, port);

    Composition c_one;
    c_one.m_tc = tc_one;
    Composition c_two;
    c_two.m_tc = tc_two;
    Composition c_three;
    c_three.m_tc = tc_three;


    Thread t_one(send_func, reinterpret_cast<void*>(&c_one));
    Thread t_two(send_func, reinterpret_cast<void*>(&c_two));
    Thread t_three(send_func, reinterpret_cast<void*>(&c_three));

    t_one.Start();
    t_two.Start();
    t_three.Start();


    t_one.Stop();
    t_two.Stop();
    t_three.Stop();
   
    sleep(2);
    tc_one->Release();
    delete tc_one;
    tc_two->Release();
    delete tc_two;
    tc_three->Release();
    delete tc_three;
    printf("done\n");

}
