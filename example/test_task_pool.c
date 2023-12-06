#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include <signal.h>

#include "utils/utils_assert.h"
#include "platform/utils_task_pool.h"

typedef struct
{
    uint32_t		stop;
    uint32_t		stop_done;
} task_pool_context_t;

task_pool_context_t g_task_pool_ctx;

static void app_task(void *arg)
{
    printf("Thread #%u working on %d\n", (int)pthread_self(), (int) arg);
}

void test_task_pool(void)
{
    int status = 0;

    printf("++++++++++++++++++++++++task_pool++++++++++++++++++++++++\n");

    util_task_pool_t tpool;
	UTIL_ASSERT_RET(0 == util_task_pool_create(&tpool, 4), "util_task_pool_create fail");

	for (int i = 0; i < 4; i++){
		util_task_pool_add(&tpool, app_task, (void*)(uintptr_t)i);
	};

    printf("util_task_pool_num_working=%d \n", util_task_pool_num_working(&tpool));

    util_task_pool_wait(&tpool);
	util_task_pool_destroy(&tpool);
}
