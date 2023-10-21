#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "container/utils_list.h"

typedef struct {
    util_list_node_t    node;
    uint32_t            value;                
} list_node_data_t;

void test_list(void)
{
    util_list_t list;

    util_list_init(&list);

    list_node_data_t *data = NULL;

    printf("++++++++++++++++++++++++util_list_insert_head++++++++++++++++++++++++\n");
    for (int i = 0; i < 10; i++)
    {
        list_node_data_t *data = (list_node_data_t*)malloc(sizeof(list_node_data_t));
        if (NULL != data) {
            data->value = i;
            util_list_insert_head(&list, &data->node);
            printf("util_list_insert_head: is_empty=%d \n", util_list_empty(&list));
        }
    }

    if (!util_list_empty(&list)) {
        util_list_node_t *pos, *tmp;
        util_list_for_each_next(pos, tmp, &list) {
            list_node_data_t *data = util_list_data(pos, list_node_data_t, node);

            printf("util_list_insert_head: data=%d \n", data->value);
        }

        util_list_node_t *node = util_list_head(&list);
        list_node_data_t *data = util_list_data(node, list_node_data_t, node);
        printf("util_list_next: data=%d \n", data->value);
        while (&list != node->next) {
            node = util_list_next(node);
            data = util_list_data(node, list_node_data_t, node);
            printf("util_list_next: data=%d \n", data->value);
        }

        node = util_list_tail(&list);
        data = util_list_data(node, list_node_data_t, node);
        printf("util_list_prev: data=%d \n", data->value);
        while (&list != node->prev) {
            node = util_list_prev(node);
            data = util_list_data(node, list_node_data_t, node);
            printf("util_list_prev: data=%d \n", data->value);
        }

        util_list_for_each_prev(pos, tmp, &list) {
            list_node_data_t *data = util_list_data(pos, list_node_data_t, node);

            util_list_remove(&data->node);
            printf("util_list_remove: data=%d is_empty=%d \n", data->value, util_list_empty(&list));
        }
    }
    printf("------------------------util_list_insert_head------------------------\n");

    printf("++++++++++++++++++++++++util_list_insert_tail++++++++++++++++++++++++\n");
    for (int i = 0; i < 10; i++)
    {
        list_node_data_t *data = (list_node_data_t*)malloc(sizeof(list_node_data_t));
        if (NULL != data) {
            data->value = i;
            util_list_insert_tail(&list, &data->node);
            printf("util_list_insert_tail: is_empty=%d \n", util_list_empty(&list));
        }
    }

    if (!util_list_empty(&list)) {
        util_list_node_t *pos, *tmp;
        util_list_for_each_next(pos, tmp, &list) {
            list_node_data_t *data = util_list_data(pos, list_node_data_t, node);

            printf("util_list_insert_tail: data=%d \n", data->value);
        }

        util_list_for_each_prev(pos, tmp, &list) {
            list_node_data_t *data = util_list_data(pos, list_node_data_t, node);

            util_list_remove(&data->node);
            printf("util_list_remove: data=%d is_empty=%d \n", data->value, util_list_empty(&list));
        }
    }
    printf("------------------------util_list_insert_tail------------------------\n");



}
