// Author: zhangguoqiang01 <80176975@qq.com>
#include <iostream>
#include <string>
#include "db.h"
DB::DB()
{}

DB::~DB()
{
    m_mysql = NULL;
}

bool DB::Init(const char *host, const char *user, 
        const char *passwd, const char *db, unsigned int port)
{
    if (NULL == host || NULL == user || NULL == passwd || NULL == db) {
   	return false; 
    }

    snprintf(m_host, 64, "%s", host);
    snprintf(m_user, 64, "%s", user);
    snprintf(m_passwd, 64, "%s", passwd);
    snprintf(m_db_name, 64, "%s", db);
    m_port = port;
    if (!Connect()) {
   	return false; 
    }
    return true;
}

bool DB::Connect()
{
    m_mysql = mysql_init(NULL);
    if (NULL == m_mysql) {
        fprintf(stderr, "fail to mysql_init Error:%s\n", mysql_error(m_mysql));
        return false;
    }
    
    if (0 != mysql_options(m_mysql, MYSQL_SET_CHARSET_NAME, "utf8")) 
    {
        fprintf(stderr, "mysql_options Error: %s\n", mysql_error(m_mysql));
        Release();
        return false;
    }

    if (!mysql_real_connect(m_mysql, m_host, m_user, m_passwd, m_db_name, m_port, NULL, 0)) {
        fprintf(stderr, "connect db %s Error:%s\n", m_db_name, mysql_error(m_mysql));
        Release();
        return false;
    }

    if (0 !=mysql_set_server_option(m_mysql, MYSQL_OPTION_MULTI_STATEMENTS_ON)) {
        fprintf(stderr, "mysql_set_server_option %d Error:%s\n", MYSQL_OPTION_MULTI_STATEMENTS_ON, mysql_error(m_mysql));
        Release();
        return false;
    }
    
    return true;
}

bool DB::Query(const char *sql, MYSQL_RES * &result)
{
    result = NULL;
    try {
        if (0 != mysql_query(m_mysql, sql)) {
            fprintf(stderr, "mysql_query %s Error:%s\n", sql, mysql_error(m_mysql));
	    ReConnect(); 
            return false;
        }
        result = mysql_store_result(m_mysql);
        //there are rows
        if (result) {
            //num_fields = mysql_num_fields(result);
            //retrieve rows, then call m_mysql_free_result(result)
        } else {
            // m_mysql_store_result() returned nothing; should it have?
            if(mysql_field_count(m_mysql) == 0) {
                // query does not return data
                // (it was not a SELECT)
            } else {
                //m_mysql_store_result() should have returned data
                fprintf(stderr, "Error: %s\n", mysql_error(m_mysql));
		ReConnect();
                return false;
            }
        } 
    } catch (...) {
	ReConnect();
        return false;
    }
    return true;
}
 
bool DB::Operate(const char *sql)
{
    MYSQL_RES * result = NULL;
    try {
        if (0 != mysql_query(m_mysql, sql)) {
            fprintf(stderr, "mysql_query %s Error:%s\n", sql, mysql_error(m_mysql));
	    ReConnect();
            return false;
        } else {
            result = mysql_store_result(m_mysql);
            //there are rows
            if (result) {
                //num_fields = mysql_num_fields(result);
                //retrieve rows, then call m_mysql_free_result(result)
            } else {
                // m_mysql_store_result() returned nothing; should it have?
                if(mysql_field_count(m_mysql) == 0) {
                    // query does not return data
                    // (it was not a SELECT)
                    //num_rows = mysql_affected_rows(m_mysql);
                } else {
                    //m_mysql_store_result() should have returned data
                    mysql_free_result(result);
                    fprintf(stderr, "Error: %s\n", mysql_error(m_mysql));
		    ReConnect();
                    return false;
                }
            } 
        }
        mysql_free_result(result);
    } catch (...) {
	ReConnect();
        return false;
    }
    return true;
}

uint32_t DB::GetInsertId()
{
    return mysql_insert_id(m_mysql);
}

void DB::ReConnect()
{
    Release();
    Connect();
}

void DB::Release()
{
    if (NULL != m_mysql) {
        mysql_close(m_mysql);
        m_mysql = NULL;
        return;
    }
}

