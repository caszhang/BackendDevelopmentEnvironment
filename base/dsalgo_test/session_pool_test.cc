// All rights reserved.
// Author: zhangguoqiang01 <80176975@qq.com>
// Fixed size, support multithreaded, circular session_pool

#include "gtest/gtest.h"
#include "dsalgo/session_pool.cc"
const int32_t kTempSessionSize = 64;
struct Session {    
	unsigned char data[kTempSessionSize];    
	uint32_t len;    
	Session() {        
		memset(data, 0, kTempSessionSize * sizeof(*data));        
		len = 0;    
	}
};
typedef SessionPool<Session> TaskPool;

void* ThreadFirst(void *param)
{
    TaskPool *session_pool = (TaskPool*)param;
    bool ret = false;
    Session *ss[64];
    for (int32_t i = 0; i < 64; i++) {
        ret = session_pool->GetSession(&ss[i]);
        EXPECT_TRUE(ret);
        EXPECT_TRUE(NULL != ss[i]);
    }

    for (int32_t i = 0; i < 64; i++) {
        ret = session_pool->ReleaseSession(ss[i]);
        EXPECT_TRUE(ret);
    }
    return NULL;
}

void* ThreadSecond(void *param)
{
    TaskPool *session_pool = (TaskPool*)param;
    bool ret = false;
    Session *ss[64];
    for (int32_t i = 0; i < 64; i++) {
        ret = session_pool->GetSession(&ss[i]);
        EXPECT_TRUE(ret);
        EXPECT_TRUE(NULL != ss[i]);
    }

    for (int32_t i = 0; i < 64; i++) {
        ret = session_pool->ReleaseSession(ss[i]);
        EXPECT_TRUE(ret);
    }
    return NULL;
}

TEST(TaskPoolTest, TestSingleThread)
{
    TaskPool *session_pool = NULL;
    //session_pool = TaskPool::GetInstance();    
    session_pool = new TaskPool;
    EXPECT_TRUE(NULL != session_pool);
    bool ret = false;
    ret = session_pool->Init(128);
    EXPECT_TRUE(ret);
    int32_t size = (int32_t)session_pool->GetSize();
    EXPECT_EQ(128, size);
    int32_t node_num = (int32_t)session_pool->GetNodeNum();
    EXPECT_EQ(128, node_num);
    Session *ss_array[128]; 
    for (int32_t i = 0; i < 128; i++) {
        ret = session_pool->GetSession(&ss_array[i]);
        EXPECT_TRUE(ret);
        EXPECT_TRUE(NULL != ss_array[i]);
    }
    size = (int32_t)session_pool->GetSize();
    EXPECT_EQ(128, size);
    node_num = (int32_t)session_pool->GetNodeNum();
    EXPECT_EQ(0, node_num);
    
    for (int32_t i = 0; i < 128; i++) {
        ret = session_pool->ReleaseSession(ss_array[i]);
        EXPECT_TRUE(ret);
    }
    size = (int32_t)session_pool->GetSize();
    EXPECT_EQ(128, size);
    node_num = (int32_t)session_pool->GetNodeNum();
    EXPECT_EQ(128, node_num);

    session_pool->Release();
}

TEST(TaskPoolTest, TestMutilThread)
{
    TaskPool *session_pool = NULL;
    //session_pool = TaskPool::GetInstance();    
    session_pool = new TaskPool;
    EXPECT_TRUE(NULL != session_pool);
    bool ret = false;
    ret = session_pool->Init(128);
    EXPECT_TRUE(ret);
    int32_t size = (int32_t)session_pool->GetSize();
    EXPECT_EQ(128, size);
    int32_t node_num = (int32_t)session_pool->GetNodeNum();
    EXPECT_EQ(128, node_num);

    pthread_t thread_id_first;
    int32_t is_ok = pthread_create(&thread_id_first, NULL, ThreadFirst, session_pool);
    EXPECT_EQ(0, is_ok);
    
    pthread_t thread_id_second;
    is_ok = pthread_create(&thread_id_second, NULL, ThreadSecond, session_pool);
    EXPECT_EQ(0, is_ok);
  
    pthread_join(thread_id_first, NULL); 
    pthread_join(thread_id_second, NULL); 
    session_pool->Release();
}

