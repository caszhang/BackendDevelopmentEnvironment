// Author: zhangguoqiang01 <80176975@qq.com>
// Fixed size, support multithreaded, circular queue
// optimize: each mutex for head & tail, 2013
#ifndef COMMON_QUEUE_CC_H_
#define COMMON_QUEUE_CC_H_
#include "common_queue.h"
template <class T>
CommonQueue<T>::CommonQueue()
{
    m_data = NULL;
    m_tail = 0;
    m_head = 0;
    m_size = 0;
    //m_node_num = 0;
}

template <class T>
CommonQueue<T>::~CommonQueue()
{}

template <class T>
bool CommonQueue<T>::Init(uint32_t size)
{
    m_size = 1;
    while (m_size < size) {
        m_size <<= 1;
    }
    //m_size = size;

    m_data = new T[m_size];
    if (NULL == m_data) {
        return false;
    }
    memset(m_data, 0, m_size * sizeof(T));
    int32_t ret = pthread_mutex_init(&m_w_mutex, NULL);
    if (-1 == ret) {
        return false;
    }
    ret = pthread_mutex_init(&m_r_mutex, NULL);
    if (-1 == ret) {
        return false;
    }


    ret = sem_init(&m_work_sem, 0, 0);
    if (-1 == ret) {
        return false;
    }

    ret = sem_init(&m_free_sem, 0, m_size);
    if (-1 == ret) {
        return false;
    }
    return true;
}
template <class T>
bool CommonQueue<T>::Release()
{
    if (m_data) {
        delete[] m_data;
        m_data = NULL;
    }
    pthread_mutex_destroy(&m_w_mutex);
    pthread_mutex_destroy(&m_r_mutex);

    sem_destroy(&m_work_sem);
    sem_destroy(&m_free_sem);
    return true;
}

template <class T>
uint32_t CommonQueue<T>::GetSize()
{
    return m_size;
}

template <class T>
uint32_t CommonQueue<T>::GetNodeNum()
{
    int32_t node_num = 0;
    sem_getvalue(&m_work_sem, &node_num);
    return node_num;
    //return static_cast<uint32_t>(m_node_num);
}

template <class T>
bool CommonQueue<T>::WaitTillPush(const T& node)
{
    int32_t ret = 0;
    while (true) {
        ret = sem_wait(&m_free_sem);
        if (-1 == ret) {
            if (errno == EINTR) {
                continue;
            } else {
                return false;
            }
        } else {
            break;
        }
    }

    ret =  pthread_mutex_lock(&m_w_mutex);
    assert(-1 != ret);
    m_data[m_tail++] = node;
    //m_tail = m_tail % m_size;
    m_tail = m_tail & (m_size - 1);
    //m_node_num++;
    ret = sem_post(&m_work_sem);
    if (-1 == ret) {
        pthread_mutex_unlock(&m_w_mutex);
        return false;
    }

    ret = pthread_mutex_unlock(&m_w_mutex);
    assert(-1 != ret);
    return true;
}

template <class T>
bool CommonQueue<T>::WaitTillPop(T& node)
{
    int32_t ret = 0;
    while (true) {
        ret = sem_wait(&m_work_sem);
        if (-1 == ret) {
            if (errno == EINTR) {
                continue;
            } else {
                return false;
            }
        } else {
            break;
        }
    }

    ret = pthread_mutex_lock(&m_r_mutex);
    assert(-1 != ret);
    node = m_data[m_head++];
    //m_head = m_head % m_size;
    m_head = m_head & (m_size - 1);
    //m_node_num--;
    ret = sem_post(&m_free_sem);
    if (-1 == ret) {
        pthread_mutex_unlock(&m_r_mutex);
        return false;
    }
    ret = pthread_mutex_unlock(&m_r_mutex);
    assert(-1 != ret);
    return true;
}

template <class T>
bool CommonQueue<T>::WaitTimePush(const T& node, uint32_t usec)
{
    struct timespec ts;
    struct timeval tv;

    gettimeofday(&tv, NULL);
    ts.tv_sec = tv.tv_sec;
    ts.tv_nsec = (tv.tv_usec  + usec) * 1000;
    ts.tv_sec += ts.tv_nsec / (1000 * 1000 * 1000);
    ts.tv_nsec %= 1000 * 1000 * 1000;
    int32_t ret = 0;

    while (true) {
        ret = sem_timedwait(&m_free_sem, &ts);
        if (-1 == ret) {
            if (errno == EINTR) {
                continue;
            } else {
                return false;
            }
        } else {
            break;
        }
    }

    ret =  pthread_mutex_lock(&m_w_mutex);
    assert(-1 != ret);
    m_data[m_tail++] = node;
    //m_tail = m_tail % m_size;
    m_tail = m_tail & (m_size - 1);
    //m_node_num++;
    ret = sem_post(&m_work_sem);
    if (-1 == ret) {
        pthread_mutex_unlock(&m_w_mutex);
        return false;
    }

    ret = pthread_mutex_unlock(&m_w_mutex);
    assert(-1 != ret);
    return true;
}

template <class T>
bool CommonQueue<T>::WaitTimePop(T& node, uint32_t usec)
{
    struct timespec ts;
    struct timeval tv;

    gettimeofday(&tv, NULL);
    ts.tv_sec = tv.tv_sec;
    ts.tv_nsec = (tv.tv_usec  + usec) * 1000;
    ts.tv_sec += ts.tv_nsec / (1000 * 1000 * 1000);
    ts.tv_nsec %= 1000 * 1000 * 1000;

    int32_t ret = 0;
    while (true) {
        ret = sem_timedwait(&m_work_sem, &ts);
        if (-1 == ret) {
            if (errno == EINTR) {
                continue;
            } else {
                return false;
            }
        } else {
            break;
        }
    }
    ret = pthread_mutex_lock(&m_r_mutex);
    assert(-1 != ret);
    node = m_data[m_head++];
    //m_head = m_head % m_size;
    m_head = m_head & (m_size - 1);
    //m_node_num--;
    ret = sem_post(&m_free_sem);
    if (-1 == ret) {
        pthread_mutex_unlock(&m_r_mutex);
        return false;
    }
    ret = pthread_mutex_unlock(&m_r_mutex);
    assert(-1 != ret);
    return true;
}
#endif // COMMON_QUEUE_CC_H_

