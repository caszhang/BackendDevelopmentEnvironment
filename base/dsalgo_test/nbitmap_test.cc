// Author: zhangguoqiang01 <80176975@qq.com>
#include "gtest/gtest.h"
#include "dsalgo/nbitmap.h"

TEST(NBitMapTest, TestNormal)
{
    NBitMap *nbitmap = new NBitMap;
    bool ret = nbitmap->Init(100, 3);
    EXPECT_TRUE(ret);
    int32_t bit_value = -1;
    bit_value = nbitmap->Get(10); 
    EXPECT_EQ(0, bit_value);
    nbitmap->Set(10, 5);
    bit_value = nbitmap->Get(10);
    EXPECT_EQ(5, bit_value);
    nbitmap->Clear(10);
    bit_value = nbitmap->Get(10);
    EXPECT_EQ(0, bit_value);
    delete nbitmap;
    nbitmap = NULL;
}

TEST(NBitMapTest, TestAbnormal)
{
    NBitMap *nbitmap = new NBitMap;
    bool ret = nbitmap->Init(100, 3);
    EXPECT_TRUE(ret);
    int32_t bit_value = 0;
    bit_value = nbitmap->Get(180); 
    EXPECT_TRUE(-7 < 0);
    ret = nbitmap->Set(180, 8);
    EXPECT_TRUE(false == ret);
    nbitmap->Set(180, 5);
    bit_value = -1;
    bit_value = nbitmap->Get(180);
    EXPECT_EQ(-7, bit_value);
    delete nbitmap;
    nbitmap = NULL;
}

TEST(NBitMapTest, TestMinSize)
{
    NBitMap *nbitmap = new NBitMap;
    bool ret = nbitmap->Init(0, 0);
    EXPECT_TRUE(false == ret);
    ret = nbitmap->Init(0, 1);
    EXPECT_TRUE(ret);
    int32_t bit_value = -1;
    bit_value = nbitmap->Get(0); 
    EXPECT_EQ(0, bit_value);
    nbitmap->Set(0, 1);
    bit_value = nbitmap->Get(0);
    EXPECT_EQ(1, bit_value);
    nbitmap->Clear(0);
    bit_value = nbitmap->Get(0);
    EXPECT_EQ(0, bit_value);
    
    bit_value = -1;
    bit_value = nbitmap->Get(31); 
    EXPECT_EQ(0, bit_value);
    nbitmap->Set(31, 1);
    bit_value = nbitmap->Get(31);
    EXPECT_EQ(1, bit_value);
    nbitmap->Clear(31);
    bit_value = nbitmap->Get(31);
    EXPECT_EQ(0, bit_value);
    
    delete nbitmap;
    nbitmap = NULL;
}

TEST(NBitMapTest, TestMaxSize)
{
    NBitMap *nbitmap = new NBitMap;
    uint32_t max_value = (1UL<<32) - 1;
    bool ret = nbitmap->Init(max_value, 0);
    EXPECT_TRUE(false == ret);
    ret = nbitmap->Init(max_value, 3);
    EXPECT_TRUE(ret);
    int32_t bit_value = -1;
    bit_value = nbitmap->Get(0); 
    EXPECT_EQ(0, bit_value);
    nbitmap->Set(0, 5);
    bit_value = nbitmap->Get(0);
    EXPECT_EQ(5, bit_value);
    nbitmap->Clear(0);
    bit_value = nbitmap->Get(0);
    EXPECT_EQ(0, bit_value);
    
    bit_value = -1;
    bit_value = nbitmap->Get(max_value); 
    EXPECT_EQ(0, bit_value);
    nbitmap->Set(max_value, 5);
    bit_value = nbitmap->Get(max_value);
    EXPECT_EQ(5, bit_value);
    nbitmap->Clear(max_value);
    bit_value = nbitmap->Get(max_value);
    EXPECT_EQ(0, bit_value);
    
    delete nbitmap;
    nbitmap = NULL;
}


