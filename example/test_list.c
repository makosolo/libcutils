#include "utils_list.h"

typedef struct {
    util_list_node_t    node;
    uint32_t            value;                
} list_node_data_t;

void test_list(void)
{
    util_list_t list;

    util_list_init(&list);

    list_node_data_t *data = (list_node_data_t*)malloc(sizeof(list_node_data_t));
    if (NULL != data) {
        data->value = 123;
        util_list_insert_head(&list, &data->node);
        if (util_list_empty(&data->node)) {
            printf("util_list_insert_head: is empty \n");
        }
        else {
            data = util_list_data(util_list_head(&data->node), list_node_data_t, node);
            printf("util_list_insert_head: data=%d \n", data->value);
        }

        
        free(data);
    }

    
}
