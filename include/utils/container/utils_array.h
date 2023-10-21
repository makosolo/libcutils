#ifndef H_UTILS_ARRAY_H
#define H_UTILS_ARRAY_H

#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

struct util_array_s;
typedef struct util_array_s util_array_t;

int util_array_create(util_array_t **array, bool zero_terminated, bool clear, size_t elt_size, size_t reserved_size);

#ifdef __cplusplus
}
#endif

#endif // H_UTILS_LIST_H
