/***************************************************************************
 * 
 * Copyright (c) 2014 Baidu.com, Inc. All Rights Reserved
 * 
 **************************************************************************/
 
 
 
/**
 * @file gconfig.h
 * @author zhangguoqiang01(com@baidu.com)
 * @date 2014/02/20 21:43:30
 * @brief 
 *  
 **/




#ifndef  __GCONFIG_H_
#define  __GCONFIG_H_

extern "C"
{
#include "config.h"
}
#include "log/log.h"
#include "util/util.h"

namespace foxea {

class FoxeaConfig
{
public:
    FoxeaConfig();
    ~FoxeaConfig();
    bool Init(const char *conf_full_name, FoxeaLog *log);
    void Release();
    void PrintInfo();
public:
    struct config *m_cfg;
    FoxeaLog *m_log;
    char m_listen_host[24];
    uint16_t m_http_port;
    // seconds
    int m_http_request_timeout;

    // request queue
    int m_request_queue_size;
};
}
#endif  //__GCONFIG_H_

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
