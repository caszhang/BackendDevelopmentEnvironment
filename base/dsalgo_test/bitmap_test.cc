// Author: zhangguoqiang01 <80176975@qq.com>
#include "gtest/gtest.h"
#include "dsalgo/bitmap.h"

TEST(BitMapTest, TestNormal)
{
    BitMap *bitmap = new BitMap;
    bool ret = bitmap->Init(100);
    EXPECT_TRUE(ret);
    int32_t bit_value = -1;
    bit_value = bitmap->Get(10); 
    EXPECT_EQ(0, bit_value);
    bitmap->Set(10);
    bit_value = bitmap->Get(10);
    EXPECT_EQ(1, bit_value);
    bitmap->Clear(10);
    bit_value = bitmap->Get(10);
    EXPECT_EQ(0, bit_value);
    delete bitmap;
    bitmap = NULL;
}

TEST(BitMapTest, TestAbnormal)
{
    BitMap *bitmap = new BitMap;
    bool ret = bitmap->Init(100);
    EXPECT_TRUE(ret);
    int32_t bit_value = -1;
    bit_value = bitmap->Get(180); 
    EXPECT_EQ(-1, bit_value);
    ret = bitmap->Set(180);
    EXPECT_TRUE(false == ret);
    bit_value = bitmap->Get(180);
    EXPECT_EQ(-1, bit_value);
    ret = bitmap->Clear(180);
    EXPECT_TRUE(false == ret);
    delete bitmap;
    bitmap = NULL;
}

TEST(BitMapTest, TestMinSize)
{
    BitMap *bitmap = new BitMap;
    bool ret = bitmap->Init(0);
    EXPECT_TRUE(ret);
    int32_t bit_value = -1;
    bit_value = bitmap->Get(0); 
    EXPECT_EQ(0, bit_value);
    bitmap->Set(0);
    bit_value = bitmap->Get(0);
    EXPECT_EQ(1, bit_value);
    bitmap->Clear(0);
    bit_value = bitmap->Get(0);
    EXPECT_EQ(0, bit_value);
    
    bit_value = -1;
    bit_value = bitmap->Get(31); 
    EXPECT_EQ(0, bit_value);
    bitmap->Set(31);
    bit_value = bitmap->Get(31);
    EXPECT_EQ(1, bit_value);
    bitmap->Clear(31);
    bit_value = bitmap->Get(31);
    EXPECT_EQ(0, bit_value);
    
    delete bitmap;
    bitmap = NULL;
}

TEST(BitMapTest, TestMaxSize)
{
    BitMap *bitmap = new BitMap;
    uint32_t max_value = (1UL<<32) - 1;
    bool ret = bitmap->Init(max_value);
    EXPECT_TRUE(ret);
    int32_t bit_value = -1;
    bit_value = bitmap->Get(0); 
    EXPECT_EQ(0, bit_value);
    bitmap->Set(0);
    bit_value = bitmap->Get(0);
    EXPECT_EQ(1, bit_value);
    bitmap->Clear(0);
    bit_value = bitmap->Get(0);
    EXPECT_EQ(0, bit_value);
    
    bit_value = -1;
    bit_value = bitmap->Get(max_value); 
    EXPECT_EQ(0, bit_value);
    bitmap->Set(max_value);
    bit_value = bitmap->Get(max_value);
    EXPECT_EQ(1, bit_value);
    bitmap->Clear(max_value);
    bit_value = bitmap->Get(max_value);
    EXPECT_EQ(0, bit_value);
    
    delete bitmap;
    bitmap = NULL;
}

