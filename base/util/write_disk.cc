/***************************************************************************
 * 
 * Copyright (c) 2014 Baidu.com, Inc. All Rights Reserved
 * 
 **************************************************************************/



/**
 * @file write_disk.cc
 * @author zhangguoqiang01(com@baidu.com)
 * @date 2014/01/07 17:23:12
 * @brief 
 *  
 **/

#include <unistd.h>
#include "write_disk.h"

WriteDisk::WriteDisk()
{
    m_work_buf = NULL;
    m_free_buf = NULL;
}

WriteDisk::~WriteDisk()
{}

bool WriteDisk::Init(int32_t size)
{
    m_max_size = size;
    m_work_buf = new char[size];
    m_work_buf_size = 0;
    m_free_buf = new char[size];
    m_free_buf_size = 0;

    int32_t ret = sem_init(&m_work_sem, 0, 0);
    if (-1 == ret) {
        return false;
    }

    ret = sem_init(&m_free_sem, 0, 1);
    if (-1 == ret) {
        return false;
    }
    m_continue = true;
    return true;
}

void WriteDisk::Run()
{
    FILE *fd = NULL;
    bool ret = false;
    while (m_continue) {
        ret = false;
        ret = WaitTime(&m_work_sem, 5000);
        if (ret) {
            fd = NULL;
            CheckAndSwitch(&fd);
            if (1 != fwrite(m_free_buf, m_free_buf_size, 1, fd)) {
                assert(0);
            }
            fflush(fd);
            m_free_buf_size = 0;
            ret =  PostTill(&m_free_sem);
            if (!ret) {
                assert(0);
            }
        }
        usleep(1000 * 100);
    }

    if (m_work_buf_size > 0) {
        fd = NULL;
        CheckAndSwitch(&fd);
        if (1 != fwrite(m_work_buf, m_work_buf_size, 1, fd)) {
            assert(0);
        }
        fflush(fd);
        m_work_buf_size = 0;
    } 
    return;
}

void WriteDisk::WriteToBuf(const char *buf, const int32_t len)
{
    if (m_work_buf_size + len >= m_max_size) {
        if (!WaitTill(&m_free_sem)) {
            assert(0);
        }
        char *temp_buf = m_work_buf;
        m_work_buf = m_free_buf;
        m_free_buf = temp_buf;

        int32_t temp_size = m_work_buf_size;
        m_work_buf_size = m_free_buf_size;
        m_free_buf_size = temp_size;
        if (!PostTill(&m_work_sem)) {
            assert(0);
        }
    }
    memcpy(m_work_buf + m_work_buf_size, buf, len);
    m_work_buf_size += len;
    return;
}

void WriteDisk::UnRun()
{
    m_continue = false;
}

void WriteDisk::Flush()
{
    if (m_work_buf_size > 0) {
        FILE *fd = NULL;
        CheckAndSwitch(&fd);
        if (1 != fwrite(m_work_buf, m_work_buf_size, 1, fd)) {
            assert(0);
        }
        fflush(fd);
        m_work_buf_size = 0;
    }
    return;
}

void WriteDisk::Release()
{
    if (NULL != m_work_buf) {
        delete[] m_work_buf;
        m_work_buf = NULL;
    }

    if (NULL != m_free_buf) {
        delete[] m_free_buf;
        m_free_buf = NULL;
    }

    sem_destroy(&m_work_sem);
    sem_destroy(&m_free_sem);
}

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
