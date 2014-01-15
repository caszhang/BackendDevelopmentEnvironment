// All rights reserved.
// Author: zhangguoqiang01 <80176975@qq.com>
#include "gtest/gtest.h"
#include "dsalgo/hashmap.cc"
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

TEST(HashMapTest, TestNormal)
{
    HashMap<int32_t, int32_t> *hashmap = new HashMap<int32_t, int32_t>;
    int32_t bucket_num = 100;
    bool ret = hashmap->Init(bucket_num);
    EXPECT_TRUE(ret);
    int32_t key = 0;
    int32_t value = -1;
    ret = hashmap->Find(key, value);
    EXPECT_FALSE(ret);
    EXPECT_EQ(-1, value);
    int32_t size = hashmap->GetSize();
    EXPECT_EQ(0, size);
    int32_t rv = hashmap->Insert(key, value);
    EXPECT_TRUE(1 == rv);
    key = 1;
    value = 1;
    rv = hashmap->Insert(key, value);
    EXPECT_TRUE(1 == rv);
    ret = hashmap->Find(0, value);
    EXPECT_TRUE(ret);
    EXPECT_EQ(-1, value);
    ret = hashmap->Find(1, value);
    EXPECT_TRUE(ret);
    EXPECT_EQ(1, value);
    size = hashmap->GetSize();
    EXPECT_EQ(2, size);
    rv = hashmap->Insert(1, 5);
    EXPECT_TRUE(2 == rv);
    ret = hashmap->Find(1, value);
    EXPECT_TRUE(ret);
    EXPECT_EQ(5, value);
    value = -1;
    size = hashmap->GetSize();
    EXPECT_EQ(2, size);

    ret = hashmap->Delete(0);
    EXPECT_TRUE(ret);
    ret = hashmap->Find(0, value);
    EXPECT_FALSE(ret);
    EXPECT_EQ(-1, value); 
    size = hashmap->GetSize();
    EXPECT_EQ(1, size);

    ret = hashmap->Release();
    EXPECT_TRUE(ret);
}

TEST(HashMapTest, TestCompositeDataType)
{
    HashMap<int32_t, Date> *hashmap = new HashMap<int32_t, Date>;
    int32_t bucket_num = 10000;
    bool ret = hashmap->Init(bucket_num);
    EXPECT_TRUE(ret);
    int32_t size = hashmap->GetSize();
    EXPECT_EQ(0, size);
   
    for (int32_t i = 0; i < 10000; i++) {
        Date date;
        date.year = i + 1;
        date.month = i + 1;
        date.date = i + 1;
        int32_t rv = hashmap->Insert(i, date);
        EXPECT_EQ(1, rv);
    }
    size = hashmap->GetSize();
    EXPECT_EQ(10000, size);
    
    for (int32_t i = 0; i < 10000; i++) {
        Date date;
        ret = hashmap->Find(i, date);
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
        int32_t rv = hashmap->Insert(i, date);
        EXPECT_EQ(2, rv);
    }
    size = hashmap->GetSize();
    EXPECT_EQ(10000, size);

    for (int32_t i = 0; i < 10000; i++) {
        ret = hashmap->Delete(i);
        EXPECT_TRUE(ret);
    }
    size = hashmap->GetSize();
    EXPECT_EQ(0, size);

    for (int32_t i = 0; i < 10000; i++) {
        Date date;
        ret = hashmap->Find(i, date);
        EXPECT_FALSE(ret);
        EXPECT_EQ(0, date.year);
        EXPECT_EQ(0, date.month);
        EXPECT_EQ(0, date.date);
    }

    hashmap->Release();
}


