// Copyright (c) 2013, Baidu Inc.
// All rights reserved.
// Author: zhangguoqiang01 <zhangguoqiang01@baidu.com>
#include "util.h"
uint64_t GetCurTimeUs()
{
	struct timeval stv;

	struct timezone stz;
	stz.tz_minuteswest = 0;
	stz.tz_dsttime = 0;
	gettimeofday(&stv,  &stz);
	return (stv.tv_sec * (uint64_t)1000000 + stv.tv_usec);
}

std::string StringFormat(const char* format, ...)
{
	char buf[1024];
	va_list arglist;
	va_start(arglist, format);
	vsnprintf(buf, 1024, format, arglist);
	va_end(arglist);
	return std::string(buf);
}

void GetFullName(char *path, int32_t by_date, int32_t forward_seek_num, const int32_t max_len, const char *path_prefix)
{
	time_t lt = time(NULL);
	if (1 == by_date) {
		lt -= forward_seek_num * 24 *3600;
	} else {
		lt -= forward_seek_num * 3600;
	}
	struct tm temp_tm;// = {0};
	localtime_r(&lt, &temp_tm);
	if (1 == by_date) {
		snprintf(path, max_len, "%s.%04d.%02d.%02d", path_prefix, temp_tm.tm_year+1900, temp_tm.tm_mon + 1, temp_tm.tm_mday);
	} else {
		snprintf(path, max_len, "%s.%04d.%02d.%02d.%02d", path_prefix, temp_tm.tm_year+1900, temp_tm.tm_mon + 1, temp_tm.tm_mday, temp_tm.tm_hour);
	}
	return;
}

bool WaitTime(sem_t *sem, uint32_t usec)
{
	struct timespec ts;
	struct timeval tv;

	gettimeofday(&tv, NULL);
	ts.tv_sec = tv.tv_sec;
	ts.tv_nsec = (tv.tv_usec  + usec) * 1000;
	ts.tv_sec += ts.tv_nsec / (1000 * 1000 * 1000);
	ts.tv_nsec %= 1000 * 1000 * 1000;
	int32_t ret = 0;

	while (true) {
		ret = sem_timedwait(sem, &ts);
		if (-1 == ret) {
			if (errno == EINTR) {
				continue;
			} else {
				return false;
			}
		} else {
			return true;
		}
	}
}

bool WaitTill(sem_t *sem)
{
	int32_t ret = 0;
	while (true) {
		ret = sem_wait(sem);
		if (-1 == ret) {
			if (errno == EINTR) {
				continue;
			} else {
				return false;
			}
		} else {
			return true;
		}
	}
}

bool PostTill(sem_t *sem)
{
	int32_t ret = 0;
	while (true) {
		ret = sem_post(sem);
		if (-1 == ret) {
			if (errno == EINTR) {
				continue;
			} else {
				return false;
			}
		} else {
			return true;
		}
	}
}

