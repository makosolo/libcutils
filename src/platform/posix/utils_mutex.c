#include <stdlib.h>
#include <stdio.h>

#include "utils_mutex.h"

int util_mutex_create(util_mutex_t *mutex)
{
    int status = 0;
    pthread_mutexattr_t mutex_attr;

    if (NULL == mutex) {
        return -1;
    }

    status |= pthread_mutexattr_init(&mutex_attr);
    status |= pthread_mutex_init(&mutex->lock, &mutex_attr);

    if(status!=0) {
        pthread_mutex_destroy(&mutex->lock);
        printf("Mutex initialization failed\n");
        status = -1;
    }
    pthread_mutexattr_destroy(&mutex_attr);

    return status;
}

int util_mutex_destroy(util_mutex_t *mutex)
{
    if(NULL == mutex) {
        return -1;
    }

    pthread_mutex_destroy(&mutex->lock);

    return 0;
}

int util_mutex_lock(util_mutex_t *mutex)
{
    int status = 0;

    if(NULL == mutex) {
        return -1;
    }

    status = pthread_mutex_lock(&mutex->lock);
    if(status != 0) {
        printf("Mutex lock failed\n");
    }

    return status;
}

int util_mutex_unlock(util_mutex_t *mutex)
{
    int status = 0;

    if(NULL == mutex) {
        return -1;
    }

    status = pthread_mutex_unlock(&mutex->lock);
    if(status != 0) {
        printf("Mutex unlock failed\n");
    }

    return status;
}
