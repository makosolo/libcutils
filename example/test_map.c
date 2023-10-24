#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include <signal.h>

#include "container/utils_map.h"
#include "utils/utils_assert.h"

void test_map(void)
{
    printf("++++++++++++++++++++++++map++++++++++++++++++++++++\n");
    {
        const char *key, *value;
        struct util_map_str map;

        UTIL_ASSERT_RET(util_map_init_str(&map, 0, 0), "util_map_init_str fail!");

        UTIL_ASSERT_RET(0 == util_map_put_str(&map, "jack", "chicago"), "util_map_put_str fail!");
        UTIL_ASSERT_RET(0 == util_map_put_str(&map, "jane", "new york"), "util_map_put_str fail!");
        UTIL_ASSERT_RET(0 == util_map_put_str(&map, "janie", "atlanta"), "util_map_put_str fail!");

        util_map_foreach (&map, key, value) {
            printf("Key:[%s], Value:[%s] \n", key, value);
        }

        util_map_destroy_str(&map);
    }

    {
        uint32_t key;
        const char *value;
        struct util_map_64s map;

        UTIL_ASSERT_RET(util_map_init_64s(&map, 0, 0), "util_map_init_str fail!");

        UTIL_ASSERT_RET(0 == util_map_put_64s(&map, 100, "chicago"), "util_map_put_64s fail!");
        UTIL_ASSERT_RET(0 == util_map_put_64s(&map, 200, "new york"), "util_map_put_64s fail!");
        UTIL_ASSERT_RET(0 == util_map_put_64s(&map, 300, "atlanta"), "util_map_put_64s fail!");

        util_map_foreach_key(&map, key) {
            printf("Key:[%d] \n", key);
        }

        util_map_foreach_value(&map, value) {
            printf("Value:[%s] \n", value);
        }

        printf("util_map_size_64s=%d \n", util_map_size_64s(&map));
        value = util_map_get_64s(&map, 200);
        if (util_map_found(&map)) {
            printf("Found Value:[%s] \n", value);
        }

        value = util_map_del_64s(&map, 100);
        if (util_map_found(&map)) {
            printf("Deleted : %s \n", value);
        }

        printf("util_map_size_64s=%d \n", util_map_size_64s(&map));
        util_map_foreach (&map, key, value) {
            printf("Key:[%d], Value:[%s] \n", key, value);
        }

        value = util_map_del_64s(&map, 200);
        if (util_map_found(&map)) {
            printf("Deleted : %s \n", value);
        }

        printf("util_map_size_64s=%d \n", util_map_size_64s(&map));
        util_map_foreach (&map, key, value) {
            printf("Key:[%d], Value:[%s] \n", key, value);
        }

        util_map_clear_64s(&map);
        if (util_map_found(&map)) {
            printf("Clear : %s \n", value);
        }
        printf("util_map_size_64s=%d \n", util_map_size_64s(&map));

        value = util_map_put_64s(&map, 300, "los angeles");
        if (util_map_found(&map)) {
            printf("overridden : %s \n", value);
        }

        printf("util_map_size_64s=%d \n", util_map_size_64s(&map));
        util_map_foreach (&map, key, value) {
            printf("Key:[%d], Value:[%s] \n", key, value);
        }

        util_map_destroy_64s(&map);
    }
}
