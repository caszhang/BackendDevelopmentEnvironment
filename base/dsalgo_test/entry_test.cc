// Author: zhangguoqiang01 <80176975@qq.com>
// Element for lru hashmap
#include "gtest/gtest.h"
#include "dsalgo/entry.h"
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

TEST(EntryTest, TestNormal)
{
    Entry<int32_t, int32_t> *head = new Entry<int32_t, int32_t>;
    EXPECT_EQ(-1, head->next);
    EXPECT_EQ(-1, head->before);
    EXPECT_EQ(-1, head->after);
    EXPECT_EQ(-1, head->index);
    if (NULL != head) {
        delete head;
        head = NULL;
    }
}

TEST(EntryTest, TestCompositeDataType)
{
    Entry<int32_t, Date> *head = new Entry<int32_t, Date>;
    EXPECT_EQ(-1, head->next);
    EXPECT_EQ(-1, head->before);
    EXPECT_EQ(-1, head->after);
    EXPECT_EQ(-1, head->index);
    if (NULL != head) {
        delete head;
    }
}

TEST(EntryQueueTest, TestSingleThread)
{
    EntryQueue<int32_t> *queue = new EntryQueue<int32_t>();

    EXPECT_TRUE(NULL != queue);
    EXPECT_TRUE(queue->m_data == NULL);
    EXPECT_EQ(queue->m_tail, 0);
    EXPECT_EQ(queue->m_head, 0);
    EXPECT_EQ(queue->m_size, 0);
    bool ret = false;
    ret = queue->Init(100);
    EXPECT_TRUE(ret);
    EXPECT_EQ(queue->m_size, 100);
    for (int32_t i = 0; i < 100; i++) {
        ret = queue->Push(i);
        EXPECT_TRUE(ret);
    }
    EXPECT_EQ(queue->m_head, queue->m_tail);

    int32_t value = -1; 
    for (int32_t i = 0; i < 100; i++) {
        ret = queue->Pop(value);
        EXPECT_TRUE(ret);
        EXPECT_EQ(i, value);
    }
    EXPECT_EQ(queue->m_head, queue->m_tail);

    ret = queue->Release();
    EXPECT_TRUE(ret);
}

TEST(EntryQueueTest, TestCompositeDataType)
{
    EntryQueue<Date> *queue = new EntryQueue<Date>();
    
    EXPECT_TRUE(NULL != queue);
    EXPECT_TRUE(queue->m_data == NULL);
    EXPECT_EQ(queue->m_tail, 0);
    EXPECT_EQ(queue->m_head, 0);
    EXPECT_EQ(queue->m_size, 0);
    
    bool ret = false;
    ret = queue->Init(10);
    EXPECT_TRUE(ret);
    EXPECT_EQ(queue->m_size, 10);
   
    Date instance;
    for (int32_t i = 0; i < 10; i++) {
        instance.year = i;
        instance.month = i;
        instance.date = i;
        ret = queue->Push(instance);
        EXPECT_TRUE(ret);
    }
    EXPECT_EQ(queue->m_head, queue->m_tail);
    
    instance.year = -1;
    instance.month = -1;
    instance.date = -1;
    for (int32_t i = 0; i < 10; i++) {
        ret = queue->Pop(instance);
        EXPECT_TRUE(ret);
        EXPECT_EQ(i, instance.year);
        EXPECT_EQ(i, instance.month);
        EXPECT_EQ(i, instance.date);
    }
    EXPECT_EQ(queue->m_head, queue->m_tail);
    
    ret = queue->Release();
    EXPECT_TRUE(ret);
}
