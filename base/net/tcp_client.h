// Author: zhangguoqiang01 <80176975@qq.com>
#ifndef TCP_CLINET_H_
#define TCP_CLINET_H_

#define TIME_OUT 5000
#include "dsalgo/head.h"
class TcpClient
{
public:
    TcpClient();
    ~TcpClient();
public:
    bool Init(const char *ip, const uint16_t port);
    bool Connect();
    bool Send(char *data, int32_t len);
    bool Recv(char *data, int32_t len);
    int32_t Writen(int32_t fd, const void *src, int32_t count);
    int32_t Readn(int32_t fd, void *src, int32_t count);
    bool Release();
private:
    char m_ip[32];
    uint16_t m_port;
    int32_t m_socket;
    bool is_connect_ok;
    int32_t m_timeout;
};
#endif // TCP_CLINET_H_
