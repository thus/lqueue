/*
 * Simple queue implementation.
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

#include <stdlib.h>
#include <pthread.h>

#include "lqueue.h"

#define QUEUE_NUM_SHUTDOWN_ITEMS 100

/**
 * \brief Create the queue.
 *
 * \retval queue Pointer to the created queue.
 * \retval NULL on failure.
 */
LQueue *lqueue_create(void)
{
    LQueue *queue = calloc(1, sizeof(LQueue));
    if (queue == NULL)
        return NULL;

    if (pthread_mutex_init(&queue->mutex, NULL) != 0)
        return NULL;

    if (pthread_cond_init(&queue->cond, NULL) != 0)
        return NULL;

    return queue;
}

/**
 * \brief Free the queue.
 *
 * \param queue Pointer to the queue.
 */
void lqueue_destroy(LQueue *queue)
{
    pthread_mutex_destroy(&queue->mutex);
    pthread_cond_destroy(&queue->cond);

    if (queue != NULL)
        free(queue);
}

/**
 * \brief Check if queue is empty.
 *
 * \param q Pointer to queue.
 *
 * \retval 1 if empty.
 * \retval 0 if non-empty.
 */
int lqueue_is_empty(LQueue *q)
{
    int retval = 1;

    pthread_mutex_lock(&q->mutex);

    if (q->len != 0)
        retval = 0;

    pthread_mutex_unlock(&q->mutex);

    return retval;
}

/**
 * \brief Inject NULL items to shut down threads.
 *
 * Threads block on exit because of the pthread condition. To fix this
 * we inject lots of "empty" items.
 *
 * \param q Pointer to the queue.
 */
void lqueue_shutdown(LQueue *q)
{
    for (int i = 0; i < QUEUE_NUM_SHUTDOWN_ITEMS; i++)
    {
        lqueue_enqueue(q, NULL);
    }
}

/**
 * \brief Free queue.
 *
 * \param q Pointer to the queue.
 * \param free_func Function pointer to free item.
 */
void lqueue_free(LQueue *q, void (*free_func)(void *))
{
    if (q == NULL)
        return;

    while (1)
    {
        if (lqueue_is_empty(q))
            break;

        void *item = lqueue_dequeue(q);

        if (item == NULL)
            continue;

        else if (free_func != NULL)
            free_func(item);

        /* Basic types like strings, integers and similar does not need
           a free function, since we can just free them. */
        else
            free(item);
    }

    lqueue_destroy(q);
}

