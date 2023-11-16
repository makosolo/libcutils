#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include <signal.h>

#include "platform/utils_task.h"
#include "platform/utils_mutex.h"
#include "container/utils_cycqueue.h"

typedef struct
{
    util_task_t     task;
    uint32_t		stop;
    uint32_t		stop_done;
} task_obj_t;

typedef struct
{
    task_obj_t task_1;
    task_obj_t task_2;
    int        exit;
    util_cycqueue_t queue;
    util_mutex_t mutex;
} queue_context_t;

static queue_context_t g_queue_ctx;

static uint32_t g_data[4] = {1, 2, 3, 4};

static void app_task_1(void *app_var)
{
    queue_context_t *pObj = (queue_context_t *)app_var;

    printf("app_task_1... \n");

    uint32_t *data = NULL;

    while(!pObj->task_1.stop)
    {
        util_mutex_lock(&pObj->mutex);
        if (util_cycqueue_isempty(&pObj->queue)) {
            util_mutex_unlock(&pObj->mutex);
            printf("util_cycqueue_isempty \n");
        }
        else if (0 == util_cycqueue_pop(&pObj->queue, (uintptr_t*)&data)) {
            util_mutex_unlock(&pObj->mutex);
            printf("app_task_1[%ld]: data=%d \n", time(0), *data);
        }
        else {
            util_mutex_unlock(&pObj->mutex);
            printf("util_cycqueue_pop fail \n");
        }

        util_task_wait_msecs(500);
    }

    printf("app_task_1 leave... \n");
    pObj->task_1.stop_done = 1;
}

static void app_task_2(void *app_var)
{
    queue_context_t *pObj = (queue_context_t *)app_var;
    int index = 0;
    int status = 0;

    printf("app_task_2... \n");

    uint32_t *data = NULL;

    while(!pObj->task_2.stop)
    {
        util_mutex_lock(&pObj->mutex);
        status = util_cycqueue_put(&pObj->queue, (uintptr_t)&g_data[index%4]);
        util_mutex_unlock(&pObj->mutex);

        if (0 != status) {
            printf("util_cycqueue_put fail, status=%d \n", status);
        }
        else {
            util_mutex_lock(&pObj->mutex);
            status = util_cycqueue_peek(&pObj->queue, (uintptr_t*)&data);
            util_mutex_unlock(&pObj->mutex);

            if (0 == status) {
                printf("app_task_2[%ld]: data=%d \n", time(0), *data);
            }
            else {
                printf("util_cycqueue_peek fail, status=%d \n", status);
            }
            index++;
        }

        util_task_wait_msecs(1000);
    }

    printf("app_task_2 leave... \n");
    pObj->task_2.stop_done = 1;
}

static void app_int_sig_handler(int sig)
{
    g_queue_ctx.exit = 1;
    g_queue_ctx.task_1.stop = 1;
    g_queue_ctx.task_2.stop = 1;

    printf("app_int_sig_handler exit\n");
    // exit(0);
}

void test_cycqueue(void)
{
    int status = 0;

    printf("++++++++++++++++++++++++task++++++++++++++++++++++++\n");

    signal(SIGINT, app_int_sig_handler);

    g_queue_ctx.exit = 0;

    /* create queue */
    status = util_cycqueue_create(&g_queue_ctx.queue, 4);
    if (0 != status) {
        return;
    }

    /* create task1 */
    util_task_create_params_t params;
    memset(&params, 0, sizeof(util_task_create_params_t));

    params.core_affinity = UTIL_TASK_AFFINITY_ANY;
    params.priority      = UTIL_TASK_PRI_LOWEST;
    params.task_main     = app_task_1;
    params.app_var       = &g_queue_ctx;
    strncpy(params.task_name, "MutexTask1", UTIL_MAX_TASK_NAME);
    params.task_name[UTIL_MAX_TASK_NAME-1U] = (char)0;

    status = util_task_create(&g_queue_ctx.task_1.task, &params);
    if (0 != status) {
        return;
    }

    g_queue_ctx.task_1.stop_done = 0;
    g_queue_ctx.task_1.stop      = 0;

    /* create task2 */
    params.core_affinity = UTIL_TASK_AFFINITY_ANY;
    params.priority      = UTIL_TASK_PRI_LOWEST;
    params.task_main     = app_task_2;
    params.app_var       = &g_queue_ctx;
    strncpy(params.task_name, "MutexTask2", UTIL_MAX_TASK_NAME);
    params.task_name[UTIL_MAX_TASK_NAME-1U] = (char)0;

    status = util_task_create(&g_queue_ctx.task_2.task, &params);
    if (0 != status) {
        return;
    }

    g_queue_ctx.task_2.stop_done = 0;
    g_queue_ctx.task_2.stop      = 0;

    status = util_mutex_create(&g_queue_ctx.mutex);
    if (0 != status) {
        return;
    }

    while (!g_queue_ctx.exit) {
        util_task_wait_msecs(100);
    }

    /* delete task */
    while(g_queue_ctx.task_1.stop_done == 0) {
        util_task_wait_msecs(1000);
    }
    util_task_destroy(&g_queue_ctx.task_1.task);

    while(g_queue_ctx.task_2.stop_done == 0) {
        util_task_wait_msecs(1000);
    }
    util_task_destroy(&g_queue_ctx.task_2.task);

    util_cycqueue_destroy(&g_queue_ctx.queue);

    util_mutex_destroy(&g_queue_ctx.mutex);
}
