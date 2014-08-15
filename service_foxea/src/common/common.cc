#include "common.h"

namespace foxea {

void MdSha(const char *input_str, unsigned char *output, uint32_t output_len)
{
    unsigned char md[SHA_DIGEST_LENGTH];
    SHA1(reinterpret_cast<const unsigned char*>(input_str), strlen(input_str), md);
    memcpy(output, md, output_len);
}

void MdShaStr(const char *input_str, std::string &output)
{
    unsigned char md[SHA_DIGEST_LENGTH];
    SHA1(reinterpret_cast<const unsigned char*>(input_str), strlen(input_str), md);
    output.clear();
    for (int i = 0; i < SHA_DIGEST_LENGTH; i++) {
        output.append(StringFormat("%02x", md[i]));
    }
}

bool WaitRequestSession(RequestSession *req_session)
{
    int32_t ret = 0;
    while (true) {
        ret = sem_wait(req_session->m_p_work_sem);
        if (-1 == ret) {
            if (errno == EINTR) {
                continue;
            } else {
                return false;
            }
        } else {
            break;
        }
    }
    return true;
}

bool NotifyRequestSession(RequestSession *req_session)
{
    int32_t ret = sem_post(req_session->m_p_work_sem);
    if (-1 == ret) {
        return false;
    }
    return true;
}


}
