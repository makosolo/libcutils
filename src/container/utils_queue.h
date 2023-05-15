#ifndef H_UTILS_QUEUE_H
#define H_UTILS_QUEUE_H

#include "utils_list.h"

typedef struct  util_list_t util_queue_t;

void  util_queue_create(util_queue_t* queue, uint32_t max_count);
void  util_queue_destroy(util_queue_t* queue);
void  util_queue_push(util_queue_t* queue, void* data);
void* util_queue_pop(util_queue_t* queue);
void* util_queue_peek(util_queue_t* queue);
bool  util_queue_count(util_queue_t* queue);

#endif // H_UTILS_QUEUE_H
