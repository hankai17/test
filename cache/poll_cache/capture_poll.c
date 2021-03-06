/*
 * capture_poll.c
 *
 *  Created on: 2013年10月10日
 *      Author: root
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>
#include <errno.h>
#include <pcap/pcap.h>
#include <sys/types.h>
#include <sys/select.h>
#include <pthread.h>
#include <poll.h>

#include "cmn_log.h"
#include "queue.h"
#include "capture_poll.h"

#define DEFINE_FD       (0)

#define QUEUE_READ_FLAG_ON      1
#define QUEUE_READ_FLAG_OFF     0

// add by shipf 2013.9.30
int g_nShutDown = CAPTURE_SHUTDOWN_NO;
pthread_mutex_t g_shutdown_mutex = PTHREAD_MUTEX_INITIALIZER;

QUEUE_CONTROL *g_apstQueuControl[MAX_MONITOR_FD] = { NULL, NULL }; //2个队列  

#if 1
static pthread_cond_t g_aThreadCond[] = { PTHREAD_COND_INITIALIZER,
PTHREAD_COND_INITIALIZER };
static pthread_mutex_t g_aThreadMutex[] = { PTHREAD_MUTEX_INITIALIZER,
PTHREAD_MUTEX_INITIALIZER };

#if 0
static pthread_mutex_t g_QueueCanReadMutex = PTHREAD_MUTEX_INITIALIZER;
static int g_nQueueCanReadFlag = QUEUE_READ_FLAG_OFF;

static void queue_read_flag_on(void);
static void queue_read_flag_off(void);
static int get_queue_read_flag(void);
#endif
#endif

static void process_handler(u_char *user, const struct pcap_pkthdr *h,
        const u_char *bytes);

void *capture_poll_thread(void *pvData)
{
    struct pollfd stPollFd;
    int polltimeout = 500;
    int status = -1;
    CAPTURE_POLL *pstPoll = (CAPTURE_POLL *) pvData;
    int nRet = CAPTURE_SHUTDOWN_YES;

    stPollFd.fd = pstPoll->nFd;
    stPollFd.events = POLLIN;

    for (;;)
    {
        pthread_mutex_lock(&g_shutdown_mutex);
        nRet = g_nShutDown;                             
        pthread_mutex_unlock(&g_shutdown_mutex);
        if (CAPTURE_SHUTDOWN_YES == nRet)
        {
            break;
        }
        status = poll(&stPollFd, 1, polltimeout);  //poll机制监听in事件 500ms超时
        if (status == -1)
        {
            CMN_LOG(MY_LOG_DEBUG, "Poll returns error (%s)\n", strerror(errno));
        }
        else
        {
            if (status == 0)
            {
                //nothing to do
            }
            else
            {
                if (stPollFd.revents & POLLIN)
                {
#if 0
                    queue_read_flag_on();
#else
                    pthread_mutex_lock(&g_aThreadMutex[pstPoll->nMonitorType]);
                    pthread_cond_broadcast(
                            &g_aThreadCond[pstPoll->nMonitorType]);  //有数据到来 发条件变量通知
                    pthread_mutex_unlock(&g_aThreadMutex[pstPoll->nMonitorType]);
#endif
                    pcap_dispatch(pstPoll->pstPcap, 1, process_handler, //开始抓包  抓包过程用到了 添加队列
                            (u_char *) &pstPoll->nMonitorType);
                    //queue_read_flag_off();
                }
            }
        }
    }

    return NULL;
}

#if 0
static void queue_read_flag_on(void)
{
    pthread_mutex_lock(&g_QueueCanReadMutex);
    g_nQueueCanReadFlag = QUEUE_READ_FLAG_ON;
    pthread_mutex_unlock(&g_QueueCanReadMutex);
}

static void queue_read_flag_off(void)
{
    pthread_mutex_lock(&g_QueueCanReadMutex);
    g_nQueueCanReadFlag = QUEUE_READ_FLAG_OFF;
    pthread_mutex_unlock(&g_QueueCanReadMutex);
}

static int get_queue_read_flag(void)
{
    pthread_mutex_lock(&g_QueueCanReadMutex);
    int nRet = g_nQueueCanReadFlag;
    pthread_mutex_unlock(&g_QueueCanReadMutex);

    return nRet;
}
#endif

#if 1 // add by shipf 2013.9.30
/*
 * process all packet
 *
 */
