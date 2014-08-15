/***************************************************************************
 * 
 * Copyright (c) 2014 Foxea.com, Inc. All Rights Reserved
 * 
 **************************************************************************/
 
 
 
/**
 * @file server.h
 * @author zhangguoqiang01(com@foxea.com)
 * @date 2014/02/20 21:24:22
 * @brief 
 *  
 **/




#ifndef  __SERVER_H_
#define  __SERVER_H_
#include "common/common.h"
#include "conf/gconfig.h"
#include "log/log.h"
#include "net/http_server.h"

namespace foxea {

class Server
{
public:
    Server();
    ~Server();
    bool Init(FoxeaLog *log);
    void Release();
    void Start();
    void ParseAndDistribute(RequestSession *request_session);

private:
    FoxeaLog *m_log;
    FoxeaConfig *m_config;
    HttpServer *m_http_server;
    pthread_t m_distribute_thread;
public:
    RequestQueue *m_request_queue;
    RequestPool *m_request_pool;
    bool m_continue;
};
}
#endif  //__SERVER_H_

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
