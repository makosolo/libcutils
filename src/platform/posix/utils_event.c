#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <sys/time.h>
#include <pthread.h>

#include "utils_event.h"

struct util_event_s {
    uint16_t        is_set;
    pthread_mutex_t lock;
    pthread_cond_t  cond;
};

int util_event_create(util_event_t **event)
{
    pthread_mutexattr_t mutex_attr;
    pthread_condattr_t cond_attr;
    util_event_t *tmp_event = NULL;
    int status = 0;

    if (NULL == event) {
        return -1;
    }

    tmp_event = (util_event_t*)malloc(sizeof(util_event_t));
    if(NULL == tmp_event) {
        *event = NULL;
        status = -1;
        printf("Memory allocation failed\n");
    }
    else {
        status |= pthread_mutexattr_init(&mutex_attr);
        status |= pthread_condattr_init(&cond_attr);

        status |= pthread_mutex_init(&tmp_event->lock, &mutex_attr);
        status |= pthread_cond_init(&tmp_event->cond, &cond_attr);

        tmp_event->is_set = 0;

        if(status!=0) {
            pthread_cond_destroy(&tmp_event->cond);
            pthread_mutex_destroy(&tmp_event->lock);
            free(tmp_event);
            *event = NULL;
            printf("Mutex initialization failed\n");
            status = -2;
        }
        else {
            *event = tmp_event;
        }

        pthread_condattr_destroy(&cond_attr);
        pthread_mutexattr_destroy(&mutex_attr);
    }

    return status;
}

int util_event_delete(util_event_t **event)
{
    if(NULL == event || NULL == *event) {
        return -1;
    }

    util_event_t *temp_event = *event;
    *event = NULL;

    pthread_cond_destroy(&(*event)->cond);
    pthread_mutex_destroy(&(*event)->lock);
    free(temp_event);

    return 0;
}

int util_event_post(util_event_t *event)
{
    int status = 0;

    if(NULL == event) {
        return -1;
    }

    status = pthread_mutex_lock(&event->lock);
    if(status == 0) {
        event->is_set = 1;

        status |= pthread_cond_signal(&event->cond);
        status |= pthread_mutex_unlock(&event->lock);
    }
    else {
        printf("Mutex post failed\n");
    }

    return status;
}

int util_event_wait(util_event_t *event, uint32_t timeout)
{
    int32_t status1;
    int32_t retVal;
    int     status = 0;

    if(NULL == event) {
        return -1;
    }

    status = pthread_mutex_lock(&event->lock);
    if(status == 0) {
        bool done = false;

        while(!done)
        {
            if(event->is_set==1U) {
                /* clear event */
                event->is_set = 0;
                status        = 0;
                done          = true;
            }
            else if(UTIL_EVENT_TIMEOUT_NO_WAIT == timeout) {
                printf("Timeout set to TIVX_EVENT_TIMEOUT_NO_WAIT\n");
                status = -1;
                done   = true;
            }
            else if(UTIL_EVENT_TIMEOUT_WAIT_FOREVER != timeout) {
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

                    retVal = pthread_cond_timedwait(&event->cond,
                                                    &event->lock,
                                                    &ts);

                    if (retVal == ETIMEDOUT) {
                        printf("Event timed-out.\n");
                        status = -1;
                        done   = true;
                    }
                    else if (retVal) {
                        /* Error other than ETIMEDOUT. */
                        printf("Event wait failed.\n");
                        status = -1;
                        done   = true;
                    }
                }
                else {
                    /* gettimeofday() failed. */
                    printf("gettimeofday() failed.\n");
                    status = -1;
                    done   = true;
                }
            }
            else {
                /* timeout == TIVX_EVENT_TIMEOUT_WAIT_FOREVER */
                retVal = pthread_cond_wait(&event->cond, &event->lock);

                if (retVal) {
                    printf("Event wait failed.\n");
                    status = -1;
                    done   = true;
                }
            }
        }

        status1 = pthread_mutex_unlock(&event->lock);
        if(status1 != 0) {
            printf("Mutex unlock failed\n");
            status = -1;
        }
    }

    return status;
}

int util_event_clear(util_event_t *event)
{
    int status = 0;

    if(NULL == event) {
        return -1;
    }

    status = pthread_mutex_lock(&event->lock);
    if(status == 0) {
        event->is_set = 0;
        status = pthread_mutex_unlock(&event->lock);
    }
    else {
        printf("Mutex lock failed\n");
        status = -1;
    }

    return status;
}
