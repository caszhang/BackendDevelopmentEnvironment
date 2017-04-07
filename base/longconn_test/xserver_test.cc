/*******************************************************************************
 * Author :          guoqiang
 * Email :           guoqiang@foxea.com
 * Last modified :   2017-04-06 14:30
 * Filename :        xserver_test.cc
 * Description :     
 * *****************************************************************************/

#include "gtest/gtest.h"
#include "longconn/xserver.h"
#include "os/lock.h"

class XServerInstance:public XServer
{
public:
    virtual bool CheckValid(CONN *conn) {
        return true;
    }
    virtual void Process(CONN *conn) {
        Lock lock(m_mutex);
        char *buf = conn->m_in_buf;
        int alreay_read = 0;
        while (alreay_read + 4 <= conn->m_in_buf_len) {
            int temp = *(int*)(conn->m_in_buf + alreay_read);
            m_sum += temp;
            alreay_read += 4;
        }
        printf("%ld\n", m_sum);
    }


public:
    Mutex m_mutex;
    uint64_t m_sum;
};

const char *ip = "xxx.xxx.xx.xx";
const uint16_t port = 8606;

class XServerTest:public testing::Test
{
public:
    virtual void SetUp() {
        m_instance = new XServerInstance;
        m_instance->Init(ip, port, 2, 5, 8, 8);
        m_instance->Start();
    }
    virtual void TearDown() {
        if (NULL != m_instance) {
            m_instance->Stop();
            m_instance->Release();
            delete m_instance;
            m_instance = NULL;
        }
    }
public:
    XServerInstance *m_instance;
};

void send_func(void *param)
{
   return;
}

TEST_F(XServerTest, TestCommon)
{
    sleep(5);
    if (NULL != m_instance) {
        m_instance->Stop();
        m_instance->Release();
        delete m_instance;
        m_instance = NULL;
    }

    //printf("%ld\n", m_instance->m_sum);
}
