#include "utils_array.h"

#define UTIL_ARRAY_MAX SIZE_MAX

#define util_array_def(name, T)                                       										\
											   																\
	/**																										\
	 * Init array																							\
	 * @param a array																						\
	 */																										\
	int util_array_create_##name(struct util_array_##name *array) 										\
    {                                                                                                       \
        if (NULL == array) {                                                                                \
            return -1;                                                                                      \
        }                                                                                                   \
                                                                                                            \
        memset((array), 0, sizeof(*(array)));                                                               \
        return 0;                                                                                           \
    }                                                                                                       \
																											\
	/**																										\
	 * Destroy array																						\
	 * @param a array																						\
	 */																										\
	int util_array_destroy_##name(struct util_array_##name *array)										\
    {                                                                                                       \
        if (NULL == array) {                                                                                \
            return -1;                                                                                      \
        }                                                                                                   \
                                                                                                            \
        if (NULL != (array)->elems) {                                                                       \
            free((array)->elems);                                                                           \
        }                                                                                                   \
		                                                                                                    \
		memset((array), 0, sizeof(*(array)));                                                               \
        return 0;                                                                                           \
    }                                                                                                       \
																											\
	/**																										\
	 * Add elem to array, call util_array_oom(v) to see if 'add' failed because of out						\
	 * of memory.																							\
	 *																										\
	 * @param a array																						\
	 * @param k elem																						\
	 */																										\
	int util_array_add_##name(struct util_array_##name *array, T elem)                      				\
    {                                                                                                       \
		size_t _max = 0;                                                                                    \
		size_t _cap;                                                                                        \
		void *_p;                                                                                           \
                                                                                                            \
        if (NULL == array) {                                                                                \
            return -1;                                                                                      \
        }                                                                                                   \
                                                                                                            \
        _max = UTIL_ARRAY_MAX / sizeof(*(array)->elems);                                                      \
		if ((array)->cap == (array)->size) {                                                                \
			if ((array)->cap > _max / 2) {                                                                  \
				(array)->oom = true;                                                                        \
				return -1;                                                                                  \
			}                                                                                               \
			_cap = (array)->cap == 0 ? 8 : (array)->cap * 2;                                                \
			_p = realloc((array)->elems, _cap * sizeof(*((array)->elems)));                                 \
			if (_p == NULL) {                                                                               \
				(array)->oom = true;                                                                        \
				return -1;                                                                                  \
			}                                                                                               \
			(array)->cap = _cap;                                                                            \
			(array)->elems = _p;                                                                            \
		}                                                                                                   \
		(array)->oom = false;                                                                               \
		(array)->elems[(array)->size++] = elem;                                                             \
                                                                                                            \
        return 0;                                                                                           \
    }                                                                                                       \
																											\
	/**																										\
	 * Deletes items from the array without deallocating underlying memory									\
	 * @param a array																						\
	 */																										\
	int util_array_clear_##name(struct util_array_##name *array)                          				\
    {                                                                                                       \
        if (NULL == array) {                                                                                \
            return -1;                                                                                      \
        }                                                                                                   \
                                                                                                            \
		(array)->size = 0;                                                                                  \
		(array)->oom = false;                                                                               \
                                                                                                            \
        return 0;                                                                                           \
    }                                                                                                       \
																											\
	/**																										\
	 *   @param a array																						\
	 *   @param i element index, If 'i' is out of the range, result is undefined.							\
	 */																										\
	int util_array_del_##name(struct util_array_##name *array, uint32_t i)                				\
    {                                                                                                       \
        if (NULL == array || i >= (array)->size) {                                                          \
            return -1;                                                                                      \
        }                                                                                                   \
                                                                                                            \
        size_t idx = (i);                                                                                   \
                                                                                                            \
		const size_t _cnt = (array)->size - (idx) - 1;                                                      \
		if (_cnt > 0) {                                                                                     \
			memmove(&((array)->elems[idx]), &((array)->elems[idx + 1]),                                     \
				_cnt * sizeof(*((array)->elems)));                                                          \
		}                                                                                                   \
		(array)->size--;                                                                                    \
                                                                                                            \
        return (array)->size;                                                                               \
    }                                                                                                       \
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
	int util_array_del_unordered_##name(struct util_array_##name *array, uint32_t i)            			\
    {                                                                                                       \
        if (NULL == array || i >= (array)->size) {                                                          \
            return -1;                                                                                      \
        }                                                                                                   \
                                                                                                            \
        size_t idx = (i);                                                                                   \
		(array)->elems[idx] = (array)->elems[(--(array)->size)];                                            \
                                                                                                            \
        return 0;                                                                                           \
    }                                                                                                       \
																											\
	/**																										\
	 * Deletes the last element. If current size is zero, result is undefined.								\
	 * @param a array																						\
	 */																										\
	int util_array_del_last_##name(struct util_array_##name *array)                             			\
    {                                                                                                       \
        if (NULL == array || 0 == (array)->size) {                                                          \
            return -1;                                                                                      \
        }                                                                                                   \
                                                                                                            \
		(array)->size--;                                                                                    \
        return 0;                                                                                           \
    }                                                                                                       \
																											\
	/**																										\
	 * Sorts the array using qsort()																		\
	 * @param a   array																						\
	 * @param cmp comparator, check qsort() documentation for details										\
	 */																										\
	int util_array_sort_##name(struct util_array_##name *array, onUtilArrayComparCallback cmp)      	    \
    {                                                                                                       \
        if (NULL == array || 0 == (array)->size || NULL == cmp) {                                           \
            return -1;                                                                                      \
        }                                                                                                   \
                                                                                                            \
        qsort((array)->elems, (array)->size, sizeof(*(array)->elems), cmp);                                 \
        return 0;                                                                                           \
    }


//        (name, value type)
util_array_def(int, 	int)
util_array_def(uint, 	unsigned int)
util_array_def(long, 	long)
util_array_def(ll, 	    long long)
util_array_def(ulong, 	unsigned long)
util_array_def(ull, 	unsigned long long)
util_array_def(32, 	    uint32_t)
util_array_def(64, 	    uint64_t)
util_array_def(double,  double)
util_array_def(str, 	const char *)
util_array_def(ptr, 	void *)
