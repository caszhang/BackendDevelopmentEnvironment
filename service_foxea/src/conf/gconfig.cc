/***************************************************************************
 * 
 * Copyright (c) 2014 Baidu.com, Inc. All Rights Reserved
 * 
 **************************************************************************/
 
 
 
/**
 * @file gconfig.cc
 * @author zhangguoqiang01(com@baidu.com)
 * @date 2014/02/20 21:47:11
 * @brief 
 *  
 **/
#include "gconfig.h"
namespace foxea {
FoxeaConfig::FoxeaConfig()
{
    m_cfg = NULL;
    m_log = NULL;
}

FoxeaConfig::~FoxeaConfig()
{}

bool FoxeaConfig::Init(const char *conf_full_name, FoxeaLog *log)
{
    if (NULL == conf_full_name || NULL == log) {
        return false; 
    }
    m_log = log;

    m_cfg = cfg_load_file(const_cast<char*>(conf_full_name));
    if (!m_cfg) {
        return false; 
    }

    snprintf(m_listen_host, 24, "%s", cfg_getstr(m_cfg, "net.listen_host"));
    m_http_port = cfg_getnum(m_cfg, "net.http_port");
    m_http_request_timeout = cfg_getnum(m_cfg, "net.http_request_timeout");

    m_request_queue_size = cfg_getnum(m_cfg, "net.request_queue_size");
    
    
    return true;
}

void FoxeaConfig::PrintInfo()
{
    std::string msg_log = StringFormat("m_listen_host:[%s], m_http_port:[%u], m_http_request_timeout:[%d] m_request_queue_size:[%d]", m_listen_host, m_http_port, m_http_request_timeout, m_request_queue_size);
    m_log->LogInfo(msg_log);
}

void FoxeaConfig::Release()
{
    if (NULL != m_cfg) {
        cfg_free(m_cfg);
        m_cfg = NULL;
    }
}

}
/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