void *process_packet_thread(void *pvdata) //传给各线程的参数是全局的 
{
    QUEUE_ENTRY *pstQueueEntry = NULL;
    int nRet = CAPTURE_SHUTDOWN_NO;
    struct timespec stTime;
    //int nQueueReadFlag = QUEUE_READ_FLAG_OFF;
    //int nIsHavadata = 0;   // 1 have  0 no data
    int nSleepTime = MAX_SLEEP_TIME;

    THREAD_PARAM *pstParam = NULL;
    pstParam = (THREAD_PARAM *) pvdata;

    CMN_LOG(MY_LOG_DEBUG, "-------------process_packet_thread start!\n");

    while (1)
    {
        pthread_mutex_lock(&g_shutdown_mutex);
        nRet = g_nShutDown;
        pthread_mutex_unlock(&g_shutdown_mutex);
        if (CAPTURE_SHUTDOWN_YES == nRet)
        {
            break;
        }

#if 0
        nQueueReadFlag = get_queue_read_flag();
        if (nQueueReadFlag == QUEUE_READ_FLAG_OFF)
        {
            if (nIsHavadata == 0)
            {
                clock_gettime(CLOCK_REALTIME, &stTime);
                stTime.tv_sec += 1;
                stTime.tv_nsec = 0;
                pthread_mutex_lock(&g_aThreadMutex[pstParam->nMonitorType]);
                pthread_cond_timedwait(&g_aThreadCond[pstParam->nMonitorType],
                        &g_aThreadMutex[pstParam->nMonitorType], &stTime);
                pthread_mutex_unlock(&g_aThreadMutex[pstParam->nMonitorType]);
            }
            else
            {
                pthread_cond_broadcast(&g_aThreadCond[pstParam->nMonitorType]);
            }
        }
#endif
        pstQueueEntry = queue_get_first(pstParam->pstQueuControl);  //取队列第一个元素 (关键是什么时候插入的？)
        if (pstQueueEntry != NULL)
        {
            pstParam->pfHandler(NULL, NULL, pstQueueEntry->pvData);  //一看就是解包回调
            //queue_read_flag_on();
            //nIsHavadata = 1;   // 1 have  0 no data
            nSleepTime = MIN_SLEEP_TIME; //100
            if (pstQueueEntry->pvData != NULL)
            {
                free(pstQueueEntry->pvData);
                pstQueueEntry->pvData = NULL;
            }

            if (pstQueueEntry != NULL)
            {
                free(pstQueueEntry);
                pstQueueEntry = NULL;
            }
        }
        else                                                        //抓包线程是先通知，然后再抓包 后添加队列。如果队列为空 说明没有数据到来 等待 等条件通知  没有看懂？？？？？？
        {
            //queue_read_flag_off();
            //nIsHavadata = 0;   // 1 have  0 no data
            usleep(nSleepTime); //100微秒
            if (nSleepTime >= MAX_SLEEP_TIME) //50000微秒
            {
                nSleepTime = MAX_SLEEP_TIME;
                clock_gettime(CLOCK_REALTIME, &stTime);
                stTime.tv_sec += 1;  //秒
                stTime.tv_nsec = 0;  //纳秒
                pthread_mutex_lock(&g_aThreadMutex[pstParam->nMonitorType]);
                pthread_cond_timedwait(&g_aThreadCond[pstParam->nMonitorType],  //精妙！  这就是发送条件广播的接受时机
                        &g_aThreadMutex[pstParam->nMonitorType], &stTime);
                pthread_mutex_unlock(&g_aThreadMutex[pstParam->nMonitorType]);
            }
            nSleepTime += MIN_SLEEP_TIME;
        }
    }
    CMN_LOG(MY_LOG_DEBUG, "-------------process_packet_thread end!\n");

    return NULL;
}

/*
 * process dns
 */
static void process_handler(u_char *user, const struct pcap_pkthdr *h,
        const u_char *bytes)
{
    int nRet = RETURN_NG;
    QUEUE_ENTRY *pstQueueEntry = NULL;

    int nIndex = (int) *user;

    pstQueueEntry = (QUEUE_ENTRY *) calloc(1, sizeof(QUEUE_ENTRY));  //每抓一个包 就calloc出一个entry结构体(里面有泛型void* len)
    if (pstQueueEntry == NULL)
    {
        CMN_LOG(MY_LOG_DEBUG, "calloc error!\n");
        nRet = RETURN_NG;
    }
    else
    {
        nRet = RETURN_NG;
        pstQueueEntry->pstNext = NULL;
        pstQueueEntry->nLen = h->caplen;
        pstQueueEntry->pvData = (void *) calloc(1, pstQueueEntry->nLen);
        if (pstQueueEntry->pvData != NULL)
        {
            memcpy(pstQueueEntry->pvData, bytes, pstQueueEntry->nLen);
            nRet = queue_add_tail(g_apstQueuControl[nIndex], pstQueueEntry);  //然后把每个entry add到全局队列里 //里面有锁 是安全的
            //nRet = RETURN_OK;
        }

        if (nRet != RETURN_OK)
        {
            if (pstQueueEntry->pvData != NULL)
            {
                free(pstQueueEntry->pvData);
                pstQueueEntry->pvData = NULL;
            }

            if (pstQueueEntry != NULL)
            {
                free(pstQueueEntry);
                pstQueueEntry = NULL;
            }
        }
    }

    return;
}
#endif

