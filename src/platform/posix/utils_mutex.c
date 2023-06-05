#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#include "utils_mutex.h"

struct tivx_mutex_s {
  pthread_mutex_t lock;
};

int util_mutex_create(util_mutex_t **mutex)
{
    int status = 0;
    pthread_mutexattr_t mutex_attr;
    util_mutex_t *tmp_mutex;

    if (NULL == mutex) {
        return -1;
    }

    tmp_mutex = (util_mutex_t*)malloc(sizeof(util_mutex_t));
    if(tmp_mutex==NULL) {
        *mutex = NULL;
        printf("Mutex memory allocation failed\n");
        status = -1;
    }
    else {
        status |= pthread_mutexattr_init(&mutex_attr);
        status |= pthread_mutex_init(&tmp_mutex->lock, &mutex_attr);

        if(status!=0) {
            pthread_mutex_destroy(&tmp_mutex->lock);
            free(tmp_mutex);
            *mutex = NULL;
            printf("Mutex initialization failed\n");
            status = -1;
        }
        else {
            *mutex = tmp_mutex;
        }
        pthread_mutexattr_destroy(&mutex_attr);
    }

    return status;
}

int util_mutex_delete(util_mutex_t **mutex)
{
    if(NULL == mutex || NULL == *mutex) {
        return -1;
    }

    util_mutex_t *temp_mutex = *mutex;
    *mutex = NULL;

    pthread_mutex_destroy(&(temp_mutex)->lock);
    free(temp_mutex);

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
