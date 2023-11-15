#ifndef H_UTILS_HEAP_H
#define H_UTILS_HEAP_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

struct util_heap_data_s {
	int64_t key;
	void*   data;
};

typedef struct util_heap_data_s util_heap_data_t;

struct util_heap_s {
	size_t              cap;
	size_t              size;
	util_heap_data_t*   elems;
};

typedef struct util_heap_s util_heap_t;

/**
 * @param heap heap
 * @param cap initial capacity, pass '0' for no initial memory allocation
 * @return    'true' on success, 'false' on out of memory
 */
int util_heap_create(util_heap_t *heap, size_t cap);

/**
 * Destroys heap, frees memory
 * @param heap heap
 */
int util_heap_destroy(util_heap_t *heap);

/**
 * @param heap heap
 * @return  element count
 */
size_t util_heap_size(util_heap_t *heap);

/**
 * Clears elements from the queue, does not free the allocated memory.
 * @param heap heap
 */
int util_heap_clear(util_heap_t *heap);

/**
 * @param heap heap
 * @param key  key
 * @param data data
 * @return     'false' on out of memory.
 */
int util_heap_push(util_heap_t *heap, int64_t key, void *data);

/**
 * Read top element without removing from the heap.
 *
 * @param heap heap
 * @return     pointer to data holder(valid until next heap operation)
 *             NULL if heap is empty.
 */
util_heap_data_t *util_heap_peek(util_heap_t *heap);

/**
 * Read top element and remove it from the heap.
 *
 * @param heap heap
 * @return     pointer to data holder(valid until next heap operation)
 *             NULL if heap is empty.
 */
util_heap_data_t *util_heap_pop(util_heap_t *heap);

#ifdef __cplusplus
}
#endif

#endif // H_UTILS_CYC_BUFFER_H
