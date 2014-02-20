// Copyright (c) 2013, Baidu Inc.
// All rights reserved.
// Author: zhangguoqiang01 <zhangguoqiang01@baidu.com>
#ifndef UTIL_H_
#define UTIL_H_
#include <semaphore.h>
#include <stdarg.h>

#include "dsalgo/head.h"
uint64_t GetCurTimeUs();
std::string StringFormat(const char* format, ...);
void GetFullName(char *path, int32_t by_date, int32_t forward_seek_num, const int32_t max_len, const char *path_prefix);
bool WaitTime(sem_t *sem, uint32_t usec);
bool WaitTill(sem_t *sem);
bool PostTill(sem_t *sem);

#endif // UTIL_H_
