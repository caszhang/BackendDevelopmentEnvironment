/*******************************************************************************
 * Author :          guoqiang
 * Email :           guoqiang@foxea.com
 * Last modified :   2014-02-24 13:09
 * Filename :        handler.h
 * Description :     
 * *****************************************************************************/
#ifndef __HANDLER_H_
#define __HANDLER_H_
#include "common/common.h"
#include "conf/gconfig.h"
#include "log/log.h"
#include "os/thread_base.h"

namespace foxea {

class Handler:public ThreadBase
{
public:
    Handler();
    virtual ~Handler();
    virtual bool Init(FoxeaLog *log, FoxeaConfig *config);
    virtual void Release();
    void Process(RequestSession *req);
    virtual void Handle(RequestSession *request_session) = 0;
    virtual void Run();
    virtual void UnRun();
public:
    FoxeaLog *m_log;
    FoxeaConfig *m_config;
    RequestQueue *m_request_queue;
    RequestPool *m_request_pool;
    bool m_continue;
};

}

#endif
