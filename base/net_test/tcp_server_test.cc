// Deprecated
// Author: zhangguoqiang01 <80176975@qq.com>
#include "gtest/gtest.h"
#include "net/tcp_client.h"
#include "net/tcp_server.h"
#include "os/lock.h"
#include "os/thread.h"

class TcpServerInstance:public TcpServer
{
public:    
    TcpServerInstance() {
        m_recv_count = 0;
        m_recv_len = 0; 
    }
    virtual int32_t DoRecv(int32_t socket, const char * buf, int32_t len) {
        Lock lock(m_mutex);
        m_recv_count++;
        m_recv_len += len;
        DoSend(socket, const_cast<char*>(buf), len);
        return len; 
    }
//private:
public:
    Mutex m_mutex;
    int32_t m_recv_count;
    int32_t m_recv_len;
};

const char *ip = "10.48.57.64";
const uint16_t port = 18606;

class TcpServerTest:public testing::Test
{
public:
    virtual void SetUp() {
        m_tcp_server_instance = new TcpServerInstance;
        m_tcp_server_instance->Init(ip, port); 
        m_tcp_server_instance->Start();
    }
    virtual void TearDown() {
        m_tcp_server_instance->Stop();
        m_tcp_server_instance->Release();
        delete m_tcp_server_instance;
        m_tcp_server_instance = NULL; 
    }
public:
    TcpServerInstance *m_tcp_server_instance;
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
        assert(tc->Recv(recv_buf, strlen(send_buf)));
        EXPECT_STREQ(send_buf, recv_buf);
        c->m_count++;
        c->m_sum += strlen(send_buf);
    }
    printf("%d\n", i);       
    return;
}

TEST_F(TcpServerTest, TestCommon)
{
    TcpClient *tc_one = new TcpClient;
    tc_one->Init(ip, port);
    TcpClient *tc_two = new TcpClient;
    tc_two->Init(ip, port);
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

    int32_t all_count = 0;
    int32_t all_sum = 0;
    all_count += c_one.m_count;
    all_count += c_two.m_count;
    all_count += c_three.m_count;
    all_sum += c_one.m_sum;
    all_sum += c_two.m_sum;
    all_sum += c_three.m_sum;
    //EXPECT_EQ(m_tcp_server_instance->m_recv_count, all_count);
    EXPECT_EQ(m_tcp_server_instance->m_recv_len, all_sum);
    tc_one->Release();
    delete tc_one;
    tc_two->Release();
    delete tc_two;
    tc_three->Release();
    delete tc_three;
    printf("done\n");

}

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
