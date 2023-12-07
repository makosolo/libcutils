/**
 * @file utils_map.h
 * @author enguo.shan (enguo.shan@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-10-23
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef UTILS_MAP_H
#define UTILS_MAP_H

#include <memory.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

#define util_map_dec_strkey(name, K, V)                                    \
	struct util_map_item_##name {                                          \
		K key;                                                             \
		V value;                                                           \
		uint32_t hash;                                                     \
	};                                                                     \
                                                                           \
	util_map_of(name, K, V)

#define util_map_dec_scalar(name, K, V)                                    \
	struct util_map_item_##name {                                          \
		K key;                                                             \
		V value;                                                           \
	};                                                                     \
                                                                           \
	util_map_of(name, K, V)

#define util_map_of(name, K, V)                                            \
	struct util_map_##name {                                               \
		struct util_map_item_##name *mem;                                  \
		uint32_t cap;                                                      \
		uint32_t size;                                                     \
		uint32_t load_fac;                                                 \
		uint32_t remap;                                                    \
		bool used;                                                         \
		bool oom;                                                          \
		bool found;                                                        \
	};                                                                     \
                                                                           \
	/**                                                                    \
	 * Create map                                                          \
	 *                                                                     \
	 * @param map map                                                      \
	 * @param cap initial capacity, zero is accepted                       \
	 * @param load_factor must be >25 and <95. Pass 0 for default value.   \
	 * @return 'true' on success,                                          \
	 *         'false' on out of memory or if 'load_factor' value is       \
	 *          invalid.                                                   \
	 */                                                                    \
	bool util_map_create_##name(struct util_map_##name *map, uint32_t cap,   \
				uint32_t load_factor);                                     \
                                                                           \
	/**                                                                    \
	 * Destroy map.                                                        \
	 *                                                                     \
	 * @param map map                                                      \
	 */                                                                    \
	void util_map_destroy_##name(struct util_map_##name *map);             \
                                                                           \
	/**                                                                    \
	 * Get map element count                                               \
	 *                                                                     \
	 * @param map map                                                      \
	 * @return element count                                               \
	 */                                                                    \
	uint32_t util_map_size_##name(struct util_map_##name *map);            \
                                                                           \
	/**                                                                    \
	 * Clear map                                                           \
	 *                                                                     \
	 * @param map map                                                      \
	 */                                                                    \
	void util_map_clear_##name(struct util_map_##name *map);               \
                                                                           \
	/**                                                                    \
	 * Put element to the map                                              \
	 *                                                                     \
	 * struct util_map_str map;                                            \
	 * util_map_put_str(&map, "key", "value");                             \
	 *                                                                     \
	 * @param map map                                                      \
	 * @param K key                                                        \
	 * @param V value                                                      \
	 * @return previous value if exists                                    \
	 *         call util_map_found() to see if the returned value is valid.\
	 */                                                                    \
	V util_map_put_##name(struct util_map_##name *map, K key, V val);      \
                                                                           \
	/**                                                                    \
	 * Get element                                                         \
	 *                                                                     \
	 * @param map map                                                      \
	 * @param K key                                                        \
	 * @return current value if exists.                                    \
	 *         call util_map_found() to see if the returned value is valid.\
	 */                                                                    \
	/** NOLINTNEXTLINE */                                                  \
	V util_map_get_##name(struct util_map_##name *map, K key);             \
                                                                           \
	/**                                                                    \
	 * Delete element                                                      \
	 *                                                                     \
	 * @param map map                                                      \
	 * @param K key                                                        \
	 * @return current value if exists.                                    \
	 *         call util_map_found() to see if the returned value is valid.\
	 */                                                                    \
	/** NOLINTNEXTLINE */                                                  \
	V util_map_del_##name(struct util_map_##name *map, K key);

/**
 * @param map map
 * @return    - if put operation overrides a value, returns true
 *            - if get operation finds the key, returns true
 *            - if del operation deletes a key, returns true
 */
#define util_map_found(map) ((map)->found)

/**
 * @param map map
 * @return    true if put operation failed with out of memory
 */
#define util_map_oom(map) ((map)->oom)

/**
 * Foreach loop
 *
 * char *key, *value;
 * struct util_map_str map;
 *
 * util_map_foreach(&map, key, value) {
 *      printf("key = %s, value = %s \n");
 * }
 */
#define util_map_foreach(map, K, V)                                            \
	for (int64_t _i = -1, _b = 0; !_b && _i < (map)->cap; _i++)                \
		for ((V) = (map)->mem[_i].value, (K) = (map)->mem[_i].key, _b = 1;     \
		     _b && ((_i == -1 && (map)->used) || (K) != 0) ? 1 : (_b = 0);     \
		     _b = 0)

/**
 * Foreach loop for keys
 *
 * char *key;
 * struct util_map_str map;
 *
 * util_map_foreach_key(&map, key) {
 *      printf("key = %s \n");
 * }
 */
#define util_map_foreach_key(map, K)                                           \
	for (int64_t _i = -1, _b = 0; !_b && _i < (map)->cap; _i++)                \
		for ((K) = (map)->mem[_i].key, _b = 1;                                 \
		     _b && ((_i == -1 && (map)->used) || (K) != 0) ? 1 : (_b = 0);     \
		     _b = 0)

/**
 * Foreach loop for values
 *
 * char *value;
 * struct util_map_str map;
 *
 * util_map_foreach_value(&map, value) {
 *      printf("value = %s \n");
 * }
 */
#define util_map_foreach_value(map, V)                                                    \
	for (int64_t _i = -1, _b = 0; !_b && _i < (map)->cap; _i++)                           \
		for ((V) = (map)->mem[_i].value, _b = 1;                                          \
		     _b && ((_i == -1 && (map)->used) || (map)->mem[_i].key != 0) ? 1 : (_b = 0); \
		     _b = 0)

// integer keys: name  key type      value type
util_map_dec_scalar(int, int,          int)
util_map_dec_scalar(intv,int,          void*)
util_map_dec_scalar(ints,int,          const char*)
util_map_dec_scalar(ll,  long long,    long long)
util_map_dec_scalar(llv, long long,    void *)
util_map_dec_scalar(lls, long long,    const char *)
util_map_dec_scalar(32,  uint32_t,     uint32_t)
util_map_dec_scalar(64,  uint64_t,     uint64_t)
util_map_dec_scalar(64v, uint64_t,     void *)
util_map_dec_scalar(64s, uint64_t,     const char *)

// string keys:  name  key type      value type
util_map_dec_strkey(str, const char *, const char *)
util_map_dec_strkey(sv,  const char *, void*)
util_map_dec_strkey(s64, const char *, uint64_t)
util_map_dec_strkey(sll, const char *, long long)

#ifdef __cplusplus
}
#endif

#endif
