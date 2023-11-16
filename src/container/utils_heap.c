#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "utils_heap.h"

#define UTIL_HEAP_MAX   (SIZE_MAX / sizeof(util_heap_data_t))

int util_heap_create(util_heap_t *heap, size_t cap)
{
	void *e;
	const size_t sz = cap * sizeof(util_heap_data_t);

    if (NULL == heap) {
        return -1;
    }
    
	*heap = (util_heap_t){0};

	if (cap == 0) {
		return 0;
	}

	if (cap > UTIL_HEAP_MAX || (e = malloc(sz)) == NULL) {
		return -1;
	}

	heap->elems = e;
	heap->cap   = cap;

	return 0;
}

int util_heap_destroy(util_heap_t *heap)
{
    if (NULL == heap) {
        return -1;
    }

    if (NULL != heap->elems) {
        free(heap->elems);
    }

	*heap = (util_heap_t){
		.elems = NULL,
	};

    return 0;
}

size_t util_heap_size(util_heap_t *heap)
{
    if (NULL == heap) {
        return 0;
    }

	return heap->size;
}

int util_heap_clear(util_heap_t *heap)
{
    if (NULL == heap) {
        return -1;
    }

	heap->size = 0;

    return 0;
}

int util_heap_put(util_heap_t *heap, int64_t key, void *data)
{
	size_t i, cap, m;
	void *exp;

    if (NULL == heap || NULL == data) {
        return -1;
    }

	if (++heap->size >= heap->cap) {
		cap = heap->cap != 0 ? heap->cap * 2 : 4;
		m = cap * 2 * sizeof(*heap->elems);

		if (heap->cap >= UTIL_HEAP_MAX / 2 ||
		    (exp = realloc(heap->elems, m)) == NULL) {
			return -2;
		}

		heap->elems = exp;
		heap->cap   = cap;
	}

	i = heap->size;
	while (i != 1 && key < heap->elems[i / 2].key) {
		heap->elems[i] = heap->elems[i / 2];
		i /= 2;
	}

	heap->elems[i].key  = key;
	heap->elems[i].data = data;

	return 0;
}

util_heap_data_t *util_heap_peek(util_heap_t *heap)
{
	if (NULL == heap || heap->size == 0) {
		return NULL;
	}

	// Top element is always at heap->elems[1].
	return &heap->elems[1];
}

util_heap_data_t *util_heap_pop(util_heap_t *heap)
{
	size_t i = 1, child = 2;
	util_heap_data_t last;

	if (NULL == heap || heap->size == 0) {
		return NULL;
	}

	// Top element is always at heap->elems[1].
	heap->elems[0] = heap->elems[1];

	last = heap->elems[heap->size--];
	while (child <= heap->size) {
		if (child < heap->size &&
		    heap->elems[child].key > heap->elems[child + 1].key) {
			child++;
		}

		if (last.key <= heap->elems[child].key) {
			break;
		}

		heap->elems[i] = heap->elems[child];

		i = child;
		child *= 2;
	}

	heap->elems[i] = last;

	return &heap->elems[0];
}
