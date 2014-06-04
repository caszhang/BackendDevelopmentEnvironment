// Author: zhangguoqiang01 <80176975@qq.com>
#include "dsalgo/head.h"
#include "tcp_client.h"

TcpClient::TcpClient()
{
    m_ip[0] = 0;
    m_port = 0;
    m_socket = -1;
    is_connect_ok = false;
    m_timeout = 0;
}

TcpClient::~TcpClient()
{}

bool TcpClient::Init(const char *ip, const uint16_t port)
{
    snprintf(m_ip, 32, "%s", ip);
    m_port = port;
    m_timeout = TIME_OUT;
    if (Connect()) {
       is_connect_ok = true; 
    }
    return true;
}


bool TcpClient::Connect()
{
    m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(m_socket == -1) {
        return false;
    }

    /*    
    int32_t on = 1;
    if (0 != setsockopt(m_socket, SOL_SOCKET, SO_KEEPALIVE, reinterpret_cast<char*>(&on), sizeof(int32_t))) {
        close(m_socket);
        return false;
    }
    */

    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(struct sockaddr_in));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(m_port);
    serv_addr.sin_addr.s_addr = inet_addr(m_ip);   

    if(0 != connect(m_socket, (struct sockaddr*)&serv_addr, sizeof(sockaddr_in))) {
        close(m_socket);
        return false;
    }

    struct timeval tv;
    tv.tv_sec  = m_timeout/1000;
    tv.tv_usec = (m_timeout%1000)*1000;
    if(0 != setsockopt(m_socket, SOL_SOCKET, SO_SNDTIMEO, reinterpret_cast<char *>(&tv), sizeof(tv))) {
        close(m_socket);
        return false;
    }
    
    if(0 != setsockopt(m_socket, SOL_SOCKET, SO_RCVTIMEO, reinterpret_cast<char *>(&tv), sizeof(tv))) {
        close(m_socket);
        return false;
    }

    return true;
}

bool TcpClient::Send(char *data, int32_t len)
{
    int32_t send_len = 0;
    if (is_connect_ok) {
        send_len = Writen(m_socket, data, len);
        if (send_len != len) {
            is_connect_ok = false;
            close(m_socket);
            return false;
        }
    } else {
        if (Connect()) {
            is_connect_ok = true;
            send_len = Writen(m_socket, data, len);
            if (send_len != len) {
                is_connect_ok = false;
                close(m_socket);
                return false;
            }
        } else {
            return false;
        }
    }
    return true;
}

int32_t TcpClient::Writen(int32_t fd, const void *src, int32_t count)
{
    int32_t write = 0;
    int32_t total = 0;
    do {
        write = send(fd, (const char*)src + total, count - total, 0);
        if (write <= 0) {
            if (errno == EINTR || errno == EAGAIN) {
                continue;
            }
            break;
        }
        total += write;
    } while(total < count);
    return total;
}


bool TcpClient::Recv(char *data, int32_t len)
{
    int32_t recv_len = 0;
    if (is_connect_ok) {
        recv_len = Readn(m_socket, data, len);
        if (recv_len != len) {
            is_connect_ok = false;
            close(m_socket);
            return false;
        }
    } else {
        if (Connect()) {
            is_connect_ok = true;
            recv_len = Readn(m_socket, data, len);
            if (recv_len != len) {
                is_connect_ok = false;
                close(m_socket);
                return false;
            }
        } else {
            return false;
        }
    }
    return true;
}

int32_t TcpClient::Readn(int32_t fd, void *desc, int32_t count)
{
    int32_t recved = 0;
    int32_t read = 0;
    do {
        read = recv(fd, (char*)desc + recved, count - recved, 0);
        if (read <= 0) {
            if (errno == EINTR || errno == EAGAIN) {
                continue;
            }
            break;
        }
        recved += read;
    } while (recved != count);
    return recved;
}

bool TcpClient::Release()
{
    if (-1 != m_socket) {
        close(m_socket);
        m_socket = -1;
    }

    return true;
}
