/*
 * queue.c
 *
 *  Created on: 2013年9月29日
 *      Author: John
 */

#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <pthread.h>
#include <errno.h>

#include "cmn_log.h"
#include "queue.h"

#define MAX_QUEUE_SIZE   50000

/*
 * queue init
 *
 * [out]   pstQueueCtl pinter to queue control info
 *
 */
QUEUE_CONTROL *new_queue(void)
{
    QUEUE_CONTROL *pstQueueCtl = (QUEUE_CONTROL *) calloc(1,
            sizeof(QUEUE_CONTROL));
    if (pstQueueCtl != NULL)
    {
        int nRet = sem_init(&pstQueueCtl->stSem, 0, 0); //初始化信号量为0
        if (OK != nRet)
        {
            CMN_LOG(MY_LOG_DEBUG, "sem_init error!");
        }
        pthread_mutex_init(&pstQueueCtl->stMutex, NULL); //初始化一把锁
    }

    return pstQueueCtl;
}

int queue_get_size(QUEUE_CONTROL *pstQueueCtl)
{
    int nSize = -1;
    pthread_mutex_lock(&pstQueueCtl->stMutex);
    nSize = pstQueueCtl->nSize;
    pthread_mutex_unlock(&pstQueueCtl->stMutex);

    CMN_LOG(MY_LOG_DEBUG, "queue size is %d!\n", nSize);

    return nSize;
}

/*
 * insert data into the tail of queue
 * [in]    QUEUE_ENTRY *  the pointer of  queue entry
 *
 * return 0 OK
 * return 1 NG
 *
 */
int queue_add_tail(QUEUE_CONTROL *pstQueueCtl, QUEUE_ENTRY *pstEntry)
{
    int nRet = NG;
    pthread_mutex_lock(&pstQueueCtl->stMutex); //插入数据到队尾,需要加锁
    if (pstQueueCtl->nSize + 1 <= MAX_QUEUE_SIZE)
    {
        if (pstQueueCtl->pstHead == NULL)
        {
            pstQueueCtl->pstHead = pstEntry;
            pstQueueCtl->pstTail = pstEntry;
            pstQueueCtl->pstTail->pstNext = NULL;
        }
        else //与链表操作一样
        {
            pstQueueCtl->pstTail->pstNext = pstEntry;
            pstQueueCtl->pstTail = pstQueueCtl->pstTail->pstNext;
            pstQueueCtl->pstTail->pstNext = NULL;
        }
        pstQueueCtl->nSize++;
        nRet = OK;
    }
    pthread_mutex_unlock(&pstQueueCtl->stMutex);
    if (nRet == OK)
    {
        sem_post(&pstQueueCtl->stSem); //插入成功 信号量++  //记住wait=0时等待,不为0时--
    }

    return nRet;
}

/*
 * get the first element of queue
 * [in] void
 * [out] QUEUE_ENTRY * the pointer of  queue entry
 *
 */
QUEUE_ENTRY *queue_get_first(QUEUE_CONTROL *pstQueueCtl)
{
    QUEUE_ENTRY *pstRet = NULL;

#if 1
    struct timespec stTime;
    clock_gettime(CLOCK_REALTIME, &stTime);

    stTime.tv_sec += 2;

    // time wait, if timeout then do nothing
    if (OK == sem_timedwait(&pstQueueCtl->stSem, &stTime)) //即在规定的时间内可以--
#else
        if (OK == sem_trywait(&pstQueueCtl->stSem))
#endif
        {
            pthread_mutex_lock(&pstQueueCtl->stMutex); //那就--
            if (pstQueueCtl->pstHead != NULL)
            {
                pstRet = pstQueueCtl->pstHead;
                pstQueueCtl->pstHead = pstQueueCtl->pstHead->pstNext;
                if (NULL == pstQueueCtl->pstHead)
                {
                    pstQueueCtl->pstTail = NULL;
                }
                pstRet->pstNext = NULL;
                pstQueueCtl->nSize--;
            }
            pthread_mutex_unlock(&pstQueueCtl->stMutex);
        }

    return pstRet;
}

/*
 *
 * destroy the queue, free memory
 */
void destroy_queue(QUEUE_CONTROL **pstQueueCtl)
{
    if (*pstQueueCtl != NULL)
    {
        QUEUE_ENTRY *pstTemp = (*pstQueueCtl)->pstHead;
        while (pstTemp != NULL)
        {
            if (pstTemp->pvData != NULL)
            {
                free(pstTemp->pvData);
                pstTemp->pvData = NULL;
            }
            free(pstTemp);
            pstTemp = pstTemp->pstNext;
        }

        sem_destroy(&((*pstQueueCtl)->stSem));
        (*pstQueueCtl)->pstHead = NULL;
        (*pstQueueCtl)->pstTail = NULL;

        free(*pstQueueCtl);
        *pstQueueCtl = NULL;
    }

    return;
}

