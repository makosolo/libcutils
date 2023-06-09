#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/time.h>
#include <pthread.h>

#include "utils_event.h"
#include "utils_mutex.h"

#include "utils_queue.h"

struct util_queue_s {

  uint32_t cur_rd;
  /**< Current read index */

  uint32_t cur_wr;
  /**< Current write index  */

  uint32_t count;
  /**< Count of element in queue  */

  uint32_t max_ele;
  /**< Max elements that be present in the queue  */

  uintptr_t *queue;
  /**< Address of data area of the queue elements */

  util_event_t* block_rd;
  /**< Read semaphore */

  util_event_t* block_wr;
  /**< Write semaphore  */

  util_mutex_t* lock;
  /**< Queue lock semaphore  */

  void *context;
  /**< Private context of queue handle */

  uint32_t flags;
  /**< Controls how APIs behave internally, i.e blocking wait or non-blocking */

  volatile bool blockedOnGet;
  /**< Flag indicating queue is blocked on get operation */

  volatile bool blockedOnPut;
  /**< Flag indicating queue is blocked on put operation */
};

typedef struct util_queue_context_s {
    pthread_mutex_t lock;
    pthread_cond_t  condGet;
    pthread_cond_t  condPut;
} util_queue_context_t;

int util_queue_create(util_queue_t **queue, uint32_t max_elements, uint32_t flags)
{
    int status = 0;
    util_queue_context_t *context = NULL;
    util_queue_t *tmp_queue = NULL;

    if (NULL == queue || 0U == max_elements) {
        return -1;
    }

    tmp_queue = (util_queue_t*)malloc(sizeof(util_queue_t));
    if (NULL == tmp_queue) {
        *queue = NULL;
        printf("queue memory allocation failed\n");
        return -1;
    }

    /*
     * init queue to 0's
     */
    memset(tmp_queue, 0, sizeof(util_queue_t));

    /*
     * init queue with user parameters
     */
    tmp_queue->max_ele  = max_elements;
    tmp_queue->flags    = flags;
    tmp_queue->queue    = malloc(sizeof(uintptr_t) * max_elements);
    tmp_queue->context  = malloc(sizeof(util_queue_context_t));
    context = tmp_queue->context;

    if(NULL == tmp_queue->context || NULL == tmp_queue->queue) {
        printf("queue memory allocation failed\n");

        if (NULL != tmp_queue->queue)
            free(tmp_queue->queue);

        if (NULL != tmp_queue->context)
            free(tmp_queue->context);

        free(tmp_queue);

        status = -1;
    }
    else {
        pthread_mutexattr_t mutex_attr;

        status = 0;
        status |= pthread_mutexattr_init(&mutex_attr);
        status |= pthread_mutex_init(&context->lock, &mutex_attr);

        pthread_mutexattr_destroy(&mutex_attr);

        if(0 == status) {
            if (tmp_queue->flags & UTIL_QUEUE_FLAG_BLOCK_ON_GET) {
                pthread_condattr_t cond_attr;

                /*
                 * user requested block on que get
                 */

                /*
                 * create cond for it
                 */
                status |= pthread_condattr_init(&cond_attr);
                status |= pthread_cond_init(&context->condGet, &cond_attr);

                pthread_condattr_destroy(&cond_attr);
            }

            if (tmp_queue->flags & UTIL_QUEUE_FLAG_BLOCK_ON_PUT) {
                pthread_condattr_t cond_attr;

                /*
                 * user requested block on que put
                 */

                /*
                 * create cond for it
                 */
                status |= pthread_condattr_init(&cond_attr);
                status |= pthread_cond_init(&context->condPut, &cond_attr);

                pthread_condattr_destroy(&cond_attr);
            }
        }

        if (0 == status) {
            tmp_queue->blockedOnGet = false;
            tmp_queue->blockedOnPut = false;
            *queue = tmp_queue;
        }
        else {
            pthread_mutex_destroy(&context->lock);
            free(tmp_queue->context);
            tmp_queue->context = NULL;
        }
    }

    return status;
}

