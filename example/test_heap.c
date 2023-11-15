#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include <signal.h>

#include "container/utils_heap.h"
#include "utils/utils_assert.h"

void test_heap(void)
{
    printf("++++++++++++++++++++++++heap++++++++++++++++++++++++\n");
    {
        static int64_t arr[] = {1, 0, 4, 5, 7, 9, 8, 6, 3, 2};
        util_heap_data_t *elem;
        util_heap_t heap;
        int result = 0;

        result = util_heap_create(&heap, 0);
        UTIL_ASSERT_RET(result == 0, "util_heap_create fail, result=%d", result);

        for (int i = 0; i < 10; i++) {
            result = util_heap_push(&heap, arr[i], &arr[i]);
            UTIL_ASSERT_RET(result == 0, "util_heap_push fail, result=%d", result);
        }

        for (int i = 0; i < 10; i++) {
            elem = util_heap_peek(&heap);
            UTIL_ASSERT_RET(elem != NULL, "elem == NULL");
            printf("key=%ld data=%d \n", elem->key, *((int*)elem->data));
        }
        printf("util_heap_size=%ld \n", util_heap_size(&heap));

        // UTIL_ASSERT_RET(0 == util_heap_clear(&heap), "util_heap_clear fail");

        for (int i = 0; i < 10; i++) {
            elem = util_heap_pop(&heap);
            UTIL_ASSERT_RET(elem != NULL, "elem == NULL");
            printf("key=%ld data=%d \n", elem->key, *((int*)elem->data));
        }
        printf("util_heap_size=%ld \n", util_heap_size(&heap));

        util_heap_destroy(&heap);
    }
}
