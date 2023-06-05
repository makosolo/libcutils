#include "utils_rbtree.h"

void test_rbtree(void)
{
    int8_t buf[10] = {2, 4, 6, 8, 10, 12, 14, 16, 18, 20};
    char *str[] = {"a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "qrstuvwxyz"};

    util_rbtree_t *tree = NULL;

    if (0 == util_rbtree_create(&tree, UTIL_RBTREE_KEY_TYPE_I8)) {
        return;
    }

    printf("++++++++++++++++++++++++test_rbtree++++++++++++++++++++++++\n");
    {
        printf("+++util_rbtree_insert: is_empty=%d\n", util_rbtree_empty(tree));
        for (int i = 0; i < 10; i++) {
            int result = 0;
            result = util_rbtree_insert(tree, util_rbtree_key_i8(i), &buf[i]);
            printf("util_rbtree_insert: %d is_empty=%d\n", result, util_rbtree_empty(tree));
        }

        for (int i = 10; i >= 0; i--) {
            util_rbtree_node_t *node = util_rbtree_find(tree, util_rbtree_key_i8(i));
            if (NULL == node) {
                printf("util_rbtree_find[%d] fail\n", i);
            }
            else {
                printf("util_rbtree_find[%d]=%d\n", i, *((int8_t*)util_rbtree_data(node)));
            }
        }

        util_rbtree_node_t *node = util_rbtree_first(tree);
        while (NULL != node) {
            int8_t key = 0;
            if (0 == util_rbtree_get_key(node, UTIL_RBTREE_KEY_TYPE_I8, &key, sizeof(key))) {
                printf("util_rbtree_next[%d]=%d\n", key, *((int8_t*)util_rbtree_data(node)));
            }
            else {
                printf("util_rbtree_get_key fail\n");
            }

            node = util_rbtree_next(node);
        }

        node = util_rbtree_last(tree);
        while (NULL != node) {
            int8_t key = 0;
            if (0 == util_rbtree_get_key(node, UTIL_RBTREE_KEY_TYPE_I8, &key, sizeof(key))) {
                printf("util_rbtree_prev[%d]=%d\n", key, *((int8_t*)util_rbtree_data(node)));
            }
            else {
                printf("util_rbtree_get_key fail\n");
            }

            node = util_rbtree_prev(node);
        }

        node = util_rbtree_first(tree);
        while (NULL != node) {
            int8_t key = 0;
            if (0 == util_rbtree_get_key(node, UTIL_RBTREE_KEY_TYPE_I8, &key, sizeof(key))) {
                printf("util_rbtree_data[%d]=%d\n", key, *((int8_t*)util_rbtree_data(node)));
            }
            else {
                printf("util_rbtree_get_key fail\n");
            }
            util_rbtree_erase(tree, node);
            node = util_rbtree_first(tree);
        }

        printf("---util_rbtree_insert: is_empty=%d\n", util_rbtree_empty(tree));
    }

    {
        printf("+++util_rbtree_insert: is_empty=%d\n", util_rbtree_empty(tree));
        for (int i = 0; i < 10; i++) {
            int result = 0;
            result = util_rbtree_insert(tree, util_rbtree_key_str(str[i]), &buf[i]);
            printf("util_rbtree_insert: %d is_empty=%d\n", result, util_rbtree_empty(tree));
        }

        for (int i = 10; i >= 0; i--) {
            util_rbtree_node_t *node = util_rbtree_find(tree, util_rbtree_key_str(str[i]));
            if (NULL == node) {
                printf("util_rbtree_find[%d] fail\n", i);
            }
            else {
                printf("util_rbtree_find[%s]=%d\n", str[i], *((int8_t*)util_rbtree_data(node)));
            }
        }

        util_rbtree_node_t *node = util_rbtree_first(tree);
        while (NULL != node) {
            char key[UTILS_RBTREE_KEY_STR_MAX] = {0};
            if (0 == util_rbtree_get_key(node, UTIL_RBTREE_KEY_TYPE_STR, key, sizeof(key))) {
                printf("util_rbtree_next[%s]=%d\n", key, *((int8_t*)util_rbtree_data(node)));
            }
            else {
                printf("util_rbtree_get_key fail\n");
            }

            node = util_rbtree_next(node);
        }

        node = util_rbtree_last(tree);
        while (NULL != node) {
            char key[UTILS_RBTREE_KEY_STR_MAX] = {0};
            if (0 == util_rbtree_get_key(node, UTIL_RBTREE_KEY_TYPE_STR, key, sizeof(key))) {
                printf("util_rbtree_prev[%s]=%d\n", key, *((int8_t*)util_rbtree_data(node)));
            }
            else {
                printf("util_rbtree_get_key fail\n");
            }

            node = util_rbtree_prev(node);
        }

        node = util_rbtree_first(tree);
        while (NULL != node) {
            char key[UTILS_RBTREE_KEY_STR_MAX] = {0};
            if (0 == util_rbtree_get_key(node, UTIL_RBTREE_KEY_TYPE_STR, key, sizeof(key))) {
                printf("util_rbtree_data[%s]=%d\n", key, *((int8_t*)util_rbtree_data(node)));
            }
            else {
                printf("util_rbtree_get_key fail\n");
            }
            util_rbtree_erase(tree, node);
            node = util_rbtree_first(tree);
        }

        printf("---util_rbtree_insert: is_empty=%d\n", util_rbtree_empty(tree));
    }


    util_rbtree_delete(&tree);
}
