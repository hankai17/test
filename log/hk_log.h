#ifndef HK_LOG_H_
#define HK_LOG_H_
#include <syslog.h>

#define MY_LOG_DEBUG ("DEBUG")
#define EVENT_LOG(log_info, format, ...)     printf("[%s]" format, log_info, ##__VA_ARGS__);
#define HK_LOG(log_level, format, ...)     syslog( LOG_LOCAL1 | LOG_DEBUG, "[%s][%s][%s][%d]" format, log_level, __FILE__, __func__, __LINE__, ##__VA_ARGS__)

void syslog_close(void);
void syslog_init(void);

#endif /* HK_LOG_H_ */
