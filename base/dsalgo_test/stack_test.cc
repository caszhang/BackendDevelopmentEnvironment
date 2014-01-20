// Author: zhangguoqiang01 <80176975@qq.com>
#include "gtest/gtest.h"
#include "dsalgo/stack.cc"
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

TEST(StackTest, TestNormal)
{
    Stack<char> *stack = new Stack<char>;
    bool ret = stack->Init(20);
    EXPECT_TRUE(ret);
    ret = stack->IsFull();
    EXPECT_FALSE(ret);
    ret = stack->IsEmpty();
    EXPECT_TRUE(ret);
    int32_t size = stack->GetSize();
    EXPECT_EQ(0, size);
    int32_t i = 0;
    char c = 'a';
    for (i = 0; i < 20; i++) {
        ret = stack->Push(c + i);
        EXPECT_TRUE(ret);
    }
    ret = stack->IsFull();
    EXPECT_TRUE(ret);
    ret = stack->IsEmpty();
    EXPECT_FALSE(ret);
    ret = stack->Push('z');
    EXPECT_FALSE(ret);
    size = stack->GetSize();
    EXPECT_EQ(20, size);
    c += 19;
    char res = 0;
    for (i = 0; i < 20; i++) {
       ret = stack->Pop(res);
       EXPECT_TRUE(ret);
       EXPECT_EQ(res, c - i);
    }
    ret = stack->IsEmpty();
    EXPECT_TRUE(ret);
    ret = stack->IsFull();
    EXPECT_FALSE(ret);
    ret = stack->Pop(res);
    EXPECT_FALSE(ret);
    size = stack->GetSize();
    EXPECT_EQ(0, size);

    ret = stack->Release();
    EXPECT_TRUE(ret);
    delete stack;
    stack = NULL;
}

TEST(StackTest, TestCompositeDataType)
{
    Stack<Date> *stack = new Stack<Date>;
    bool ret = stack->Init(20);
    EXPECT_TRUE(ret);
    ret = stack->IsFull();
    EXPECT_FALSE(ret);
    ret = stack->IsEmpty();
    EXPECT_TRUE(ret);
    int32_t size = stack->GetSize();
    EXPECT_EQ(0, size);
    int32_t i = 0;
    for (i = 0; i < 20; i++) {
        Date d;
        d.year = i;
        d.month = i;
        d.date = i;
        ret = stack->Push(d);
        EXPECT_TRUE(ret);
    }
    ret = stack->IsFull();
    EXPECT_TRUE(ret);
    ret = stack->IsEmpty();
    EXPECT_FALSE(ret);
    Date d;
    ret = stack->Push(d);
    EXPECT_FALSE(ret);
    size = stack->GetSize();
    EXPECT_EQ(20, size);
    Date res;
    for (i = 0; i < 20; i++) {
        ret = stack->Pop(res);
        EXPECT_TRUE(ret);
        EXPECT_EQ(res.year, 19 - i);
        EXPECT_EQ(res.month, 19 - i);
        EXPECT_EQ(res.date, 19 - i);
    }
    ret = stack->IsEmpty();
    EXPECT_TRUE(ret);
    ret = stack->IsFull();
    EXPECT_FALSE(ret);
    ret = stack->Pop(res);
    EXPECT_FALSE(ret);
    size = stack->GetSize();
    EXPECT_EQ(0, size);

    ret = stack->Release();
    EXPECT_TRUE(ret);
}
