// Author: zhangguoqiang01 <80176975@qq.com>
#include "gtest/gtest.h"
#include "dsalgo/lru_hashmap.cc"
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

TEST(LruHashMapTest, TestNotLru)
{
    LruHashMap<int32_t, int32_t> *lru_hashmap = new LruHashMap<int32_t, int32_t>;
    
    int32_t size = 5;
    bool is_lru = false;
    bool ret = lru_hashmap->Init(size);
    EXPECT_TRUE(ret);
    EXPECT_EQ(0, lru_hashmap->GetSize());
    EXPECT_EQ(size, lru_hashmap->GetMaxNodeNum());
    
    int32_t key = 0;
    int32_t value = -1;
    ret = lru_hashmap->Get(key, value, is_lru);
    EXPECT_FALSE(ret);
    EXPECT_EQ(-1, value);
    
    EXPECT_EQ(0, lru_hashmap->GetSize());
    ret = lru_hashmap->Put(key, value, is_lru);
    EXPECT_TRUE(ret);
    EXPECT_EQ(1, lru_hashmap->GetSize());
    EXPECT_EQ(size, lru_hashmap->GetMaxNodeNum());
    
    ret = lru_hashmap->Remove(0);
    EXPECT_TRUE(ret);
    EXPECT_EQ(0, lru_hashmap->GetSize());
    EXPECT_EQ(size, lru_hashmap->GetMaxNodeNum());

    ret = lru_hashmap->Put(0, 0, is_lru);
    EXPECT_TRUE(ret);
    EXPECT_EQ(1, lru_hashmap->GetSize());
    ret = lru_hashmap->Put(1, 1, is_lru);
    EXPECT_TRUE(ret);
    EXPECT_EQ(2, lru_hashmap->GetSize());
    ret = lru_hashmap->Put(2, 2, is_lru);
    EXPECT_TRUE(ret);
    EXPECT_EQ(3, lru_hashmap->GetSize());
    ret = lru_hashmap->Put(3, 3, is_lru);
    EXPECT_TRUE(ret);
    EXPECT_EQ(4, lru_hashmap->GetSize());
    ret = lru_hashmap->Put(4, 4, is_lru);
    EXPECT_TRUE(ret);
    EXPECT_EQ(5, lru_hashmap->GetSize());
    ret = lru_hashmap->Put(5, 5, is_lru);
    EXPECT_TRUE(ret);
    EXPECT_EQ(5, lru_hashmap->GetSize());

    value = -1;
    ret = lru_hashmap->Get(0, value, is_lru);
    EXPECT_FALSE(ret);
    EXPECT_EQ(-1, value);

    ret = lru_hashmap->Get(1, value, is_lru);
    EXPECT_TRUE(ret);
    EXPECT_EQ(1, value);
    ret = lru_hashmap->Get(2, value, is_lru);
    EXPECT_TRUE(ret);
    EXPECT_EQ(2, value);
    ret = lru_hashmap->Get(3, value, is_lru);
    EXPECT_TRUE(ret);
    EXPECT_EQ(3, value);
    ret = lru_hashmap->Get(4, value, is_lru);
    EXPECT_TRUE(ret);
    EXPECT_EQ(4, value);
    ret = lru_hashmap->Get(5, value, is_lru);
    EXPECT_TRUE(ret);
    EXPECT_EQ(5, value);

    ret = lru_hashmap->Remove(3);
    EXPECT_TRUE(ret);
    EXPECT_EQ(4, lru_hashmap->GetSize());
    ret = lru_hashmap->Get(3, value, is_lru);
    EXPECT_FALSE(ret);

    ret = lru_hashmap->Release();
    EXPECT_TRUE(ret);
}

