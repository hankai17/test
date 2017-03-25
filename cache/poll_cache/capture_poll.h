/*
 * capture_poll.h
 *
 *  Created on: 2013年10月10日
 *      Author: root
 */

#ifndef CAPTURE_POLL_H_
#define CAPTURE_POLL_H_

#include <pcap/pcap.h>
#include "queue.h"

#define MAX_MONITOR_FD   (2)

#define DNS_PCAP_HANDLE         0
#define HTTP_PCAP_HANDLE        1

#define DNS_MONITOR_TYPE        0x01
#define HTTP_MONITOR_TYPE       0x02

#define CAPTURE_SHUTDOWN_NO     (1)
#define CAPTURE_SHUTDOWN_YES    (0)

#define MAX_SLEEP_TIME          50000
#define MIN_SLEEP_TIME          100

typedef struct
{
    int nFd;
    pcap_t *pstPcap;
    int nMonitorType;
}CAPTURE_POLL;

typedef struct
{
    QUEUE_CONTROL *pstQueuControl;
    pcap_handler pfHandler;
    int nMonitorType;
}THREAD_PARAM;

extern QUEUE_CONTROL *g_apstQueuControl[MAX_MONITOR_FD];

// add by shipf 2013.9.30
extern int g_nShutDown;
extern pthread_mutex_t g_shutdown_mutex;

void *capture_poll_thread(void *pvData);
void *process_packet_thread(void *pvdata);

#endif /* CAPTURE_POLL_H_ */
