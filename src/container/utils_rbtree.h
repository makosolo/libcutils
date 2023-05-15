#ifndef H_UTILS_RBTREE_H
#define H_UTILS_RBTREE_H

typedef enum util_rbtree_key_type_e {
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
} util_rbtree_key_type_t;

typedef union util_rbtree_key_u {
	char*    str;
	int8_t	 i8;
	int16_t  i16;
	int32_t  i32;
	int64_t  i64;
	uint8_t	 u8;
	uint16_t u16;
	uint32_t u32;
	uint64_t u64;
	void*    ptr;
} util_rbtree_key_t;

typedef struct util_rbtree_node_s
{
	util_rbtree_key_t rb_key;
	struct util_rbtree_node_s* rb_parent;
	struct util_rbtree_node_s* rb_right;
	struct util_rbtree_node_s* rb_left;
	char rb_color;
} util_rbtree_node_t;

typedef struct util_rbtree_node_t util_rbtree_t;

void             	util_rbtree_init(util_rbtree_t* tree, util_rbtree_key_type_t type);
void             	util_rbtree_insert(util_rbtree_t* tree, util_rbtree_key_t* key, void *data);
util_rbtree_node_t*	util_rbtree_find(util_rbtree_t* tree, util_rbtree_key_t* key);
void             	util_rbtree_erase(util_rbtree_t* tree, util_rbtree_key_t* key);
util_rbtree_node_t* util_rbtree_next(util_rbtree_node_t* node);
util_rbtree_node_t* util_rbtree_prev(util_rbtree_node_t* node);
util_rbtree_node_t* util_rbtree_first(util_rbtree_t* tree);
util_rbtree_node_t* util_rbtree_last(util_rbtree_t* tree);
bool             	util_rbtree_empty(util_rbtree_t* tree);
uint32_t           	util_rbtree_count(util_rbtree_t* tree);

#define        		util_rbtree_data(ptr, type, member)                              \
               		((type *) ((char *) (ptr) - offsetof(type, member)))

#endif // H_UTILS_RBTREE_H
