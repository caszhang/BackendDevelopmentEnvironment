// Author: zhangguoqiang01 <80176975@qq.com>
#include "gtest/gtest.h"
#include "dsalgo/linkedlist.cc"
typedef struct Date {
    int32_t year;
    int32_t month;
    int32_t date;
    Date() {
        year = 0;
        month = 0;
        date = 0;
    }
} Date;

TEST(LinkedListTest, TestNormal)
{
    LinkedList<int32_t, int32_t> *ll = new LinkedList<int32_t, int32_t>;
    int32_t key = 0;
    int32_t value = 0;
    bool ret = ll->Find(key, value);
    EXPECT_FALSE(ret);
    EXPECT_EQ(0, value);
    
    int32_t rv = ll->Insert(key, value);
    EXPECT_TRUE(rv == 1);
    key = 1;
    value = 1;
    rv = ll->Insert(key, value);
    EXPECT_TRUE(1 == rv);
    ret = ll->Find(0, value);
    EXPECT_TRUE(ret);
    EXPECT_EQ(0, value);
    ret = ll->Find(1, value);
    EXPECT_TRUE(ret);
    EXPECT_EQ(1, value);
    rv = ll->Insert(1, 5);
    EXPECT_TRUE(2 == rv);
    ret = ll->Find(1, value);
    EXPECT_TRUE(ret);
    EXPECT_EQ(5, value);
    value = -1;
    ret = ll->Delete(0);
    EXPECT_TRUE(ret);
    ret = ll->Find(0, value);
    EXPECT_FALSE(ret);
    EXPECT_EQ(-1, value); 

    ret = ll->Release();
    EXPECT_TRUE(ret);
}

TEST(LinkedListTest, TestCompositeDataType)
{
    LinkedList<int32_t, Date> *ll = new LinkedList<int32_t, Date>;
    int32_t key = -1;
    Date date;
    bool ret = ll->Find(key, date);
    EXPECT_FALSE(ret);
    
    date.year = 2012;
    date.month = 4;
    date.date = 5;
    int32_t rv = ll->Insert(2012, date);
    EXPECT_TRUE(1 == rv);
    date.year = 2013;
    date.month = 11;
    date.date = 15;
    rv = ll->Insert(2013, date);
    EXPECT_TRUE(1 == rv);

    ret = ll->Find(2012, date);
    EXPECT_TRUE(ret);
    EXPECT_EQ(2012, date.year);
    EXPECT_EQ(4, date.month);
    EXPECT_EQ(5, date.date);

    ret = ll->Find(2013, date);
    EXPECT_TRUE(ret);
    EXPECT_EQ(2013, date.year);
    EXPECT_EQ(11, date.month);
    EXPECT_EQ(15, date.date);
    
    date.year = 2012;
    date.month = 6;
    date.date = 6;
    rv = ll->Insert(2012, date);
    EXPECT_TRUE(2 == rv);

    ret = ll->Find(2012, date);
    EXPECT_TRUE(ret);
    EXPECT_EQ(2012, date.year);
    EXPECT_EQ(6, date.month);
    EXPECT_EQ(6, date.date);
    
    ret = ll->Delete(2012);
    EXPECT_TRUE(ret);
    ret = ll->Find(2012, date);
    EXPECT_FALSE(ret);

    ll->Release();
}

