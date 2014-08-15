/***************************************************************************
 * 
 * Copyright (c) 2014 Foxea.com, Inc. All Rights Reserved
 * 
 **************************************************************************/
 
 
 
/**
 * @file server.cc
 * @author zhangguoqiang01(com@foxea.com)
 * @date 2014/02/20 21:26:54
 * @brief 
 *  
 **/
#include <openssl/sha.h>
#include "server.h"

namespace foxea {

const char *kConfPath = "../conf/foxea.conf";
const char *kFoxeaExit = "../bin/foxea.exit";

void* distribute(void *param)
{
    Server *server = reinterpret_cast<Server*>(param);
    RequestSession *request_session = NULL;
    while (server->m_continue || server->m_request_queue->GetNodeNum() > 0) {
        request_session = NULL;
        bool ret = server->m_request_queue->WaitTimePop(request_session, kMaxSessionWaitTimeUs);
        if (!ret) {
            continue;
        }
        server->ParseAndDistribute(request_session);
        server->m_request_pool->ReleaseSession(request_session);
    }
    return NULL;
}

void Server::ParseAndDistribute(RequestSession *request_session)
{
    struct evhttp_request *req = request_session->m_req;
    std::string log_msg;
    const char *uri = NULL;
    struct evhttp_uri *decode = NULL;
    const char *request_path = NULL;

    // decode uri
    uri = evhttp_request_get_uri(req);
    if (NULL == uri) {
        m_log->LogError("evhttp_request_get_uri call failed"); 
        evhttp_send_error(req, 404, "Document not found");
        NotifyRequestSession(request_session);
        return;
    }
    log_msg = StringFormat("uri:[%s]", uri);
    m_log->LogInfo(log_msg);

    decode = evhttp_uri_parse(uri); 
    if (NULL == decode) {
        log_msg = StringFormat("evhttp_uri_parse [%s] call failed", uri); 
        m_log->LogError(log_msg);
        evhttp_send_error(req, HTTP_BADREQUEST, 0);
        evhttp_send_error(req, 404, "Document not found");
        NotifyRequestSession(request_session);
        return;
    }
    // the path the user asked for
    request_path = evhttp_uri_get_path(decode);
    if (NULL == request_path) {
        request_path = "/"; 
    }
    log_msg = StringFormat("request_path:[%s]", request_path);
    m_log->LogInfo(log_msg);

    if (0 == strcmp(request_path, request_path)) {
        // add business handler
    } else {
        // output, response
        struct evbuffer *res_buf = evbuffer_new();
        evhttp_add_header(req->output_headers, "Server", "FOXEA VERSION 1.0");
        evhttp_add_header(evhttp_request_get_output_headers(req), "Content-Type", "text/html; charset=UTF-8");
        evbuffer_add_printf(res_buf, "unknow request_path, requst uri:%s\n", uri);
        evhttp_send_reply(req, HTTP_OK, "ok", res_buf);
        evbuffer_free(res_buf);
        NotifyRequestSession(request_session);
    }

    evhttp_uri_free(decode);
    return;
}

Server::Server()
{
    m_log = NULL;
    m_config = NULL;
    m_http_server = NULL;
    m_request_queue = NULL;
    m_request_pool = NULL;
    m_continue = false;
}

Server::~Server()
{}

bool Server::Init(FoxeaLog *log)
{
    if (NULL == log) {
        return false; 
    }
    m_log = log;

    m_config = new FoxeaConfig;
    if (!m_config->Init(kConfPath, m_log)) {
        m_log->LogError("m_config init failed");
        delete m_config;
        m_config = NULL;
        Release(); 
        return false;
    }
    m_config->PrintInfo();


    m_continue = true;
    m_request_queue = new RequestQueue;
    if (!m_request_queue->Init(m_config->m_request_queue_size)) {
        delete m_request_queue;
        m_request_queue = NULL;
        Release();
        return false;
    }

    m_request_pool = new RequestPool;
    if (!m_request_pool->Init(m_config->m_request_queue_size)) {
        delete m_request_pool;
        m_request_pool = NULL;
        Release();
        return false;
    }

    m_http_server = new HttpServer;
    if (!m_http_server->Init(m_config, m_log, m_request_queue, m_request_pool)) {
        delete m_http_server;
        m_http_server = NULL;
        Release();
        m_log->LogError("m_http_server init failed");
        return false; 
    }

   
    return true;
}


void Server::Start()
{
    // start distribute
    if (0 != pthread_create(&m_distribute_thread, NULL, distribute, this)) {
         m_log->LogError("pthread_create m_distribute_thread init failed"); 
         assert(0);
    }

    // start recv request
    m_http_server->Start();

    if (0 == access(kFoxeaExit, F_OK)) {
        remove(kFoxeaExit);
    }

    while (true) {
        if (0 == access(kFoxeaExit, F_OK)) {
            remove(kFoxeaExit);
            m_http_server->Stop();    
            m_continue = false;
            pthread_join(m_distribute_thread, NULL);
            break;
        }
        sleep(1);
    }
    return;
}

void Server::Release()
{
    if (NULL != m_http_server) {
        m_http_server->Release();
        delete m_http_server;
        m_http_server = NULL; 
    }

    if (NULL != m_request_pool) {
        m_request_pool->Release();
        delete m_request_pool;
        m_request_pool = NULL;
    }

    if (NULL != m_request_queue) {
        m_request_queue->Release();
        delete m_request_queue;
        m_request_queue = NULL;
    }


    if (NULL != m_config) {
        m_config->Release();
        delete m_config;
        m_config = NULL; 
    }


    return;
}


}
/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
