/**
 * @file utils_array.h
 * @author enguo.shan (enguo.shan@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-10-25
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef H_UTILS_ARRAY_H
#define H_UTILS_ARRAY_H

#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef int (*onUtilArrayComparCallback) (const void *, const void *);

#define util_array_dec(name, T)                                       										\
	struct util_array_##name {                                        										\
		bool oom;                                                      										\
		size_t cap;                                                    										\
		size_t size;                                                   										\
		/* NOLINTNEXTLINE */                                           										\
		T *elems;                                                      										\
	};																   										\
											   																\
	/**																										\
	 * Init array																							\
	 * @param a array																						\
	 */																										\
	int util_array_create_##name(struct util_array_##name *array);										\
																											\
	/**																										\
	 * Destroy array																						\
	 * @param a array																						\
	 */																										\
	int util_array_destroy_##name(struct util_array_##name *array);										\
																											\
	/**																										\
	 * Add elem to array, call util_array_oom(v) to see if 'add' failed because of out						\
	 * of memory.																							\
	 *																										\
	 * @param a array																						\
	 * @param k elem																						\
	 */																										\
	int util_array_add_##name(struct util_array_##name *array, T elem);                     				\
																											\
	/**																										\
	 * Deletes items from the array without deallocating underlying memory									\
	 * @param a array																						\
	 */																										\
	int util_array_clear_##name(struct util_array_##name *array);                          				\
																											\
	/**																										\
	 *   @param a array																						\
	 *   @param i element index, If 'i' is out of the range, result is undefined.							\
	 */																										\
	int util_array_del_##name(struct util_array_##name *array, uint32_t i);                				\
																											\
	/**																										\
	 * Deletes the element at index i, replaces last element with the deleted								\
	 * element unless deleted element is the last element. This is faster than								\
	 * moving elements but elements will no longer be in the 'add order'									\
	 *																										\
	 * arr[a,b,c,d,e,f] -> util_array_del_unordered(arr, 2) - > arr[a,b,f,d,e]								\
	 *																										\
	 * @param a array																						\
	 * @param i index. If 'i' is out of the range, result is undefined.										\
	 */																										\
	int util_array_del_unordered_##name(struct util_array_##name *array, uint32_t i);            			\
																											\
	/**																										\
	 * Deletes the last element. If current size is zero, result is undefined.								\
	 * @param a array																						\
	 */																										\
	int util_array_del_last_##name(struct util_array_##name *array);                             			\
																											\
	/**																										\
	 * Sorts the array using qsort()																		\
	 * @param a   array																						\
	 * @param cmp comparator, check qsort() documentation for details										\
	 */																										\
	int util_array_sort_##name(struct util_array_##name *array, onUtilArrayComparCallback cmp);

/**
 * @param a array
 * @return true if last add operation failed, false otherwise.
 */
#define util_array_oom(array) ((array)->oom)

/**
 * Get element at index i, if 'i' is out of range, result is undefined.
 *
 * @param a array
 * @param i index
 * @return element at index 'i'
 */
#define util_array_at(array, i) ((array)->elems[i])

/**
 * @param a array
 * @return element count
 */
#define util_array_size(array) ((array)->size)

/**
 * Returns last element. If array is empty, result is undefined.
 * @param a array
 */
#define util_array_last(array) (array)->elems[(array)->size - 1]

/**
 * @param a    array
 * @param elem elem
 */
#define util_array_foreach(array, elem)                                    \
	for (size_t _k = 1, _i = 0; _k && _i != (array)->size; _k = !_k, _i++)  \
		for ((elem) = (array)->elems[_i]; _k; _k = !_k)

//        (name, value type)
util_array_dec(int, 	int)
util_array_dec(uint, 	unsigned int)
util_array_dec(long, 	long)
util_array_dec(ll, 		long long)
util_array_dec(ulong, 	unsigned long)
util_array_dec(ull, 	unsigned long long)
util_array_dec(32, 		uint32_t)
util_array_dec(64, 		uint64_t)
util_array_dec(double, 	double)
util_array_dec(str, 	const char *)
util_array_dec(ptr, 	void *)

#ifdef __cplusplus
}
#endif

#endif // H_UTILS_LIST_H
