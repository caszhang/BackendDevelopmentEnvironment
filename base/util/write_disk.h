/***************************************************************************
 * 
 * Copyright (c) 2014 Baidu.com, Inc. All Rights Reserved
 * 
 **************************************************************************/
 
 
 
/**
 * @file write_disk.h
 * @author zhangguoqiang01(com@baidu.com)
 * @date 2014/01/07 17:23:00
 * @brief 
 *  
 **/
#ifndef  __WRITE_DISK_H_
#define  __WRITE_DISK_H_
#include "dsalgo/head.h"
#include "os/thread_base.h"
#include "util.h"

class WriteDisk:public ThreadBase
{
    public:
        WriteDisk();
        virtual ~WriteDisk();
    public:
        void Run();
        void UnRun();
        bool Init(int32_t size);
        void Release();
        void WriteToBuf(const char *buf, const int32_t len);
        void Flush();
        virtual void CheckAndSwitch(FILE **fd) = 0;
    private:
        char *m_work_buf;
        char *m_free_buf;
        int32_t m_work_buf_size;
        int32_t m_free_buf_size;
        sem_t m_work_sem;
        sem_t m_free_sem;
        int32_t m_max_size;
        bool m_continue;
};

#endif  //__WRITE_DISK_H_

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
