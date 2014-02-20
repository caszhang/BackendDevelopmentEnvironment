// Author: zhangguoqiang01 <80176975@qq.com>
#ifndef DB_H_
#define DB_H_
#include "dsalgo/head.h"
#include "mysql/mysql.h"
class DB
{
public:
    DB();
    ~DB();
public:
    bool Init(const char *host, const char *user,
            const char *passwd, const char *db, unsigned int port);
    bool Query(const char *sql, MYSQL_RES * &result);
    bool Operate(const char *sql);
    void Release();
    uint32_t GetInsertId(); 
    bool Connect();
    void ReConnect();
private:
    MYSQL *m_mysql;
    char m_host[64];
    char m_user[64];
    char m_passwd[64];
    char m_db_name[64];
    uint16_t m_port;
};
#endif // DB_H_
