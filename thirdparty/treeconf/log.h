/**
 * @author: ideawu
 * @url: http://www.ideawu.net
 * @description: C 语言使用配置文件库.
 */
#ifndef UTIL__LOG_H
#define UTIL__LOG_H

#define log_debug(fmt, args...)	printf("[LOG] " __FILE__ "(%d): " fmt "\n", __LINE__, ##args)
#define log_error log_debug

#endif