int util_queue_delete(util_queue_t **queue)
{
    int status = 0;
    util_queue_context_t *context;
    util_queue_t *tmp_queue;

    if (NULL == queue || NULL == *queue || NULL == (*queue)->context) {
        return -1;
    }

    context   = (*queue)->context;
    tmp_queue = *queue;

    (*queue)->context = NULL;
    *queue = NULL;

    printf("if this hangs, please ensure all application threads have been destroyed\n");
    if ((tmp_queue->flags & UTIL_QUEUE_FLAG_BLOCK_ON_GET)) {
        pthread_cond_destroy(&(context)->condGet);
    }

    if ((tmp_queue->flags & UTIL_QUEUE_FLAG_BLOCK_ON_PUT)) {
        pthread_cond_destroy(&(context)->condPut);
    }
    pthread_mutex_destroy(&context->lock);

    free(context);
    free(tmp_queue);

    return status;
}

int util_queue_put(util_queue_t *queue, uintptr_t data, uint32_t timeout)
{
    int status = 0;
    volatile bool do_break = false;
    util_queue_context_t *context = NULL;

    if(NULL == queue || NULL == queue->context || 0 == data) {
        return -1;
    }

    context = queue->context;

    status = pthread_mutex_lock(&context->lock);
    if (0 != status) {
        return -1;
    }

    do {
        if (queue->count < queue->max_ele) {
            /* insert element */
            queue->queue[queue->cur_wr] = data;

            /* increment put pointer */
            queue->cur_wr = (queue->cur_wr + 1U) % queue->max_ele;

            /* increment count of number element in que */
            queue->count++;

            /* mark status as success */
            status = 0;

            if (queue->flags & UTIL_QUEUE_FLAG_BLOCK_ON_GET) {
                /* blocking on que get enabled */

                /* post cond to unblock, blocked tasks */
                pthread_cond_signal(&context->condGet);
            }

            /* exit, with success */
            do_break = true;
        }
        else {
            int32_t retVal;

            /* que is full */
            if (timeout == UTIL_EVENT_TIMEOUT_NO_WAIT) {
                printf("queue is full\n");
                status   = -1;
                do_break = true; /* non-blocking, so exit with error */
            }
            else if (queue->flags & UTIL_QUEUE_FLAG_BLOCK_ON_PUT) {
                if (UTIL_QUEUE_TIMEOUT_WAIT_FOREVER == timeout) {
                    /* blocking on que put enabled */
                    queue->blockedOnPut = true;
                    retVal = pthread_cond_wait(&context->condPut, &context->lock);
                    queue->blockedOnPut = false;

                    if (retVal) {
                        status   = -1;
                        do_break = true;
                        printf("Event wait failed.\n");
                    }
                }
                else {
                    /* A valid and finite timeout has been specified. */
                    struct timespec ts;
                    struct timeval  tv;

                    retVal = gettimeofday(&tv, NULL);

                    if (retVal == 0) {
                        uint32_t        sec;
                        unsigned long   micro;

                        /* timeout is expected to be in milli-sec. */
                        micro = tv.tv_usec + (timeout * 1000);
                        sec   = tv.tv_sec;

                        if (micro >= 1000000LLU) {
                            sec   += micro/1000000LLU;
                            micro %= 1000000LLU;
                        }

                        ts.tv_nsec = micro * 1000;
                        ts.tv_sec  = sec;

                        queue->blockedOnPut = true;
                        retVal = pthread_cond_timedwait(&context->condPut,
                                                        &context->lock,
                                                        &ts);
                        queue->blockedOnPut = false;

                        if (retVal == ETIMEDOUT) {
                            printf("Event timed-out.\n");
                            status   = -1;
                            do_break = true;
                        }
                        else if (retVal) {
                            /* Error other than ETIMEDOUT. */
                            printf("Event wait failed.\n");
                            status   = -1;
                            do_break = true;
                        }
                    }
                    else {
                        /* gettimeofday() failed. */
                        printf("gettimeofday() failed.\n");
                        status   = -1;
                        do_break = true;
                    }
                }
            }
            else {
                /* blocking on que put disabled */

                /* exit with error */
                printf("blocking on queue is disabled\n");
                status   = -1;
                do_break = true;
            }
        }

        if (true == do_break) {
            break;
        }
    } while (1);

    status |= pthread_mutex_unlock(&context->lock);

    return status;
}

