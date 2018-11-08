/*
 * cmn_log.c
 *
 *  Created on: 2013年10月11日
 *      Author: root
 */

#include "cmn_log.h"

void syslog_init(void)
{
    openlog("CAPTURE", LOG_ODELAY | LOG_PID, LOG_LOCAL5 | LOG_LOCAL6);
    //setlogmask(LOG_DEBUG);
}

void syslog_close(void)
{
    closelog();
}

