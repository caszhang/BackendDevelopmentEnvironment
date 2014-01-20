// Author: zhangguoqiang01 <80176975@qq.com>
#include "gtest/gtest.h"
#include "dsalgo/union_search_set.h"

class UnionSearchSetTest:public testing::Test
{
protected:
    virtual void SetUp() {
        m_union_search_set = new UnionSearchSet;
    }

    virtual void TearDown() {
        delete m_union_search_set;
        m_union_search_set = NULL;
    }
public:
    UnionSearchSet *m_union_search_set;
};

TEST_F(UnionSearchSetTest, JudgeRegionTest)
{
    /*
     *[20,25]
     *[0,10]
     *[15,20]
     *[40,45]
     *[30,40]
     *[50,60]
     * */
    // judge [20,22] whether in region

    if (!m_union_search_set->Init(61)) {
        printf("init failed\n");
        return;
    }

    for (int32_t i = 0; i <= 10; i++) {
        m_union_search_set->Union(0, i);
    }

    for (int32_t i = 15; i <= 20; i++) {
        m_union_search_set->Union(15, i);
    }

    for (int32_t i = 20; i <= 25; i++) {
        m_union_search_set->Union(20, i);
    }

    for (int32_t i = 30; i <= 40; i++) {
        m_union_search_set->Union(30, i);
    }

    for (int32_t i = 40; i <= 45; i++) {
        m_union_search_set->Union(40, i);
    }

    for (int32_t i = 50; i <= 60; i++) {
        m_union_search_set->Union(50, i);
    }

    printf("father[20]:%d\n", m_union_search_set->m_father[20]);
    printf("father[22]:%d\n", m_union_search_set->m_father[22]);

    for (int32_t i = 0; i < 61; i++) {
        printf("father[%d]:%d\n", i, m_union_search_set->m_father[i]);
    }
}
