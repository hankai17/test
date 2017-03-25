/*
 * queue.h
 *
 *  Created on: 2013年9月29日
 *      Author: John
 */

#ifndef QUEUE_H_
#define QUEUE_H_

#include <semaphore.h>

typedef struct QUEUE_ENTRY
{
    int nLen;
    void *pvData;
    struct QUEUE_ENTRY *pstNext;
}QUEUE_ENTRY;

typedef struct
{
    QUEUE_ENTRY *pstHead;
    QUEUE_ENTRY *pstTail;
    sem_t stSem;
    pthread_mutex_t stMutex;
    int nSize;
}QUEUE_CONTROL;


#define OK              (0)
#define NG              (1)

#define RETURN_OK       (0)
#define RETURN_NG       (1)

/*
 * queue init
 *
 * [out]   pstQueueCtl pinter to queue control info
 *
 */
QUEUE_CONTROL *new_queue(void);

/*
 * insert data into the tail of queue
 * [in]    QUEUE_ENTRY *  the pointer of  queue entry
 *
 * return 0 OK
 * return 1 NG
 *
 */
int queue_add_tail(QUEUE_CONTROL *pstQueueCtl, QUEUE_ENTRY *pstEntry);

/*
 * get the first element of queue
 * [in] void
 * [out] QUEUE_ENTRY * the pointer of  queue entry
 *
 */
QUEUE_ENTRY *queue_get_first(QUEUE_CONTROL *pstQueueCtl);

/*
 *
 * destroy the queue, free memory
 */
void destroy_queue(QUEUE_CONTROL **pstQueueCtl);

int queue_get_size(QUEUE_CONTROL *pstQueueCtl);


#endif /* QUEUE_H_ */
