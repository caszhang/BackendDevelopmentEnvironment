// All rights reserved.
// Author: zhangguoqiang01 <80176975@qq.com>
#include "gtest/gtest.h"
#include "dsalgo/skiplist.cc"
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

TEST(SkipListTest, TestNormal)
{
    SkipList<int32_t, int32_t> *skiplist = new SkipList<int32_t, int32_t>;
    bool ret = skiplist->Init();
    
    EXPECT_TRUE(ret);
    int32_t key = 0;
    int32_t value = -1;
    ret = skiplist->Find(key, value);
    EXPECT_FALSE(ret);
    EXPECT_EQ(-1, value);
    int32_t size = skiplist->GetSize();
    EXPECT_EQ(0, size);

    for (int32_t i = 59; i >= 0; i--) {
        ret = skiplist->Insert(i, i);
        EXPECT_TRUE(ret);
    }
    skiplist->PrintInfo();
    size = skiplist->GetSize();
    EXPECT_EQ(60, size); 
    
    for (int32_t i = 0; i < 60; i++) {
       ret = skiplist->Find(i, value);
       EXPECT_TRUE(ret); 
       EXPECT_EQ(i, value); 
    }

    for (int32_t i = 0; i < 60; i++) {
       ret = skiplist->Delete(i);
       EXPECT_TRUE(ret);
       ret = skiplist->Find(i, value);
       EXPECT_FALSE(ret);
    }
    size = skiplist->GetSize();
    EXPECT_EQ(0, size);
    
    ret = skiplist->Release();
    EXPECT_TRUE(ret);
}

TEST(SkipListTest, TestCompositeDataType)
{
    SkipList<int32_t, Date> *skiplist = new SkipList<int32_t, Date>;
    bool ret = skiplist->Init();
    EXPECT_TRUE(ret);
    int32_t size = skiplist->GetSize();
    EXPECT_EQ(0, size);
   
    for (int32_t i = 0; i < 10000; i++) {
        Date date;
        date.year = i + 1;
        date.month = i + 1;
        date.date = i + 1;
        ret = skiplist->Insert(i, date);
        EXPECT_TRUE(ret);
    }
    size = skiplist->GetSize();
    EXPECT_EQ(10000, size);
    
    for (int32_t i = 9999; i >= 0; i--) {
        Date date;
        ret = skiplist->Find(i, date);
        EXPECT_TRUE(ret);
        EXPECT_EQ(i + 1, date.year);
        EXPECT_EQ(i + 1, date.month);
        EXPECT_EQ(i + 1, date.date);
    }

    for (int32_t i = 0; i < 10000; i++) {
        Date date;
        date.year = i;
        date.month = i;
        date.date = i;
        ret = skiplist->Insert(i, date);
        EXPECT_TRUE(ret);
    }
    size = skiplist->GetSize();
    EXPECT_EQ(10000, size);
    for (int32_t i = 9999; i >= 0; i--) {
        Date date;
        ret = skiplist->Find(i, date);
        EXPECT_TRUE(ret);
        EXPECT_EQ(i, date.year);
        EXPECT_EQ(i, date.month);
        EXPECT_EQ(i, date.date);
    }

    for (int32_t i = 0; i < 10000; i++) {
        ret = skiplist->Delete(i);
        EXPECT_TRUE(ret);
    }
    size = skiplist->GetSize();
    EXPECT_EQ(0, size);

    for (int32_t i = 0; i < 10000; i++) {
        Date date;
        ret = skiplist->Find(i, date);
        EXPECT_FALSE(ret);
        EXPECT_EQ(0, date.year);
        EXPECT_EQ(0, date.month);
        EXPECT_EQ(0, date.date);
    }

    ret = skiplist->Release();
    EXPECT_TRUE(ret);
}
