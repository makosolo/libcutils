#include "utils_queue.h"

int test_data[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

void test_queue(void)
{
    util_queue_t *queue = NULL;

    queue = util_queue_creat(9);
    if (NULL == queue) {
        printf("util_queue_creat fail\n");
        return;
    }

    printf("++++++++++++++++++++++++util_queue_push++++++++++++++++++++++++\n");
    for (int i = 0; i < 10; i++)
    {
        int result = util_queue_push(queue, &test_data[i]);
        printf("util_queue_push: result=%d count=%ld\n", result, util_queue_count(queue));
    }

    printf("++++++++++++++++++++++++util_queue_peek++++++++++++++++++++++++\n");
    for (int i = 0; i < 10; i++)
    {
        int *data = (int*)util_queue_peek(queue);
        printf("util_queue_peek: data=%d count=%ld \n", *data, util_queue_count(queue));
    }

    printf("++++++++++++++++++++++++util_queue_pop++++++++++++++++++++++++\n");
    for (int i = 0; i < 10; i++)
    {
        int *data = (int*)util_queue_pop(queue);
        if (NULL != data) {
            printf("util_queue_pop: data=%d count=%ld \n", *data, util_queue_count(queue));
        }
        else {
            printf("util_queue_pop: data=%p \n", data);
        }
    }

    printf("++++++++++++++++++++++++util_queue_push&pop++++++++++++++++++++++++\n");
    for (int i = 0; i < 10; i++)
    {   
        int result = util_queue_push(queue, &test_data[i]);
        printf("util_queue_push: result=%d count=%ld\n", result, util_queue_count(queue));

        if (0 == i%2) {
            int *data = (int*)util_queue_pop(queue);
            printf("util_queue_pop: data=%d count=%ld \n", *data, util_queue_count(queue));
        }
    }

    printf("++++++++++++++++++++++++util_queue_clear++++++++++++++++++++++++\n");
    {
        util_queue_clear(queue);
        printf("util_queue_clear: count=%ld\n", util_queue_count(queue));
        
        int *data = (int*)util_queue_pop(queue);
        if (NULL != data) {
            printf("util_queue_pop: data=%d count=%ld \n", *data, util_queue_count(queue));
        }
        else {
            printf("util_queue_pop: data=%p \n", data);
        }        
    }

    util_queue_destroy(queue);
}
