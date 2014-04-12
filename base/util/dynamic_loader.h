/*******************************************************************************
 * Author :          guoqiang
 * Email :           guoqiang@foxea.com
 * Last modified :   2014-04-07 00:43
 * Filename :        dynamic_loader.h
 * Description :     
 * *****************************************************************************/
#ifndef __DYNAMIC_LOADER_H_
#define __DYNAMIC_LOADER_H_
#include "dsalgo/head.h"
#include "os/thread_base.h"


class DynamicLoader:public ThreadBase
{
public:
    DynamicLoader();
    virtual ~DynamicLoader();
    bool Init();
    void Release();
    bool LoadInstance();
    virtual bool DoLoadInstance(void **instance) = 0;
    virtual void ReleaseInstance(void **instance) = 0;
    virtual bool NeedLoad() = 0;
    virtual void SetNoNeedLoad() = 0;
    void GetWorkInstance(void **temp);
    void ReleaseWorkInstance(void *temp);
    virtual void Run();
    virtual void UnRun();
private:
    void *m_work_instance;
    void *m_load_instance;
    pthread_mutex_t m_mutex;
    uint32_t m_work_instance_num;
    uint32_t m_load_instance_num;
    bool m_continue;
};

#endif // __DYNAMIC_LOADER_H_
