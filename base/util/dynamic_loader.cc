#include "dynamic_loader.h"


DynamicLoader::DynamicLoader()
{
    m_work_instance = NULL;
    m_load_instance = NULL;
    pthread_mutex_init(&m_mutex, NULL);
    m_work_instance_num = 0;
    m_load_instance_num = 0;
    m_continue = true;
}


DynamicLoader::~DynamicLoader()
{
}


bool DynamicLoader::Init()
{
    if (!LoadInstance()) {
        return false; 
    }
    return true;
}

void DynamicLoader::Run()
{
    while (m_continue) {
        if (NeedLoad()) {
            LoadInstance();
            SetNoNeedLoad();
        }
        sleep(10);
    }
}

void DynamicLoader::UnRun()
{
    m_continue = false;
}

bool DynamicLoader::LoadInstance()
{
    if (!DoLoadInstance(&m_load_instance)) {
        return false; 
    }

    void *temp = m_work_instance;

    pthread_mutex_lock(&m_mutex);
    m_work_instance = m_load_instance;
    m_load_instance = temp;
    m_load_instance_num = m_work_instance_num;
    m_work_instance_num = 0;
    pthread_mutex_unlock(&m_mutex);

    while (0 != m_load_instance_num) {
        usleep(1000);
    }

    if (NULL != m_load_instance) {
        ReleaseInstance(&m_load_instance);
    }

    return true;
}


void DynamicLoader::GetWorkInstance(void **temp)
{
    pthread_mutex_lock(&m_mutex);
    m_work_instance_num++;
    *temp = m_work_instance;
    pthread_mutex_unlock(&m_mutex);
}


void DynamicLoader::ReleaseWorkInstance(void *temp)
{
    pthread_mutex_lock(&m_mutex);
    if (temp == m_work_instance) {
        m_work_instance_num--;
    } else if (temp == m_load_instance) {
        m_load_instance_num--;
    }
    pthread_mutex_unlock(&m_mutex);
}


void DynamicLoader::Release()
{
    if (NULL != m_load_instance) {
        ReleaseInstance(&m_load_instance);
    }

    if (NULL != m_work_instance) {
        ReleaseInstance(&m_work_instance);
    }

    pthread_mutex_destroy(&m_mutex);
}

