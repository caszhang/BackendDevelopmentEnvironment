// Author: zhangguoqiang01 <80176975@qq.com>
#include "gtest/gtest.h"
#include "os/thread.h"
void increase(void *param)
{
    int *p = reinterpret_cast<int*>(param);
    for (int i = 0; i < 10; i++) {
        (*p)++;
    }
}

class SomeInstance
{
    public:
        SomeInstance() {
            m_num = 0; 
        }

        virtual ~SomeInstance() {}

        void Increase() {
            m_num++; 
        }   

        int32_t GetNum() {
            return m_num; 
        } 
    private:
        int32_t m_num;
};

void increase_instance(void *param)
{
    SomeInstance *some_instance = reinterpret_cast<SomeInstance*>(param);
    for (int i = 0; i < 10; i++) {
        some_instance->Increase(); 
    }
}


TEST(ThreadTest, TestFunc)
{
    int num = 0;
    int* p_num = &num;
    Thread t(increase, reinterpret_cast<void*>(p_num));
    t.Start();
    t.Stop();
    EXPECT_EQ(10, *p_num);
}

TEST(ThreadTest, TestInstance)
{
    SomeInstance *some_instance = new SomeInstance;
    Thread t(increase_instance, reinterpret_cast<void*>(some_instance));
    t.Start();
    t.Stop();
    EXPECT_EQ(10, some_instance->GetNum());
    delete some_instance;
    some_instance = NULL;
}
/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
