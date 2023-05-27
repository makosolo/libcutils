#ifndef H_UTILS_QUEUE_H
#define H_UTILS_QUEUE_H

#include "utils_list.h"

struct util_queue_s;
typedef struct util_queue_s util_queue_t;

util_queue_t*   util_queue_creat(size_t max_count);
void            util_queue_destroy(util_queue_t* queue);
int             util_queue_push(util_queue_t* queue, void* data);
void*           util_queue_pop(util_queue_t* queue);
void*           util_queue_peek(util_queue_t* queue);
size_t          util_queue_count(util_queue_t* queue);
void            util_queue_clear(util_queue_t* queue);

#endif // H_UTILS_QUEUE_H
