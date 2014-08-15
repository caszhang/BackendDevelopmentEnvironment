#include "handler.h"

namespace foxea {

Handler::Handler()
{
    m_log = NULL;
    m_config = NULL;
    m_request_queue = NULL;
    m_request_pool = NULL;
    m_continue = false;
}

Handler::~Handler()
{}

bool Handler::Init(FoxeaLog *log, FoxeaConfig *config)
{
    if (NULL == log || NULL == config) {
        return false; 
    }
    m_log = log;
    m_config = config;
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
    return true;
}

void Handler::Release()
{
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

}

void Handler::Process(RequestSession *req)
{
    RequestSession *request_session = NULL;
    if (!m_request_pool->GetSession(&request_session)) {
        m_log->LogError("m_request_pool GetSession failed");
        assert(0);
        return;
    }
    *request_session = *req;
    if (!m_request_queue->WaitTillPush(request_session)) {
        m_log->LogError("m_request_queue WaitTillPush failed"); 
        if (!m_request_pool->ReleaseSession(request_session)) {
            assert(0); 
        }
        return;
    }
}

void Handler::Run()
{
    RequestSession *request_session = NULL;
    while (m_continue || m_request_queue->GetNodeNum() > 0) {
        request_session = NULL;
        bool ret = m_request_queue->WaitTimePop(request_session, kMaxSessionWaitTimeUs);
        if (!ret) {
            continue;
        }
        Handle(request_session);
        NotifyRequestSession(request_session);
        m_request_pool->ReleaseSession(request_session);
    }
 
}

void Handler::UnRun()
{
    m_continue = false;
}

}
