/* Simple example of pushing to and popping from queue */

#include <stdio.h>

#include <lqueue.h>

int main(int argc, char **argv)
{
    /* Create queue */
    LQueue *queue = lqueue_create();
    if (queue == NULL) {
        fprintf(stderr, "Error creating queue\n");
        goto error;
    }

    int data = 42;

    /* Push data onto queue */
    int rv = lqueue_enqueue(queue, &data);
    if (rv != 0) {
        fprintf(stderr, "Error when pushing to queue\n");
        goto error;
    }

    printf("Pushed '%d' onto queue\n", data);

    /* Pop data from queue */
    int *data_ptr = (int *)lqueue_dequeue(queue);
    if (data_ptr == NULL) {
        fprintf(stderr, "Error reading from queue\n");
        goto error;
    }

    printf("Popped '%d' from queue\n", *data_ptr);

    /* Free queue */
    lqueue_destroy(queue);

    return 0;

error:
    if (queue != NULL)
        lqueue_destroy(queue);

    return 1;
}

