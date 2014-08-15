/***************************************************************************
 * 
 * Copyright (c) 2013 Baidu.com, Inc. All Rights Reserved
 * 
 **************************************************************************/
 
 
 
/**
 * @file log.h
 * @author zhangguoqiang01(com@baidu.com)
 * @date 2013/12/20 16:19:05
 * @brief 
 *  
 **/




#ifndef  __LOG_H_
#define  __LOG_H_
#include "glog/logging.h"

namespace foxea {
class FoxeaLog {
public:    
    void LogInfo(const std::string log_msg); 
    void LogError(const std::string log_msg); 
    void LogDebug(const std::string log_msg, const uint32_t level = 1); 
    void LogWarning(const std::string log_msg); 
};
}
#endif  //__LOG_H_

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
