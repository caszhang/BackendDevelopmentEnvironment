/*******************************************************************************
 * Author :          guoqiang
 * Email :           guoqiang@foxea.com
 * Last modified :   2014-02-24 10:15
 * Filename :        common.h
 * Description :     
 * *****************************************************************************/
#ifndef __COMMON_H_
#define __COMMON_H_
#include <event.h>
#include <evhttp.h>
#include <event2/event.h>
#include <event2/http.h>
#include <event2/buffer.h>
#include <event2/util.h>
#include <event2/keyvalq_struct.h>

#include <openssl/sha.h>
#include "dsalgo/common_queue.cc"
#include "dsalgo/lru_hashmap.cc"
#include "dsalgo/session_pool.cc"
#include "dsalgo/skiplist.cc"
#include "dsalgo/head.h"
#include "util/util.h"

namespace foxea {

#pragma pack(1)

struct RequestSession {
    struct evhttp_request *m_req;
    sem_t m_work_sem;
    sem_t *m_p_work_sem;
    RequestSession() {
        m_req = NULL; 
        int ret = sem_init(&m_work_sem, 0, 0);
        if (-1 == ret) {
            assert(0);
        }
        m_p_work_sem = &m_work_sem;
    }

    ~RequestSession() {
        m_req = NULL;
        sem_destroy(&m_work_sem);
        m_p_work_sem = NULL;
    }

    RequestSession & operator = (RequestSession & request_session) {
        m_req = request_session.m_req;
        m_p_work_sem = &(request_session.m_work_sem);
        return *this;
    }
};

#pragma pack()

typedef CommonQueue<RequestSession*> RequestQueue;
typedef SessionPool<RequestSession> RequestPool;
const uint32_t kMaxSessionWaitTimeUs = (500);
void MdSha(const char *input_str, unsigned char *output, uint32_t output_len);
void MdShaStr(const char *input_str, std::string &output);
bool WaitRequestSession(RequestSession *req_session);
bool NotifyRequestSession(RequestSession *req_session);

}
#endif
