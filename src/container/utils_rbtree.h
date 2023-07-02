#ifndef H_UTILS_RBTREE_H
#define H_UTILS_RBTREE_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum util_rbtree_key_type {
    UTIL_RBTREE_KEY_TYPE_STR,
    UTIL_RBTREE_KEY_TYPE_I8,
    UTIL_RBTREE_KEY_TYPE_I16,
    UTIL_RBTREE_KEY_TYPE_I32,
    UTIL_RBTREE_KEY_TYPE_I64,
    UTIL_RBTREE_KEY_TYPE_U8,
    UTIL_RBTREE_KEY_TYPE_U16,
    UTIL_RBTREE_KEY_TYPE_U32,
    UTIL_RBTREE_KEY_TYPE_U64,
    UTIL_RBTREE_KEY_TYPE_PTR,
} util_rbtree_key_type_e;

/* red brack tree's key */
#define UTILS_RBTREE_KEY_STR_MAX (64)
typedef union util_rbtree_key {
    char     str[UTILS_RBTREE_KEY_STR_MAX+1];
    int8_t   i8;
    int16_t  i16;
    int32_t  i32;
    int64_t  i64;
    uint8_t  u8;
    uint16_t u16;
    uint32_t u32;
    uint64_t u64;
    void*    ptr;
} util_rbtree_key_u;

/* red brack tree's node */
struct util_rbtree_node;
typedef struct util_rbtree_node util_rbtree_node_t;

/* red brack tree */
struct util_rbtree_s;
typedef struct util_rbtree_s util_rbtree_t;

util_rbtree_key_u   util_rbtree_key_str(char *str);
util_rbtree_key_u   util_rbtree_key_i8(int8_t i8);
util_rbtree_key_u   util_rbtree_key_i16(int16_t i16);
util_rbtree_key_u   util_rbtree_key_i32(int32_t i32);
util_rbtree_key_u   util_rbtree_key_i64(int64_t i64);
util_rbtree_key_u   util_rbtree_key_u8(uint8_t u8);
util_rbtree_key_u   util_rbtree_key_u16(uint16_t u16);
util_rbtree_key_u   util_rbtree_key_u32(uint32_t u32);
util_rbtree_key_u   util_rbtree_key_u64(uint64_t u64);
util_rbtree_key_u   util_rbtree_key_ptr(void *ptr);

int                 util_rbtree_create(util_rbtree_t** tree, util_rbtree_key_type_e type);
int                 util_rbtree_delete(util_rbtree_t** tree);
void                util_rbtree_erase(util_rbtree_t* tree, util_rbtree_node_t* node);
int                 util_rbtree_insert(util_rbtree_t* tree, util_rbtree_key_u key, void *data);
util_rbtree_node_t* util_rbtree_find(util_rbtree_t* tree, util_rbtree_key_u key);
util_rbtree_node_t* util_rbtree_next(util_rbtree_node_t* node);
util_rbtree_node_t* util_rbtree_prev(util_rbtree_node_t* node);
util_rbtree_node_t* util_rbtree_first(util_rbtree_t* tree);
util_rbtree_node_t* util_rbtree_last(util_rbtree_t* tree);
bool                util_rbtree_empty(util_rbtree_t* tree);
int                 util_rbtree_get_key(util_rbtree_node_t* node, util_rbtree_key_type_e type, void *buf, size_t size);
void*               util_rbtree_data(util_rbtree_node_t* node);

#ifdef __cplusplus
}
#endif

#endif // H_UTILS_RBTREE_H
