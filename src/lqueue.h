/*
 * Simple locked queue implementation.
 * Copyright (C) 2017  Mats Klepsland <mats.klepsland@gmail.com>
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 * 
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA  02110-1301, USA.
 */

#ifndef __LQUEUE_H__
#define __LQUEUE_H__

#include <stdlib.h>
#include <inttypes.h>
#include <pthread.h>

typedef struct LQueueItem_ {
    void *data;
    struct LQueueItem_ *next;
} LQueueItem;

typedef struct LQueue_ {
    LQueueItem      *head;
    LQueueItem      *tail;
    uint32_t        len;
    pthread_mutex_t mutex;
    pthread_cond_t  cond;
} LQueue;

/**
 * \brief Push item onto the queue.
 *
 * \param queue Pointer to the queue.
 * \param item  Pointer to data to add.
 *
 * \retval 0 on success.
 * \retval -1 on failure.
 */
static inline int lqueue_enqueue(LQueue *queue, void *data)
{
    if (queue == NULL || data == NULL)
        return -1;

    LQueueItem *item = malloc(sizeof(LQueueItem));
    if (item == NULL)
        return -1;

    item->next = NULL;
    item->data = data;

    pthread_mutex_lock(&queue->mutex);

    if (queue->len == 0) {
        queue->head = item;
        queue->tail = item;
    }
    else {
        queue->tail->next = item;
        queue->tail = item;
    }

    queue->len++;

    pthread_cond_signal(&queue->cond);
    pthread_mutex_unlock(&queue->mutex);

    return 0;
}

/**
 * \brief Pop item from the queue.
 *
 * \param queue Pointer to the queue.
 *
 * \retval data Pointer to the data from the item.
 * \retval NULL on failure.
 */
static inline void *lqueue_dequeue(LQueue *queue)
{
    if (queue == NULL)
        return NULL;

    pthread_mutex_lock(&queue->mutex);

    while (queue->len == 0)
        pthread_cond_wait(&queue->cond, &queue->mutex);

    LQueueItem *item = queue->head;
    void *data = item->data;

    if (item->next == NULL)
        queue->head = NULL;
    else
        queue->head = item->next;

    queue->len--;

    pthread_mutex_unlock(&queue->mutex);

    free(item);

    return data;
}

LQueue *lqueue_create(void);

void lqueue_destroy(LQueue *);

int queue_is_empty(LQueue *);

void queue_shutdown(LQueue *);

void queue_free(LQueue *, void (*free_func)(void *));

#endif /* __LQUEUE_H__ */

