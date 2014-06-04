// Author: zhangguoqiang01 <80176975@qq.com>
#include "gtest/gtest.h"
#include "net/tcp_client.h"
#include "net/simple_server.h"
#include "os/lock.h"
#include "os/thread.h"

class SimpleServerInstance:public SimpleServer
{
public:
    virtual void ProcessSocket(int socket) {
        char recv_buf[8];
        for (int i = 0; i < 100; i++) {
            memset(recv_buf, 0, 8 * sizeof(recv_buf[0]));
            recv(socket, recv_buf, 8, 0);
            printf("recv:[%s]\n", recv_buf);
        } 
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
    char recv_buf[8];
    int i;
    for (i = 0; i < 100; i++) {
        snprintf(send_buf, 8, "%d", i);
        assert(tc->Send(send_buf, strlen(send_buf))); 
        printf("send:[%s]\n", send_buf);       
        //assert(tc->Recv(recv_buf, strlen(send_buf)));
        //EXPECT_STREQ(send_buf, recv_buf);
        c->m_count++;
        c->m_sum += strlen(send_buf);
    }
    return;
}


TEST_F(SimpleServerTest, TestCommon)
{
    TcpClient *tc_one = new TcpClient;
    tc_one->Init(ip, port);

    Composition c_one;
    c_one.m_tc = tc_one;

    Thread t_one(send_func, reinterpret_cast<void*>(&c_one));

    t_one.Start();

    t_one.Stop();

    tc_one->Release();
    delete tc_one;
    printf("done\n");

}
