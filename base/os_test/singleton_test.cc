// Author: zhangguoqiang01 <80176975@qq.com>
#include "gtest/gtest.h"
#include "os/singleton.cc"

class SingleInstance
{
public:
    SingleInstance() {
        m_value = 0;
        Init(); 
    }
    ~SingleInstance() {
        Release(); 
    }
    bool Init() {
        m_value++; 
    }
    void Release() {}
    int32_t GetValue() {
        return m_value; 
    }

    void IncreaseValue() {
        m_value++; 
    }
private:
    int32_t m_value;
};

TEST(SingletonTest, TestCommon) 
{
    SingleInstance *instance_one = Singleton<SingleInstance>::GetInstance();   
    SingleInstance *instance_two = Singleton<SingleInstance>::GetInstance();   
    SingleInstance *instance_three = new SingleInstance;

    EXPECT_EQ(instance_one, instance_two);
    EXPECT_NE(instance_one, instance_three);
    int32_t instance_one_value = instance_one->GetValue();
    instance_one_value++;
    instance_two->IncreaseValue();
    EXPECT_EQ(instance_one_value, instance_two->GetValue());

    Singleton<SingleInstance>::ReleaseInstance();
    delete instance_three;
    instance_three = NULL;
}
/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
