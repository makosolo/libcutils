#ifndef H_UTILS_LIST_H
#define H_UTILS_LIST_H

#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

struct util_list_node_s {
    struct util_list_node_s* prev;
    struct util_list_node_s* next;
};

typedef struct  util_list_node_s util_list_t;
typedef struct  util_list_node_s util_list_node_t;

void              util_list_init(util_list_t* list);
void              util_list_insert_head(util_list_t* list, util_list_node_t* node);
void              util_list_insert_tail(util_list_t* list, util_list_node_t* node);
bool              util_list_empty(util_list_t* list);
util_list_node_t* util_list_head(util_list_t* list);
util_list_node_t* util_list_tail(util_list_t* list);
util_list_node_t* util_list_next(util_list_node_t* node);
util_list_node_t* util_list_prev(util_list_node_t* node);
void              util_list_remove(util_list_node_t* node);

#define         util_list_data(ptr, type, member)                              \
                ((type *) ((char *) (ptr) - offsetof(type, member)))
#define         util_list_for_each_next(pos, p, list) \
                for (pos = (list)->next, p = pos->next; \
                    pos != (list); \
                    pos = p, p = pos->next)

#define         util_list_for_each_prev(pos, p, list) \
                for (pos = (list)->prev, p = pos->prev; \
                    pos != (list); \
                    pos = p, p = pos->prev)

#ifdef __cplusplus
}
#endif

#endif // H_UTILS_LIST_H
