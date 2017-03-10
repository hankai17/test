#include "hk_log.h"

void syslog_init(void)
{
    //openlog("CAPTURE", LOG_ODELAY | LOG_PID, LOG_LOCAL5 | LOG_LOCAL6);
    openlog("CAPTURE", LOG_ODELAY | LOG_PID, LOG_LOCAL1);
    //setlogmask(LOG_DEBUG);
}

void syslog_close(void)
{
    closelog();
}

