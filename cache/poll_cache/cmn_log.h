/*
 * cmn_log.h
 *
 *  Created on: 2013年10月11日
 *      Author: root
 */

#ifndef CMN_LOG_H_
#define CMN_LOG_H_

#include <syslog.h>

#define MY_LOG_DEBUG ("DEBUG")

#define EVENT_LOG(log_info, format, ...)     printf("[%s]" format, log_info, ##__VA_ARGS__);
//#define CMN_LOG(log_level, format, ...)     printf("[%s]" format, log_level, ##__VA_ARGS__);
#define CMN_LOG(log_level, format, ...)     syslog( LOG_LOCAL5 | LOG_DEBUG, "[%s][%s][%s][%d]" format, log_level, __FILE__, __func__, __LINE__, ##__VA_ARGS__)

//#define EVENT_LOG(log_info, format, ...)    syslog(LOG_LOCAL6 | LOG_DEBUG, "[%s]" format, log_info, ##__VA_ARGS__)

void syslog_close(void);

void syslog_init(void);

#endif /* CMN_LOG_H_ */
