#include "utils_list.h"

void util_list_init(util_list_t* l) {

	(l)->n = (l);
	(l)->p = (l);
}

void util_list_insert_head(util_list_t* l, util_list_node_t* x) {

	/**
	 * init node;
	 */
	(x)->n     = (x);
	(x)->p     = (x);
	/**
	 * insert node;
	 */
	(x)->n     = (l)->n;
	(x)->p->n  = (x);
	(x)->p     = (l);
	(l)->n     = (x);
}

void util_list_insert_tail(util_list_t* l, util_list_node_t* x) {

	/**
	 * init node;
	 */
	(x)->n = (x);
	(x)->p = (x);
	/**
	 * insert node;
	 */
	(x)->p     = (l)->p;
	(x)->p->n  = (x);
	(x)->n     = (l);
	(l)->p     = (x);
}

bool util_list_empty(util_list_t* l) {

	return (l) == (l)->p;
}

util_list_node_t* util_list_head(util_list_t* l) {

	return (l)->n;
}

util_list_node_t* util_list_tail(util_list_t* l) {

	return (l)->p;
}

util_list_node_t* util_list_sentinel(util_list_t* l) {

	return (l);
}

void util_list_remove(util_list_node_t* x) {

	(x)->n->p   = (x)->p;
	(x)->p->n   = (x)->n;
}

util_list_node_t* util_list_next(util_list_node_t* x) {

	return (x)->n;
}

util_list_node_t* util_list_prev(util_list_node_t* x) {

	return (x)->p;
}