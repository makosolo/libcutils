#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include <signal.h>

#include "container/utils_array.h"
#include "utils/utils_assert.h"

void test_array(void)
{
    printf("++++++++++++++++++++++++array++++++++++++++++++++++++\n");
    {
        int value;
        struct util_array_int array;

        UTIL_ASSERT_RET(0 == util_array_create_int(&array), "util_map_init_str fail!");

        printf("0: add {100, 200, 300} \n");
        UTIL_ASSERT_RET(0 == util_array_add_int(&array, 100), "util_array_add_int fail!");
        UTIL_ASSERT_RET(0 == util_array_add_int(&array, 200), "util_array_add_int fail!");
        UTIL_ASSERT_RET(0 == util_array_add_int(&array, 300), "util_array_add_int fail!");

        printf("util_array_size=%ld \n", util_array_size(&array));
        util_array_foreach (&array, value) {
            printf("Value:[%d] \n", value);
        }

        printf("\n 1: del[1] \n");
        util_array_del_int(&array, 1);

        printf("util_array_size=%ld \n", util_array_size(&array));
        util_array_foreach (&array, value) {
            printf("Value:[%d] \n", value);
        }
        
        printf("\n 2: clear[1] \n");
        UTIL_ASSERT_RET(0 == util_array_clear_int(&array), "util_array_clear_int fail!");
        printf("util_array_size=%ld \n", util_array_size(&array));

        printf("\n 3: add[400, 500, 600, 700, 800] \n");
        UTIL_ASSERT_RET(0 == util_array_add_int(&array, 400), "util_array_add_int fail!");
        UTIL_ASSERT_RET(0 == util_array_add_int(&array, 500), "util_array_add_int fail!");
        UTIL_ASSERT_RET(0 == util_array_add_int(&array, 600), "util_array_add_int fail!");
        UTIL_ASSERT_RET(0 == util_array_add_int(&array, 700), "util_array_add_int fail!");
        UTIL_ASSERT_RET(0 == util_array_add_int(&array, 800), "util_array_add_int fail!");

        printf("util_array_size=%ld \n", util_array_size(&array));
        util_array_foreach (&array, value) {
            printf("Value:[%d] \n", value);
        }

        printf("\n 4: del_unordered[0] \n");
        UTIL_ASSERT_RET(0 == util_array_del_unordered_int(&array, 0), "util_array_del_unordered_int fail!");

        printf("util_array_size=%ld \n", util_array_size(&array));
        util_array_foreach (&array, value) {
            printf("Value:[%d] \n", value);
        }

        printf("\n 5: del_last[0] \n");

        printf("berfore: util_array_last=%d \n", util_array_last(&array));
        UTIL_ASSERT_RET(0 == util_array_del_last_int(&array), "util_array_del_last_int fail!");
        printf("after: util_array_last=%d \n", util_array_last(&array));

        printf("util_array_size=%ld \n", util_array_size(&array));
        for (int i = util_array_size(&array); i > 0; i--) {
            printf("util_array_at=%d \n", util_array_at(&array, i-1));
        }
        
        UTIL_ASSERT_RET(0 == util_array_destroy_int(&array), "util_array_destroy_int fail!");
    }

    {
        const char *value;
        struct util_array_str array;

        UTIL_ASSERT_RET(0 == util_array_create_str(&array), "util_array_create_str fail!");

        printf("\n 0: add {\"100\", \"200\", \"300\"} \n");
        UTIL_ASSERT_RET(0 == util_array_add_str(&array, "100"), "util_array_add_str fail!");
        UTIL_ASSERT_RET(0 == util_array_add_str(&array, "200"), "util_array_add_str fail!");
        UTIL_ASSERT_RET(0 == util_array_add_str(&array, "300"), "util_array_add_str fail!");

        printf("util_array_size=%ld \n", util_array_size(&array));
        util_array_foreach (&array, value) {
            printf("Value:[%s] \n", value);
        }

        printf("\n 1: del[1] \n");
        util_array_del_str(&array, 1);

        printf("util_array_size=%ld \n", util_array_size(&array));
        util_array_foreach (&array, value) {
            printf("Value:[%s] \n", value);
        }
        
        printf("\n 2: clear[1] \n");
        UTIL_ASSERT_RET(0 == util_array_clear_str(&array), "util_array_clear_str fail!");
        printf("util_array_size=%ld \n", util_array_size(&array));

        printf("\n 3: add[\"400\", \"500\", \"600\", \"700\", \"800\"] \n");
        UTIL_ASSERT_RET(0 == util_array_add_str(&array, "400"), "util_array_add_str fail!");
        UTIL_ASSERT_RET(0 == util_array_add_str(&array, "500"), "util_array_add_str fail!");
        UTIL_ASSERT_RET(0 == util_array_add_str(&array, "600"), "util_array_add_str fail!");
        UTIL_ASSERT_RET(0 == util_array_add_str(&array, "700"), "util_array_add_str fail!");
        UTIL_ASSERT_RET(0 == util_array_add_str(&array, "800"), "util_array_add_str fail!");

        printf("util_array_size=%ld \n", util_array_size(&array));
        util_array_foreach (&array, value) {
            printf("Value:[%s] \n", value);
        }

        printf("\n 4: del_unordered[0] \n");
        UTIL_ASSERT_RET(0 == util_array_del_unordered_str(&array, 0), "util_array_del_unordered_str fail!");

        printf("util_array_size=%ld \n", util_array_size(&array));
        util_array_foreach (&array, value) {
            printf("Value:[%s] \n", value);
        }

        printf("\n 5: del_last[0] \n");

        printf("berfore: util_array_last=%s \n", util_array_last(&array));
        UTIL_ASSERT_RET(0 == util_array_del_last_str(&array), "util_array_del_last_str fail!");
        printf("after: util_array_last=%s \n", util_array_last(&array));

        printf("util_array_size=%ld \n", util_array_size(&array));
        for (int i = util_array_size(&array); i > 0; i--) {
            printf("util_array_at=%s \n", util_array_at(&array, i-1));
        }
        
        UTIL_ASSERT_RET(0 == util_array_destroy_str(&array), "util_array_destroy_str fail!");
    }
}
