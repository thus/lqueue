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

#include "lqueue.h"

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

