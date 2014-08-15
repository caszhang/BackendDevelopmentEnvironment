/***************************************************************************
 * 
 * Copyright (c) 2014 Foxea.com, Inc. All Rights Reserved
 * 
 **************************************************************************/
 
 
 
/**
 * @file http_server.h
 * @author zhangguoqiang01(com@foxea.com)
 * @date 2014/02/21 11:25:27
 * @brief 
 *  
 **/




#ifndef  __HTTP_SERVER_H_
#define  __HTTP_SERVER_H_
#include <event.h>
#include <evhttp.h>
#include <event2/event.h>
#include <event2/http.h>
#include <event2/buffer.h>
#include <event2/util.h>
#include <event2/keyvalq_struct.h>

#include "common/common.h"
#include "conf/gconfig.h"
#include "log/log.h"
#include "os/thread_base.h"

namespace foxea {

class HttpServer:public ThreadBase
{
public:
    HttpServer();
    ~HttpServer();
    bool Init(FoxeaConfig *config, FoxeaLog *log, RequestQueue *request_queue, RequestPool *request_pool);
    void Release();
    virtual void  Run();
    virtual void UnRun();
    void GetServerInfo();
private:
    // for http server
    struct event_base *m_base;
    struct evhttp *m_http;
    struct evhttp_bound_socket *m_bound_handle;
    char m_http_host[24];
    uint16_t m_http_port;
    int m_http_request_timeout;
private:
    // for http server
    FoxeaConfig *m_config;
public:
    FoxeaLog *m_log;
    RequestQueue *m_request_queue;
    RequestPool *m_request_pool;
};

}

#endif  //__HTTP_SERVER_H_

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
