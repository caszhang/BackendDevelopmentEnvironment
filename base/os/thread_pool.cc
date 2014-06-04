#include "thread_pool.h"
const int32_t kMaxThreadPoolTaskQueueSize = 512;
const uint32_t kMaxSessionWaitTimeUs = (500);

void thread_pool_routine(void *param)
{
    ThreadPool *tp = (ThreadPool*)param;
    ThreadPoolTask thread_pool_task;
    while (tp->m_continue  || tp->m_task_queue->GetNodeNum() > 0) {
        bool ret = tp->m_task_queue->WaitTimePop(thread_pool_task, kMaxSessionWaitTimeUs);
        if (!ret) {
            continue;
        }
        (*(thread_pool_task.m_function))(thread_pool_task.m_argument);
    }
}

ThreadPool::ThreadPool()
{
    m_threads = NULL;
    m_threads_num = 0;
    m_task_queue = NULL;
    m_continue = false;
}

bool ThreadPool::Init(int threads_num)
{
    if (1 > threads_num) {
        return false; 
    }
    m_continue = true;
    m_task_queue = new ThreadPoolTaskQueue;
    if (!m_task_queue->Init(kMaxThreadPoolTaskQueueSize)) {
        return false; 
    }

    m_threads = new Thread*[threads_num];
    for (int i = 0; i < threads_num; i++) {
        m_threads[i] = new Thread(thread_pool_routine, reinterpret_cast<void*>(this)); 
    }
    m_threads_num = threads_num;
    return true;
}

void ThreadPool::Start()
{
    for (int i = 0; i < m_threads_num; i++) {
        m_threads[i]->Start();
    }
}

void ThreadPool::AddTask(const ThreadPoolTask &task)
{
    m_task_queue->WaitTillPush(task);
}

void ThreadPool::Stop()
{
    m_continue = false;
    for (int i = 0; i < m_threads_num; i++) {
        m_threads[i]->Stop();
    }
}

void ThreadPool::Release()
{
    if (NULL != m_task_queue) {
        m_task_queue->Release();
        delete m_task_queue;
        m_task_queue = NULL;
    }

    if (NULL != m_threads) {
        for (int i = 0; i < m_threads_num; i++) {
            delete m_threads[i];
            m_threads[i] = NULL;
        }
        delete[] m_threads;
        m_threads = NULL;
    }
}

ThreadPool::~ThreadPool()
{}

