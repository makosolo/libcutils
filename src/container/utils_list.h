#ifndef H_UTIL_LIST_H
#define H_UTIL_LIST_H

#include <stddef.h>
#include <stdbool.h>

struct util_list_node_s {
	struct util_list_node_s* p;
	struct util_list_node_s* n;
};

typedef struct  util_list_node_s util_list_t;
typedef struct  util_list_node_s util_list_node_t;

void              util_list_init(util_list_t* l);
void              util_list_insert_head(util_list_t* l, util_list_node_t* x);
void              util_list_insert_tail(util_list_t* l, util_list_node_t* x);
bool              util_list_empty(util_list_t* l);
util_list_node_t* util_list_head(util_list_t* l);
util_list_node_t* util_list_tail(util_list_t* l);
util_list_node_t* util_list_next(util_list_node_t* x);
util_list_node_t* util_list_prev(util_list_node_t* x);
void              util_list_remove(util_list_node_t* x);
util_list_node_t* util_list_sentinel(util_list_t* l);

#define         util_list_data(x, t, m)                              \
                ((t *) ((char *) (x) - offsetof(t, m)))
#define         util_list_for_each_next(n, t, l) \
                for (n = (l)->next, t = n->next; \
                    n != (l); \
                    n = t, t = n->next)

#define         util_list_for_each_prev(n, t, l) \
                for (n = (l)->prev, t = n->prev; \
                    n != (l); \
                    n = t, t = n->prev)

#endif // H_UTIL_LIST_H