int util_queue_pop(util_queue_t *queue, uintptr_t *data, uint32_t timeout)
{
    int status = 0;
    volatile bool do_break = false;
    util_queue_context_t *context = NULL;

    if(NULL == queue || NULL == queue->context || NULL == data) {
        return -1;
    }

    context = queue->context;

    status = pthread_mutex_lock(&context->lock);
    if(0 != status) {
        return -1;
    }

    do {
        if (queue->count > 0U) {
            /* extract the element */
            *data = queue->queue[queue->cur_rd];

            /* increment get pointer */
            queue->cur_rd = (queue->cur_rd + 1U) % queue->max_ele;

            /* decrmeent number of elements in que */
            queue->count--;

            /* set status as success */
            status = 0;

            if (queue->flags & UTIL_QUEUE_FLAG_BLOCK_ON_PUT) {
                /* post cond to unblock, blocked tasks */
                pthread_cond_signal(&context->condPut);
            }

            /* exit with success */
            do_break = true;
        }
        else {
            int32_t retVal;

            /* no elements or not enough element in que to extract */
            if (timeout == UTIL_EVENT_TIMEOUT_NO_WAIT) {
                status   = -1;
                do_break = true; /* non-blocking, exit with error */
            }
            else if (queue->flags & UTIL_QUEUE_FLAG_BLOCK_ON_GET) {
                if (UTIL_QUEUE_TIMEOUT_WAIT_FOREVER == timeout) {
                    /* blocking on que get enabled */

                    queue->blockedOnGet = true;
                    retVal = pthread_cond_wait(&context->condGet, &context->lock);
                    queue->blockedOnGet = false;
                    /* received semaphore, check que again */

                    if (retVal) {
                        status   = -1;
                        do_break = true;
                        printf("Event wait failed.\n");
                    }
                }
                else {
                    /* A valid and finite timeout has been specified. */
                    struct timespec ts;
                    struct timeval  tv;

                    retVal = gettimeofday(&tv, NULL);

                    if (retVal == 0) {
                        uint32_t        sec;
                        unsigned long   micro;

                        /* timeout is expected to be in milli-sec. */
                        micro = tv.tv_usec + (timeout * 1000);
                        sec   = tv.tv_sec;

                        if (micro >= 1000000LLU) {
                            sec   += micro/1000000LLU;
                            micro %= 1000000LLU;
                        }

                        ts.tv_nsec = micro * 1000;
                        ts.tv_sec  = sec;

                        queue->blockedOnGet = true;
                        retVal = pthread_cond_timedwait(&context->condGet,
                                                        &context->lock,
                                                        &ts);
                        queue->blockedOnGet = false;

                        if (retVal == ETIMEDOUT) {
                            // printf("Event timed-out.\n");
                            status   = -1;
                            do_break = true;
                        }
                        else if (retVal) {
                            /* Error other than ETIMEDOUT. */
                            printf("Event wait failed.\n");
                            status   = -1;
                            do_break = true;
                        }
                    }
                    else {
                        /* gettimeofday() failed. */
                        printf("gettimeofday() failed.\n");
                        status   = -1;
                        do_break = true;
                    }
                }
            }
            else {
                /* blocking on que get disabled */

                /* exit with error */
                printf("blocking on que get disabled\n");
                status   = -1;
                do_break = true;
            }
        }

        if (true == do_break) {
            break;
        }
    } while (1);

    status |= pthread_mutex_unlock(&context->lock);

    return status;
}

bool util_queue_isempty(const util_queue_t *queue)
{
    bool is_empty = true;
    util_queue_context_t *context = NULL;
    int status;

    if(NULL == queue || NULL == queue->context) {
        return -1;
    }

    context = queue->context;

    status = pthread_mutex_lock(&context->lock);
    if(0 == status) {
        is_empty = (queue->count == 0U);
        pthread_mutex_unlock(&context->lock);
    }

    return is_empty;
}

int util_queue_peek(const util_queue_t *queue, uintptr_t *data)
{
    int status = 0;
    util_queue_context_t *context = NULL;

    if(NULL == queue || NULL == queue->context || NULL == data) {
        return -1;
    }

    context = queue->context;

    status = pthread_mutex_lock(&context->lock);
    if(0 == status) {
        if (queue->count > 0U) {
            /* 'peek' the element but dont extract it */
            *data = queue->queue[queue->cur_rd];

            /* set status as success */
            status = 0;
        }

        status = pthread_mutex_unlock(&context->lock);
    }

    return status;
}
