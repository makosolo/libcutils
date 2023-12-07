#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "utils_cycqueue.h"

int util_cycqueue_create(util_cycqueue_t *queue, uint32_t max_elements, uintptr_t *queue_memory)
{
    if (NULL == queue || 0U == max_elements) {
        return -1;
    }

    /*
     * init queue to 0's
     */
    memset(queue, 0, sizeof(util_cycqueue_t));

    /*
     * init queue with user parameters
     */
    queue->max_ele  = max_elements;
    if (NULL == queue_memory) {
        queue->flag  = 0;
        queue->queue = malloc(sizeof(uintptr_t) * max_elements);
    }
    else {
        queue->flag  = 1;
        queue->queue = queue_memory;
    }

    if(NULL == queue->queue) {
        return -2;
    }

    return 0;
}

int util_cycqueue_destroy(util_cycqueue_t *queue)
{
    uintptr_t *tmp_queue;

    if (NULL == queue) {
        return -1;
    }

    tmp_queue = queue->queue;
    queue->queue = NULL;

    if (0 == queue->flag) {
        free(tmp_queue);
    }

    return 0;
}

int util_cycqueue_put(util_cycqueue_t *queue, uintptr_t data)
{
    if(NULL == queue || 0 == data) {
        return -1;
    }

    if (queue->count >= queue->max_ele) {
        return -2;
    }

    /* insert element */
    queue->queue[queue->cur_wr] = data;

    /* increment put pointer */
    queue->cur_wr = (queue->cur_wr + 1U) % queue->max_ele;

    /* increment count of number element in que */
    queue->count++;

    return 0;
}

int util_cycqueue_pop(util_cycqueue_t *queue, uintptr_t *data)
{
    if(NULL == queue || NULL == data) {
        return -1;
    }

    if (queue->count == 0U) {
        return -2;
    }

    /* extract the element */
    *data = queue->queue[queue->cur_rd];

    /* increment get pointer */
    queue->cur_rd = (queue->cur_rd + 1U) % queue->max_ele;

    /* decrmeent number of elements in que */
    queue->count--;

    return 0;
}

bool util_cycqueue_isempty(const util_cycqueue_t *queue)
{
    if(NULL == queue) {
        return true;
    }

    return (queue->count == 0U);
}

int util_cycqueue_peek(const util_cycqueue_t *queue, uintptr_t *data)
{
    if(NULL == queue || NULL == data) {
        return -1;
    }

    if (0 == queue->count) {
        return -2;
    }

    /* 'peek' the element but dont extract it */
    *data = queue->queue[queue->cur_rd];

    return 0;
}
