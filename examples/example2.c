/* Simple example of using separate threads for pushing and popping */

#include <stdio.h>
#include <pthread.h>
#include <time.h>

#include <lqueue.h>

#define ENTRIES_TO_ADD 1000000

void *enqueue_thread(void *user)
{
    LQueue *queue = (LQueue *)user;

    int i;
    for (i = 0; i < ENTRIES_TO_ADD; i++)
    {
        int *item = malloc(sizeof(int));
        *item = i;
        lqueue_enqueue(queue, item);
    }

    return NULL;
}

void *dequeue_thread(void *user)
{
    LQueue *queue = (LQueue *)user;

    int i;
    for (i = 0; i < ENTRIES_TO_ADD; i++)
    {
        int *item = lqueue_dequeue(queue);
        if (*item != i)
            printf("Expected %d, but got %d\n", i, *item);
        free(item);
    }

    return NULL;
}

int main(int argc, char **argv)
{
    /* Create queue */
    LQueue *queue = lqueue_create();
    if (queue == NULL) {
        fprintf(stderr, "Error creating queue\n");
        goto error;
    }

    clock_t begin = clock();

    pthread_t et;
    if (pthread_create(&et, NULL, &enqueue_thread, queue) != 0)
        goto error;

    pthread_t dt;
    if (pthread_create(&dt, NULL, &dequeue_thread, queue) != 0)
        goto error;

    pthread_join(et, NULL);
    pthread_join(dt, NULL);

    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

    /* Free queue */
    lqueue_destroy(queue);

    printf("Pushed and popped %d items in %fs\n", ENTRIES_TO_ADD, time_spent);

    return 0;

error:
    if (queue != NULL)
        lqueue_destroy(queue);

    return 1;
}

