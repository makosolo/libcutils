#include <stdlib.h>

#include "utils_queue.h"

struct util_queue_s {
    util_list_t     list;
    size_t          max_count;
    size_t          push_count;
    size_t          pop_count;
};

typedef struct {
    util_list_node_t    node;
    void               *data;                
} util_queue_node_t;

util_queue_t *util_queue_creat(size_t max_count)
{
    util_queue_t *queue = (util_queue_t*)malloc(sizeof(util_queue_t));
    if (NULL != queue) {
        queue->max_count  = max_count;
        queue->push_count = 0;
        queue->pop_count  = 0;  
        util_list_init(&queue->list);
    }

    return queue;
}

void util_queue_destroy(util_queue_t* queue)
{
    util_queue_t *temp = queue;
    queue = NULL;

    if (NULL != temp) {
        free(temp);
    }
}

int util_queue_push(util_queue_t* queue, void* data)
{
    util_queue_node_t *q_node = NULL;
    
    if (NULL == queue || NULL == data || queue->push_count - queue->pop_count >= queue->max_count) {
        return -1;
    }

    q_node = (util_queue_node_t*)malloc(sizeof(util_queue_node_t));
    if (NULL != q_node) {
        queue->push_count++;
        q_node->data = data;
        util_list_insert_tail(&queue->list, &q_node->node);
        return 0;
    }
    
    return -1;
}

void* util_queue_pop(util_queue_t* queue)
{
    if (NULL == queue || queue->push_count == queue->pop_count) {
        return NULL;
    }

    util_list_node_t *node = util_list_tail(&queue->list);
    if (NULL != node) {
        queue->pop_count++;
        util_list_remove(node);
        return util_list_data(node, util_queue_node_t, node)->data;
    }
    
    return NULL;
}

void* util_queue_peek(util_queue_t* queue)
{
    if (NULL == queue || queue->push_count == queue->pop_count) {
        return NULL;
    }

    util_list_node_t *node = util_list_tail(&queue->list);
    if (NULL != node) {
        return util_list_data(node, util_queue_node_t, node)->data;
    }
    
    return NULL;
}

size_t util_queue_count(util_queue_t* queue)
{
    if (NULL == queue) {
        return -1;
    }

    return queue->push_count - queue->pop_count;
}

void util_queue_clear(util_queue_t* queue)
{
    if (NULL != queue && !util_list_empty(&queue->list)) {
        util_list_node_t *pos, *tmp;
        util_list_for_each_next(pos, tmp, &queue->list) {
            util_list_remove(pos);
        }
        queue->push_count = queue->pop_count = 0;
    }
}
