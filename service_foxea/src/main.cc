/***************************************************************************
 * 
 * Copyright (c) 2014 Foxea.com, Inc. All Rights Reserved
 * 
 **************************************************************************/
 
 
 
/**
 * @file main.cc
 * @author zhangguoqiang01(com@foxea.com)
 * @date 2014/02/20 20:34:37
 * @brief 
 *  
 **/
#include "dsalgo/head.h"
#include "log/log.h"
#include "server/server.h"

const char *kLogDir = "../log";
// 100 MB
const int32_t kMaxEachLogSize = 100;

void init_signal()
{
    signal(SIGPIPE,SIG_IGN);
    signal(SIGCHLD,SIG_IGN);
    signal(SIGINT,SIG_IGN);
    signal(SIGTERM,SIG_IGN);
    return;
}

void init_log(const char *program_name)
{
    google::InitGoogleLogging(program_name);
    google::SetStderrLogging(google::INFO);
    FLAGS_log_dir = kLogDir;
    FLAGS_max_log_size = kMaxEachLogSize;
    return; 
}

int main(int argc, char* argv[])
{
    init_signal();
    init_log(argv[0]);
    foxea::FoxeaLog *log_manager = new foxea::FoxeaLog;
    log_manager->LogInfo("foxea begin to work!");
    foxea::Server *server = new foxea::Server;
    if (!server->Init(log_manager)) {
        log_manager->LogError("sever init failed"); 
        server->Release();
        delete server;
        delete log_manager;
        google::ShutdownGoogleLogging();
        return -1;
    }
    server->Start();
    server->Release();
    delete server;
    log_manager->LogInfo("foxea exit!");
    delete log_manager;
    google::ShutdownGoogleLogging();
    // remove the complie warning
    argc = 0;
    sleep(1);
    return 0;
}

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
