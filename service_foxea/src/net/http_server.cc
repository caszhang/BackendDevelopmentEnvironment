/***************************************************************************
 * 
 * Copyright (c) 2014 Baidu.com, Inc. All Rights Reserved
 * 
 **************************************************************************/
 
 
 
/**
 * @file http_server.cc
 * @author zhangguoqiang01(com@baidu.com)
 * @date 2014/02/21 11:35:34
 * @brief 
 *  
 **/

#include "http_server.h"
namespace foxea {
void help(HttpServer *http_server, struct evhttp_request *req);

void spec_httpd_handler(struct evhttp_request *req, void *param)
{
    // to remove the complie warning
    req = NULL;
    param = NULL;
}

void httpd_handler(struct evhttp_request *req, void *param)
{
    HttpServer *http_server = reinterpret_cast<HttpServer*>(param);
    RequestSession *request_session = NULL;
    if (!http_server->m_request_pool->GetSession(&request_session)) {
        http_server->m_log->LogError("m_request_pool GetSession failed");
        assert(0);
        return;
    }
    request_session->m_req = req;
    if (!http_server->m_request_queue->WaitTillPush(request_session)) {
        http_server->m_log->LogError("m_request_queue WaitTillPush failed"); 
        if (!http_server->m_request_pool->ReleaseSession(request_session)) {
            assert(0); 
        }
        return;
    }

    //help(http_server, req);
    WaitRequestSession(request_session);
    return;
}

void help(HttpServer *http_server, struct evhttp_request *req)
{
    // --------------------------split line-------------------
    char *cmd_type = NULL;
    std::string log_msg;
    struct evhttp_uri *decode = NULL;
    const char *request_path = NULL;
    char *decode_path = NULL;
    const char *uri = NULL;

    switch (evhttp_request_get_command(req)) {
    case EVHTTP_REQ_GET: 
        cmd_type = "GET"; 
        break;
    case EVHTTP_REQ_POST: 
        cmd_type = "POST"; 
        break;
    case EVHTTP_REQ_HEAD: 
        cmd_type = "HEAD"; 
        break;
    case EVHTTP_REQ_PUT: 
        cmd_type = "PUT"; 
        break;
    case EVHTTP_REQ_DELETE: 
        cmd_type = "DELETE"; 
        break;
    case EVHTTP_REQ_OPTIONS: 
        cmd_type = "OPTIONS"; 
        break;
    case EVHTTP_REQ_TRACE: 
        cmd_type = "TRACE"; 
        break;
    case EVHTTP_REQ_CONNECT: 
        cmd_type = "CONNECT"; 
        break;
    case EVHTTP_REQ_PATCH: 
        cmd_type = "PATCH"; 
        break;
    default: 
        cmd_type = "unknown"; 
        break;
    }

    log_msg = StringFormat("request cmd type:[%s]", cmd_type);
    http_server->m_log->LogInfo(log_msg);
    if (0 != strcmp("GET", cmd_type)) {
        //goto err; 
    }
    // decode uri
    uri = evhttp_request_get_uri(req);
    if (NULL == uri) {
        http_server->m_log->LogError("evhttp_request_get_uri call failed"); 
        evhttp_send_error(req, 404, "Document not found");
        return;
    }
    log_msg = StringFormat("uri:[%s]", uri);
    http_server->m_log->LogInfo(log_msg);

    decode = evhttp_uri_parse(uri); 
    if (NULL == decode) {
        log_msg = StringFormat("evhttp_uri_parse [%s] call failed", uri); 
        http_server->m_log->LogError(log_msg);
        evhttp_send_error(req, HTTP_BADREQUEST, 0);
        evhttp_send_error(req, 404, "Document not found");
        return;
    }
    // the path the user asked for
    request_path = evhttp_uri_get_path(decode);
    if (NULL == request_path) {
        request_path = "/"; 
    }
    log_msg = StringFormat("request_path:[%s]", request_path);
    http_server->m_log->LogInfo(log_msg);

    // the path the user really want
    decode_path = evhttp_uridecode(request_path, 0, NULL);
    if (NULL == decode_path) {
        http_server->m_log->LogError("evhttp_uridecode call failed");
        evhttp_send_error(req, 404, "Document not found");
        evhttp_uri_free(decode);
        return;
    }
    log_msg = StringFormat("decode_path:[%s]", decode_path);
    http_server->m_log->LogInfo(log_msg);
   
    // for parse parameters
    const char *param_uri = evhttp_request_uri(req);
    if (NULL == param_uri) {
        http_server->m_log->LogError("evhttp_request_uri call failed");
        evhttp_send_error(req, 404, "Document not found");
        evhttp_uri_free(decode);
        free(decode_path);
        return;
    }
    log_msg = StringFormat("param_uri:[%s]", param_uri);
    http_server->m_log->LogInfo(log_msg);

    char *param_decode_uri = evhttp_decode_uri(param_uri);
    if (NULL == param_decode_uri) {
        http_server->m_log->LogError("evhttp_decode_uri call failed");
        evhttp_send_error(req, 404, "Document not found");
        evhttp_uri_free(decode);
        free(decode_path);
        return;
    }
    log_msg = StringFormat("param_decode_uri:[%s]", param_decode_uri);
    http_server->m_log->LogInfo(log_msg);
    
    struct evbuffer *res_buf = evbuffer_new();

    struct evkeyvalq request_params;
    evhttp_parse_query(param_decode_uri, &request_params);
    struct evkeyval *request_param;
    for (request_param = request_params.tqh_first; NULL != request_param; request_param = request_param->next.tqe_next) {
        log_msg = StringFormat("[%s]:[%s]", request_param->key, request_param->value); 
        http_server->m_log->LogInfo(log_msg);        
        evbuffer_add_printf(res_buf, "[%s]:[%s]\n", request_param->key, request_param->value);
    }
    // output, response
    evhttp_add_header(req->output_headers, "Server", "GENE VERSION 1.0");
    evhttp_add_header(evhttp_request_get_output_headers(req), "Content-Type", "text/html; charset=UTF-8");
    evbuffer_add_printf(res_buf, "it works!\nrequst uri:%s\n", uri);
    evhttp_send_reply(req, HTTP_OK, "ok", res_buf);

    evhttp_uri_free(decode);
    evbuffer_free(res_buf);
    evhttp_clear_headers(&request_params);
    free(param_decode_uri);
    free(decode_path);
    return;
}

HttpServer::HttpServer()
{
    m_config = NULL;
    m_log = NULL;
    m_base = NULL;
    m_bound_handle = NULL;
    m_request_queue = NULL;
    m_request_pool = NULL;
}

HttpServer::~HttpServer()
{}

bool HttpServer::Init(FoxeaConfig *config, FoxeaLog *log, RequestQueue *request_queue, RequestPool *request_pool)
{
    if (NULL == config 
        || NULL == log 
        || NULL == request_queue 
        || NULL == request_pool) {
        return false; 
    }

    m_config = config;
    m_log = log;
    m_request_queue = request_queue;
    m_request_pool = request_pool;

    snprintf(m_http_host, 24, "%s", m_config->m_listen_host);
    m_http_port = m_config->m_http_port;
    m_http_request_timeout = m_config->m_http_request_timeout;

    m_base = event_base_new();
    if (NULL == m_base) {
        m_log->LogError("event_base_new call failed"); 
        return false;
    }

    m_http = evhttp_new(m_base);
    if (NULL == m_http) {
        m_log->LogError("evhttp_new call failed"); 
        return false;
    }

    evhttp_set_timeout(m_http, m_http_request_timeout);

    // specific url
    evhttp_set_cb(m_http, "/spec", spec_httpd_handler, this);
    // foxearal url
    evhttp_set_gencb(m_http, httpd_handler, this);

    // support multiple port
    m_bound_handle = evhttp_bind_socket_with_handle(m_http, m_http_host, m_http_port);
    if (NULL == m_bound_handle) {
        std::string msg = StringFormat("evhttp_bind_socket_with_handle call failed, could not bind %s:%u", m_http_host, m_http_port); 
        m_log->LogError(msg);
        return false;
    }

    GetServerInfo(); 
    return true;
}

void HttpServer::GetServerInfo()
{
    std::string log_msg;
    struct sockaddr_storage ss;
    ev_socklen_t ss_len = sizeof(ss);
    evutil_socket_t socket_fd;
    char addr_buf[128];
    void *inaddr = NULL;
    const char *addr = NULL;
    int got_port = -1;
    socket_fd = evhttp_bound_socket_get_fd(m_bound_handle);
    memset(&ss, 0, sizeof(ss));
    if (getsockname(socket_fd, (struct sockaddr*)&ss, &ss_len)) {
        m_log->LogError("getsockname failed"); 
        return;
    }

    if (AF_INET == ss.ss_family) {
        got_port = ntohs(((struct sockaddr_in*)&ss)->sin_port);
        inaddr = &((struct sockaddr_in*)&ss)->sin_addr; 
    } else if (AF_INET6 == ss.ss_family) {
        got_port = ntohs(((struct sockaddr_in6*)&ss)->sin6_port);
        inaddr = &((struct sockaddr_in6*)&ss)->sin6_addr; 
    } else {
        log_msg = StringFormat("wrong address family %d", ss.ss_family);
        m_log->LogError(log_msg); 
        return;
    }

    addr = evutil_inet_ntop(ss.ss_family, inaddr, addr_buf, sizeof(addr_buf));
    if (NULL == addr) {
        m_log->LogError("evutil_inet_ntop call failed"); 
        return;
    } else {
        log_msg = StringFormat("listening on %s:%d", addr, got_port);
        m_log->LogInfo(log_msg);
    }
    return;
}

void HttpServer::Run()
{
    event_base_dispatch(m_base);
}

void HttpServer::UnRun()
{
    event_base_loopbreak(m_base);
}

void HttpServer::Release()
{
    if (NULL != m_http) {
        evhttp_free(m_http); 
        m_http = NULL;
    }

    if (NULL != m_base) {
        event_base_free(m_base);
        m_base = NULL; 
    }
}

}
/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
