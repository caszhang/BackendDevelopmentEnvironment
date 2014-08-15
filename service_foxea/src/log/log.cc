/***************************************************************************
 * 
 * Copyright (c) 2013 Foxea.com, Inc. All Rights Reserved
 * 
 **************************************************************************/
 
 
 
/**
 * @file log.cc
 * @author zhangguoqiang01(com@foxea.com)
 * @date 2013/12/20 16:28:03
 * @brief 
 *  
 **/
#include "log.h"
namespace foxea {

void FoxeaLog::LogInfo(const std::string log_msg)
{
    LOG(INFO) << log_msg;
}

void FoxeaLog::LogError(const std::string log_msg)
{
    LOG(ERROR) << log_msg;
}

void FoxeaLog::LogDebug(const std::string log_msg, const uint32_t level)
{
    VLOG(level) << log_msg;
}

void FoxeaLog::LogWarning(const std::string log_msg)
{
    LOG(WARNING) << log_msg;
}

}
/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
