// Copyright (c) 2012, Tencent Inc.
// All rights reserved.
// Author: caszhang <caszhang@tencent.com>
// Fixed size, support multithreaded, circular queue

#include "dsalgo/common_queue.cc"
#include "gtest/gtest.h"

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

void* ThreadFirstForPush(void *param)
{
    CommonQueue<int32_t> *queue = (CommonQueue<int32_t>*)param;
    bool ret = false;
    for (int32_t i = 0; i < 128; i++) {
        ret = queue->WaitTillPush(i);
        EXPECT_TRUE(ret);
    }
    return NULL;
}

void* ThreadSecondForPush(void *param)
{
    CommonQueue<int32_t> *queue = (CommonQueue<int32_t>*)param;
    bool ret = false;
    for (int32_t i = 0; i < 128; i++) {
        ret = queue->WaitTillPush(i);
        EXPECT_TRUE(ret);
    }
    return NULL;
}

TEST(CommonQueueTest, TestSingleThread)
{
    CommonQueue<int32_t> *queue = new CommonQueue<int32_t>();

    EXPECT_TRUE(NULL != queue);
    bool ret = false;
    ret = queue->Init(128);
    EXPECT_TRUE(ret);
    int32_t size = static_cast<int32_t>(queue->GetSize());
    EXPECT_EQ(128, size);
    int32_t node_num = static_cast<int32_t>(queue->GetNodeNum());
    EXPECT_EQ(0, node_num);
    for (int32_t i = 0; i < 128; i++) {
        ret = queue->WaitTillPush(i);
        EXPECT_TRUE(ret);
    }
    node_num = static_cast<int32_t>(queue->GetNodeNum());
    EXPECT_EQ(128, node_num);
    int32_t value = -1; 
    for (int32_t i = 0; i < 128; i++) {
        ret = queue->WaitTillPop(value);
        EXPECT_TRUE(ret);
        EXPECT_EQ(i, value);
    }
    node_num = static_cast<int32_t>(queue->GetNodeNum());
    EXPECT_EQ(0, node_num);

    const uint32_t kMaxWaitTimeUs = (128 * 1280);
    for (int32_t i = 0; i < 10; i++) {
        ret = queue->WaitTimePop(value, kMaxWaitTimeUs);
        EXPECT_FALSE(ret);
    }

    for (int32_t i = 0; i < 128; i++) {
        ret = queue->WaitTimePush(i, kMaxWaitTimeUs);
        EXPECT_TRUE(ret);
    }

    for (int32_t i = 0; i < 10; i++) {
        ret = queue->WaitTimePush(i, kMaxWaitTimeUs);
        EXPECT_FALSE(ret);
    }

    queue->Release();
}

TEST(CommonQueueTest, TestMutilThread)
{
    CommonQueue<int32_t> *queue = new CommonQueue<int32_t>();

    EXPECT_TRUE(NULL != queue);
    bool ret = false;
    ret = queue->Init(128);
    EXPECT_TRUE(ret);
    int32_t size = static_cast<int32_t>(queue->GetSize());
    EXPECT_EQ(128, size);
    int32_t node_num = static_cast<int32_t>(queue->GetNodeNum());
    EXPECT_EQ(0, node_num);

    pthread_t thread_id_first;
    int32_t is_ok = pthread_create(&thread_id_first, NULL, ThreadFirstForPush, queue);
    EXPECT_EQ(0, is_ok);

    pthread_t thread_id_second;
    is_ok = pthread_create(&thread_id_second, NULL, ThreadSecondForPush, queue);
    EXPECT_EQ(0, is_ok);

    int32_t value = -1; 
    int32_t res = 0;
    for (int32_t i = 0; i < 256; i++) {
        ret = queue->WaitTillPop(value);
        EXPECT_TRUE(ret);
        res += value;
    }
    EXPECT_EQ(16256, res);
    
    const uint32_t kMaxWaitTimeUs = (128 * 1280);
    for (int32_t i = 0; i < 10; i++) {
        ret = queue->WaitTimePop(value, kMaxWaitTimeUs);
        EXPECT_FALSE(ret);
    }

    queue->Release();
}

TEST(CommonQueueTest, TestCompositeDataType)
{
    CommonQueue<Date> *queue = new CommonQueue<Date>();
    
    EXPECT_TRUE(NULL != queue);
    bool ret = false;
    ret = queue->Init(10);
    EXPECT_TRUE(ret);
    int32_t size = static_cast<int32_t>(queue->GetSize());
    EXPECT_EQ(16, size);
    int32_t node_num = static_cast<int32_t>(queue->GetNodeNum());
    EXPECT_EQ(0, node_num);

    Date now;
    now.date = 1;
    Date yes;
    yes.date = 2;
    ret = queue->WaitTillPush(now);
    EXPECT_TRUE(ret);
    ret = queue->WaitTillPush(yes);
    EXPECT_TRUE(ret);
    
    Date res;
    ret = queue->WaitTillPop(res);
    EXPECT_TRUE(ret);
    EXPECT_EQ(1, res.date);
    
    ret = queue->WaitTillPop(res);
    EXPECT_TRUE(ret);
    EXPECT_EQ(2, res.date);

    queue->Release();
}
