#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include <signal.h>

#include "platform/include/utils_task.h"
#include "platform/include/utils_queue.h"

typedef struct
{
    util_task_t*    task;
    uint32_t		stop;
    uint32_t		stop_done;
} task_obj_t;

typedef struct
{
    task_obj_t task_1;
    task_obj_t task_2;
    int        exit;
    util_queue_t *queue;
} queue_context_t;

queue_context_t g_queue_ctx;

static uint32_t g_data[4] = {1, 2, 3, 4};

static void app_task_1(void *app_var)
{
    queue_context_t *pObj = (queue_context_t *)app_var;

    printf("app_task_1... \n");

    uint32_t *data = NULL;

    while(!pObj->task_1.stop)
    {
        if (util_queue_isempty(pObj->queue)) {
            printf("util_queue_isempty \n");
        }
        else if (0 == util_queue_pop(pObj->queue, (uintptr_t*)&data, 100)) {
            printf("app_task_1[%ld]: data=%d \n", time(0), *data);
            continue;
        }
        else {
            printf("util_queue_pop fail \n");
        }

        util_task_wait_msecs(1000);
    }

    printf("app_task_1 leave... \n");
    pObj->task_1.stop_done = 1;
}

static void app_task_2(void *app_var)
{
    queue_context_t *pObj = (queue_context_t *)app_var;
    int index = 0; 

    printf("app_task_2... \n");

    uint32_t *data = NULL;

    while(!pObj->task_2.stop)
    {
        if (0 != util_queue_put(pObj->queue, (uintptr_t)&g_data[index%4], 1000)) {
            printf("util_queue_put fail\n");
            continue;
        }
        else if (0 == util_queue_peek(pObj->queue, (uintptr_t*)&data)) {
            printf("app_task_2[%ld]: data=%d \n", time(0), *data);
        }
        else {
            printf("util_queue_peek fail, isempty=%d \n", util_queue_isempty(pObj->queue));
        }
        index++;

        util_task_wait_msecs(100);
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

void test_queue(void)
{
    int status = 0;

    printf("++++++++++++++++++++++++task++++++++++++++++++++++++\n");

    signal(SIGINT, app_int_sig_handler);

    g_queue_ctx.exit = 0;

    /* create queue */
    status = util_queue_create(&g_queue_ctx.queue, 4, UTIL_QUEUE_FLAG_BLOCK_ON_PUT|UTIL_QUEUE_FLAG_BLOCK_ON_GET);
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

    while (!g_queue_ctx.exit) {
        util_task_wait_msecs(100);
    }

    /* delete task */
    while(g_queue_ctx.task_1.stop_done == 0) {
        util_task_wait_msecs(1000);
    }
    util_task_delete(&g_queue_ctx.task_1.task);

    while(g_queue_ctx.task_2.stop_done == 0) {
        util_task_wait_msecs(1000);
    }
    util_task_delete(&g_queue_ctx.task_2.task);

    util_queue_delete(&g_queue_ctx.queue);
}
