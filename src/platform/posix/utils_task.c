#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#if _POSIX_C_SOURCE >= 199309L
#include <time.h>   /* for nanosleep */

int nanosleep(const struct timespec *req, struct timespec *rem);

#else
#include <unistd.h> /* for usleep */

extern int usleep (useconds_t __useconds);

#endif

#include "utils_task.h"

#define PRI_MAX  sched_get_priority_max(SCHED_FIFO)
#define PRI_MIN  sched_get_priority_min(SCHED_FIFO)

typedef struct task_context_s {
    pthread_t hndl;
} task_context_t;

static void *task_main(void *arg)
{
    util_task_t *task = (util_task_t*)arg;

    if( task && task->task_func) {
        task->task_func(task->app_var);
    }

    return NULL;
}

int util_task_create(util_task_t *task, const util_task_create_params_t *params)
{
    int status = 0;

    if (NULL == task || NULL == params) {
        return -1;
    }

    task_context_t* context = NULL;

    task->tsk_handle = NULL;

    context = malloc(sizeof(task_context_t));
    if(context == NULL) {
        printf("Context memory allocation failed\n");
    }
    else {
        pthread_attr_t thread_attr;

        task->stack_ptr     = params->stack_ptr;
        task->stack_size    = params->stack_size;
        task->core_affinity = params->core_affinity;
        task->priority      = params->priority;
        task->task_func     = params->task_main;
        task->app_var       = params->app_var;

        status = pthread_attr_init(&thread_attr);
        if(status == 0) {
            status = pthread_create(&context->hndl, &thread_attr, task_main, task);
            pthread_attr_destroy(&thread_attr);

#ifdef  _GNU_SOURCE
            if (0 != pthread_setname_np(context->hndl, params->task_name)) {
                printf("err: set thread name failed , errno is %d\n", errno);
            }
#else
            printf("pthread_setname_np is not supported\n");
#endif
        }

        if (status == 0) {
            task->tsk_handle = (void *)context;
        }
        else {
            status = -1;
            free(context);
            context = NULL;
        }
    }

    return status;
}

int util_task_destroy(util_task_t *task)
{
    if (NULL == task) {
        return -1;
    }

    if (task->tsk_handle) {
        task_context_t* context;
        void *ret_val;

        context = (task_context_t*)task->tsk_handle;
        task->tsk_handle = NULL;

        pthread_cancel(context->hndl);
        pthread_join(context->hndl, &ret_val);

        free(context);
    }

    return 0;
}

void util_task_wait_msecs(uint32_t msec)
{
#if _POSIX_C_SOURCE >= 199309L
    struct timespec delay_time = {0}, remain_time = {0};
    int ret;

    delay_time.tv_sec  = msec/1000U;
    delay_time.tv_nsec = (msec%1000U)*1000000U;

    do
    {
        ret = nanosleep(&delay_time, &remain_time);
        if(ret < 0 && remain_time.tv_sec > 0 && remain_time.tv_nsec > 0)
        {
            /* restart for remaining time */
            delay_time = remain_time;
        }
        else
        {
            break;
        }
    } while(1);
#else
    usleep(msec * 1000);
#endif
}