TEST(LruHashMapTest, TestLru)
{
    LruHashMap<int32_t, int32_t> *lru_hashmap = new LruHashMap<int32_t, int32_t>;
    
    int32_t size = 5;
    bool is_lru = true;
    bool ret = lru_hashmap->Init(size);
    EXPECT_TRUE(ret);
    EXPECT_EQ(0, lru_hashmap->GetSize());
    EXPECT_EQ(size, lru_hashmap->GetMaxNodeNum());
    
    int32_t key = 0;
    int32_t value = -1;
    ret = lru_hashmap->Get(key, value, is_lru);
    EXPECT_FALSE(ret);
    EXPECT_EQ(-1, value);
    
    EXPECT_EQ(0, lru_hashmap->GetSize());
    ret = lru_hashmap->Put(key, value, is_lru);
    EXPECT_TRUE(ret);
    EXPECT_EQ(1, lru_hashmap->GetSize());
    EXPECT_EQ(size, lru_hashmap->GetMaxNodeNum());
    
    ret = lru_hashmap->Remove(0);
    EXPECT_TRUE(ret);
    EXPECT_EQ(0, lru_hashmap->GetSize());
    EXPECT_EQ(size, lru_hashmap->GetMaxNodeNum());

    ret = lru_hashmap->Put(0, 0, is_lru);
    EXPECT_TRUE(ret);
    EXPECT_EQ(1, lru_hashmap->GetSize());
    ret = lru_hashmap->Put(1, 1, is_lru);
    EXPECT_TRUE(ret);
    EXPECT_EQ(2, lru_hashmap->GetSize());
    //test lru
    ret = lru_hashmap->Get(0, value, is_lru);
    EXPECT_TRUE(ret);
    EXPECT_EQ(0, value);
    
    ret = lru_hashmap->Put(2, 2, is_lru);
    EXPECT_TRUE(ret);
    EXPECT_EQ(3, lru_hashmap->GetSize());
    ret = lru_hashmap->Put(3, 3, is_lru);
    EXPECT_TRUE(ret);
    EXPECT_EQ(4, lru_hashmap->GetSize());
    ret = lru_hashmap->Put(4, 4, is_lru);
    EXPECT_TRUE(ret);
    EXPECT_EQ(5, lru_hashmap->GetSize());
    ret = lru_hashmap->Put(5, 5, is_lru);
    EXPECT_TRUE(ret);
    EXPECT_EQ(5, lru_hashmap->GetSize());

    value = -1;
    ret = lru_hashmap->Get(1, value, is_lru);
    EXPECT_FALSE(ret);
    EXPECT_EQ(-1, value);

    ret = lru_hashmap->Get(0, value, is_lru);
    EXPECT_TRUE(ret);
    EXPECT_EQ(0, value);
    ret = lru_hashmap->Get(2, value, is_lru);
    EXPECT_TRUE(ret);
    EXPECT_EQ(2, value);
    ret = lru_hashmap->Get(3, value, is_lru);
    EXPECT_TRUE(ret);
    EXPECT_EQ(3, value);
    ret = lru_hashmap->Get(4, value, is_lru);
    EXPECT_TRUE(ret);
    EXPECT_EQ(4, value);
    ret = lru_hashmap->Get(5, value, is_lru);
    EXPECT_TRUE(ret);
    EXPECT_EQ(5, value);
    //end test lru
    
    ret = lru_hashmap->Remove(3);
    EXPECT_TRUE(ret);
    EXPECT_EQ(4, lru_hashmap->GetSize());
    ret = lru_hashmap->Get(3, value, is_lru);
    EXPECT_FALSE(ret);

    ret = lru_hashmap->Release();
    EXPECT_TRUE(ret);
}

