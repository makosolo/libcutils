#ifndef H_UTILS_QUEUE_H
#define H_UTILS_QUEUE_H

#include "utils_list.h"

typedef struct {
    util_list_t     list;
    size_t          max_count;
    size_t          push_count;
    size_t          pop_count;
} util_queue_t;

void   util_queue_init(util_queue_t* queue, size_t max_count);
int    util_queue_push(util_queue_t* queue, void* data);
void*  util_queue_pop(util_queue_t* queue);
void*  util_queue_peek(util_queue_t* queue);
size_t util_queue_count(util_queue_t* queue);
void   util_queue_clear(util_queue_t* queue);

#endif // H_UTILS_QUEUE_H
