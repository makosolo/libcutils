#include "utils_list.h"

void util_list_init(util_list_t* list) {

	(list)->next = (list);
	(list)->prev = (list);
}

void util_list_insert_head(util_list_t* list, util_list_node_t* node) {

	/**
	 * init node;
	 */
	(node)->next     = (node);
	(node)->prev     = (node);
	/**
	 * insert node;
	 */
	(node)->next     = (list)->next; // n1 = list->next; node->next = n1
	(node)->next->prev  = (node);    // n1 = list->next; n1->prev = node
	(node)->prev     = (list);
	(list)->next     = (node);
}

void util_list_insert_tail(util_list_t* list, util_list_node_t* node) {

	/**
	 * init node;
	 */
	(node)->next = (node);
	(node)->prev = (node);
	/**
	 * insert node;
	 */
	(node)->prev     = (list)->prev; // tail = list->prev; node->prev = tail
	(node)->prev->next  = (node); 	 // tail = list->prev; tail->next = node
	(node)->next     = (list);
	(list)->prev     = (node);
}

bool util_list_empty(util_list_t* list) {

	return (list) == (list)->prev;
}

util_list_node_t* util_list_head(util_list_t* list) {

	return (list)->next;
}

util_list_node_t* util_list_tail(util_list_t* list) {

	return (list)->prev;
}

void util_list_remove(util_list_node_t* node) {

	(node)->next->prev   = (node)->prev;
	(node)->prev->next   = (node)->next;
}

util_list_node_t* util_list_next(util_list_node_t* node) {

	return (node)->next;
}

util_list_node_t* util_list_prev(util_list_node_t* node) {

	return (node)->prev;
}