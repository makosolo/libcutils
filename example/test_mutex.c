#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include <signal.h>

#include "platform/include/utils_task.h"
#include "platform/include/utils_mutex.h"

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
    uint32_t   count;
    int        exit;
    util_mutex_t *mutex;
} mutex_context_t;

mutex_context_t g_mutex_ctx;

static void app_task_1(void *app_var)
{
    mutex_context_t *pObj = (mutex_context_t *)app_var;

    printf("app_task_1... \n");

    while(!pObj->task_1.stop)
    {
        util_mutex_lock(pObj->mutex);
        printf("app_task_1[%ld]: count=%d \n", time(0), pObj->count);
        util_mutex_unlock(pObj->mutex);

        util_task_wait_msecs(100);
    }

    printf("app_task_1 leave... \n");
    pObj->task_1.stop_done = 1;
}

static void app_task_2(void *app_var)
{
    mutex_context_t *pObj = (mutex_context_t *)app_var;

    printf("app_task_2... \n");

    while(!pObj->task_2.stop)
    {
        util_mutex_lock(pObj->mutex);
        printf("app_task_2[%ld]: count=%d \n", time(0), ++pObj->count);
        util_task_wait_msecs(1000);
        printf("app_task_2[%ld]: count=%d \n", time(0), ++pObj->count);
        util_task_wait_msecs(1000);
        printf("app_task_2[%ld]: count=%d \n", time(0), ++pObj->count);
        util_task_wait_msecs(1000);
        printf("app_task_2[%ld]: count=%d \n", time(0), ++pObj->count);
        util_mutex_unlock(pObj->mutex);

        util_task_wait_msecs(1000);
    }

    printf("app_task_2 leave... \n");
    pObj->task_2.stop_done = 1;
}

static void app_int_sig_handler(int sig)
{
    g_mutex_ctx.exit = 1;
    g_mutex_ctx.task_1.stop = 1;
    g_mutex_ctx.task_2.stop = 1;

    printf("app_int_sig_handler exit\n");
    // exit(0);
}

void test_mutex(void)
{
    int status = 0;

    printf("++++++++++++++++++++++++task++++++++++++++++++++++++\n");

    signal(SIGINT, app_int_sig_handler);

    g_mutex_ctx.exit = 0;

    /* create mutex */
    status = util_mutex_create(&g_mutex_ctx.mutex);
    if (0 != status) {
        return;
    }

    /* create task1 */
    util_task_create_params_t params;
    memset(&params, 0, sizeof(util_task_create_params_t));

    params.core_affinity = UTIL_TASK_AFFINITY_ANY;
    params.priority      = UTIL_TASK_PRI_LOWEST;
    params.task_main     = app_task_1;
    params.app_var       = &g_mutex_ctx;
    strncpy(params.task_name, "MutexTask1", UTIL_MAX_TASK_NAME);
    params.task_name[UTIL_MAX_TASK_NAME-1U] = (char)0;

    status = util_task_create(&g_mutex_ctx.task_1.task, &params);
    if (0 != status) {
        return;
    }

    g_mutex_ctx.task_1.stop_done = 0;
    g_mutex_ctx.task_1.stop      = 0;

    /* create task2 */
    params.core_affinity = UTIL_TASK_AFFINITY_ANY;
    params.priority      = UTIL_TASK_PRI_LOWEST;
    params.task_main     = app_task_2;
    params.app_var       = &g_mutex_ctx;
    strncpy(params.task_name, "MutexTask2", UTIL_MAX_TASK_NAME);
    params.task_name[UTIL_MAX_TASK_NAME-1U] = (char)0;

    status = util_task_create(&g_mutex_ctx.task_2.task, &params);
    if (0 != status) {
        return;
    }
    g_mutex_ctx.task_2.stop_done = 0;
    g_mutex_ctx.task_2.stop      = 0;

    while (!g_mutex_ctx.exit) {
        util_task_wait_msecs(100);
    }

    /* delete task */
    while(g_mutex_ctx.task_1.stop_done == 0) {
        util_task_wait_msecs(1000);
    }
    util_task_delete(&g_mutex_ctx.task_1.task);

    while(g_mutex_ctx.task_2.stop_done == 0) {
        util_task_wait_msecs(1000);
    }
    util_task_delete(&g_mutex_ctx.task_2.task);

    util_mutex_delete(&g_mutex_ctx.mutex);
}