TEST(LruHashMapTest, TestCompositeDataType)
{
    LruHashMap<int32_t, Date> *lru_hashmap = new LruHashMap<int32_t, Date>;
    int32_t size = 10;
    bool ret = lru_hashmap->Init(size);
    EXPECT_TRUE(ret);
    size = lru_hashmap->GetSize();
    EXPECT_EQ(0, size);
    size = lru_hashmap->GetMaxNodeNum();
    EXPECT_EQ(10, size);
   
    for (int32_t i = 0; i < size; i++) {
        Date date;
        date.year = i + 1;
        date.month = i + 1;
        date.date = i + 1;
        ret = lru_hashmap->Put(i, date, true);
        EXPECT_TRUE(ret);
    }
    size = lru_hashmap->GetSize();
    EXPECT_EQ(10, size);
       
    for (int32_t i = 0; i < 10; i++) {
        Date date;
        ret = lru_hashmap->Get(i, date, true);
        EXPECT_TRUE(ret);
        EXPECT_EQ(i + 1, date.year);
        EXPECT_EQ(i + 1, date.month);
        EXPECT_EQ(i + 1, date.date);
    }
 
    for (int32_t i = 0; i < 10; i++) {
        ret = lru_hashmap->Remove(i);
        EXPECT_TRUE(ret);
    }
    size = lru_hashmap->GetSize();
    EXPECT_EQ(0, size);
    
    for (int32_t i = 0; i < 10; i++) {
        Date date;
        ret = lru_hashmap->Get(i, date, true);
        EXPECT_FALSE(ret);
    }
    //test lru
    for (int32_t i = 0; i < 10; i++) {
        Date date;
        date.year = i + 1;
        date.month = i + 1;
        date.date = i + 1;
        ret = lru_hashmap->Put(i, date, true);
        EXPECT_TRUE(ret);
    }
    size = lru_hashmap->GetSize();
    EXPECT_EQ(10, size);
 
    Date value;
    ret = lru_hashmap->Get(0, value, true);
    EXPECT_TRUE(ret);
    EXPECT_EQ(1, value.year);
    EXPECT_EQ(1, value.month);
    EXPECT_EQ(1, value.date);

    value.year = 11;
    value.month = 11;
    value.date = 11;
    ret = lru_hashmap->Put(1, value, true);
    EXPECT_TRUE(ret);
    
    size = lru_hashmap->GetSize();
    EXPECT_EQ(10, size);
   
    for (int32_t i = 10; i < 13; i++) {
        Date value;
        value.year = i;
        value.month = i;
        value.date = i;
        ret = lru_hashmap->Put(i, value, true);
        EXPECT_TRUE(ret);
    }
    size = lru_hashmap->GetSize();
    EXPECT_EQ(10, size);

    ret = lru_hashmap->Get(0, value, true);
    EXPECT_TRUE(ret);
    ret = lru_hashmap->Get(1, value, true);
    EXPECT_TRUE(ret);
    ret = lru_hashmap->Get(2, value, true);
    EXPECT_FALSE(ret);
    ret = lru_hashmap->Get(3, value, true);
    EXPECT_FALSE(ret);
    ret = lru_hashmap->Get(4, value, true);
    EXPECT_FALSE(ret);
    //end lru test
    
    lru_hashmap->Release();
}

TEST(LruHashMapTest, TestDup)
{
    LruHashMap<int32_t, int32_t> *lru_hashmap = new LruHashMap<int32_t, int32_t>;
    
    int32_t size = 2;
    bool is_lru = true;
    bool ret = lru_hashmap->Init(size);
    EXPECT_TRUE(ret);
    EXPECT_EQ(0, lru_hashmap->GetSize());
    EXPECT_EQ(size, lru_hashmap->GetMaxNodeNum());

    ret = lru_hashmap->Put(1, 1, is_lru);
    ret = lru_hashmap->Put(1, 2, is_lru);
    EXPECT_EQ(1, lru_hashmap->GetSize());

    int32_t value = 0;
    lru_hashmap->Get(1, value, is_lru);
    EXPECT_EQ(2, value);
    value = 0;
    lru_hashmap->Remove(1);
    lru_hashmap->Get(1, value, is_lru);
    EXPECT_EQ(0, value);
} 
