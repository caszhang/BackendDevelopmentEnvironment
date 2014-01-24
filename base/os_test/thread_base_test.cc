// Author: zhangguoqiang01 <80176975@qq.com>
#include "gtest/gtest.h"
#include "os/thread_base.h"

class ThreadBaseInstance:public ThreadBase
{
    public:
        ThreadBaseInstance() {
            m_continue = true; 
            m_run = 0;
            m_un_run = 0;
        }

        virtual ~ThreadBaseInstance() {}

        virtual void Run() {
            while (m_continue) {
                usleep(1000 * 10);
                m_run++;
            } 
            m_un_run++;
        }
        virtual void UnRun() {
            m_continue = false; 
        }

        bool GetContinue() {
            return m_continue;
        }

        int32_t GetRun() {
            return m_run; 
        }

        int32_t GetUnRun() {
            return m_un_run; 
        }
    private:
        bool m_continue;
        int32_t m_run;
        int32_t m_un_run;
};

class ThreadBaseInstanceTest:public testing::Test
{
    protected:
        virtual void SetUp() {
            m_instance = new ThreadBaseInstance;
        }
        virtual void TearDown() {
            delete m_instance;
            m_instance = NULL; 
        }
    public:
        ThreadBaseInstance *m_instance;    
};

TEST_F(ThreadBaseInstanceTest, TestCommon)
{
    EXPECT_TRUE(m_instance->GetContinue());
    EXPECT_EQ(0, m_instance->GetRun());
    EXPECT_EQ(0, m_instance->GetUnRun());

    m_instance->Start();
    sleep(2);

    EXPECT_TRUE(m_instance->GetContinue());
    EXPECT_GT(m_instance->GetRun(), 0);
    EXPECT_EQ(0, m_instance->GetUnRun());

    m_instance->Stop();

    EXPECT_FALSE(m_instance->GetContinue());
    EXPECT_GT(m_instance->GetRun(), 0);
    EXPECT_GT(m_instance->GetUnRun(), 0);
}


/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
