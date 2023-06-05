#include <stdlib.h>
#include <string.h>

#include "utils_list.h"

#include "utils_rbtree.h"

#define	RB_RED			(0)
#define	RB_BLACK		(1)

struct util_rbtree_node_s
{
    util_rbtree_key_t rb_key;
    struct util_rbtree_node_s* rb_parent;
    struct util_rbtree_node_s* rb_right;
    struct util_rbtree_node_s* rb_left;
    char rb_color;
};

struct util_rbtree_s
{
	util_rbtree_node_t* rb_root;
	int(*rb_keycmp)(util_rbtree_key_t*, util_rbtree_key_t*);
};

typedef struct {
    util_rbtree_node_t  node;
    void*               data;
} util_rbtree_data_t;

static int keycmp_i8(util_rbtree_key_t* k1, util_rbtree_key_t* k2)
{
	if (k1->i8 < k2->i8) {
		return -1;
	}
	if (k1->i8 > k2->i8) {
		return 1;
	}
	return 0;
}

static int keycmp_i16(util_rbtree_key_t* k1, util_rbtree_key_t* k2)
{
	if (k1->i16 < k2->i16) {
		return -1;
	}
	if (k1->i16 > k2->i16) {
		return 1;
	}
	return 0;
}

static int keycmp_i32(util_rbtree_key_t* k1, util_rbtree_key_t* k2)
{
	if (k1->i32 < k2->i32) {
		return -1;
	}
	if (k1->i32 > k2->i32) {
		return 1;
	}
	return 0;
}

static int keycmp_i64(util_rbtree_key_t* k1, util_rbtree_key_t* k2)
{
	if (k1->i64 < k2->i64) {
		return -1;
	}
	if (k1->i64 > k2->i64) {
		return 1;
	}
	return 0;
}

static int keycmp_u8(util_rbtree_key_t* k1, util_rbtree_key_t* k2)
{
	if (k1->u8 < k2->u8) {
		return -1;
	}
	if (k1->u8 > k2->u8) {
		return 1;
	}
	return 0;
}

static int keycmp_u16(util_rbtree_key_t* k1, util_rbtree_key_t* k2)
{
	if (k1->u16 < k2->u16) {
		return -1;
	}
	if (k1->u16 > k2->u16) {
		return 1;
	}
	return 0;
}

static int keycmp_u32(util_rbtree_key_t* k1, util_rbtree_key_t* k2)
{
	if (k1->u32 < k2->u32) {
		return -1;
	}
	if (k1->u32 > k2->u32) {
		return 1;
	}
	return 0;
}

static int keycmp_u64(util_rbtree_key_t* k1, util_rbtree_key_t* k2)
{
	if (k1->u64 < k2->u64) {
		return -1;
	}
	if (k1->u64 > k2->u64) {
		return 1;
	}
	return 0;
}

static int keycmp_ptr(util_rbtree_key_t* k1, util_rbtree_key_t* k2)
{
	if (k1->ptr < k2->ptr) {
		return -1;
	}
	if (k1->ptr > k2->ptr) {
		return 1;
	}
	return 0;
}

static int keycmp_str(util_rbtree_key_t* k1, util_rbtree_key_t* k2)
{
	return strcmp(k1->str, k2->str);
}

static inline void __rbtree_rotate_left(util_rbtree_node_t* node, util_rbtree_t* tree)
{
	util_rbtree_node_t* right = node->rb_right;

	if ((node->rb_right = right->rb_left)) {

		right->rb_left->rb_parent = node;
	}
	right->rb_left = node;

	if ((right->rb_parent = node->rb_parent)) {

		if (node == node->rb_parent->rb_left) {

			node->rb_parent->rb_left = right;
		}
		else {
			node->rb_parent->rb_right = right;
		}
	}
	else {
		tree->rb_root = right;
	}
	node->rb_parent = right;
}

static inline void __rbtree_rotate_right(util_rbtree_node_t* node, util_rbtree_t* tree)
{
	util_rbtree_node_t* left = node->rb_left;

	if ((node->rb_left = left->rb_right)) {

		left->rb_right->rb_parent = node;
	}
	left->rb_right = node;

	if ((left->rb_parent = node->rb_parent)) {

		if (node == node->rb_parent->rb_right) {

			node->rb_parent->rb_right = left;
		}
		else {
			node->rb_parent->rb_left = left;
		}
	}
	else {
		tree->rb_root = left;
	}
	node->rb_parent = left;
}

static inline void __rbtree_link_node(util_rbtree_node_t* node, util_rbtree_node_t* parent, util_rbtree_node_t** rb_link)
{
	node->rb_parent = parent;
	node->rb_color = RB_RED;
	node->rb_left = node->rb_right = NULL;
	*rb_link = node;
}

static inline void __rbtree_insert_color(util_rbtree_t* tree, util_rbtree_node_t* node)
{
	util_rbtree_node_t* parent, * gparent;

	while ((parent = node->rb_parent) && parent->rb_color == RB_RED)
	{
		gparent = parent->rb_parent;

		if (parent == gparent->rb_left)
		{
			{
				register util_rbtree_node_t* uncle = gparent->rb_right;
				if (uncle && uncle->rb_color == RB_RED)
				{
					uncle->rb_color   = RB_BLACK;
					parent->rb_color  = RB_BLACK;
					gparent->rb_color = RB_RED;
					node = gparent;
					continue;
				}
			}
			if (parent->rb_right == node)
			{
				register util_rbtree_node_t* tmp;
				__rbtree_rotate_left(parent, tree);
				tmp    = parent;
				parent = node;
				node   = tmp;
			}
			parent->rb_color  = RB_BLACK;
			gparent->rb_color = RB_RED;
			__rbtree_rotate_right(gparent, tree);
		}
		else {
			{
				register util_rbtree_node_t* uncle = gparent->rb_left;
				if (uncle && uncle->rb_color == RB_RED)
				{
					uncle->rb_color   = RB_BLACK;
					parent->rb_color  = RB_BLACK;
					gparent->rb_color = RB_RED;
					node = gparent;
					continue;
				}
			}
			if (parent->rb_left == node)
			{
				register util_rbtree_node_t* tmp;
				__rbtree_rotate_right(parent, tree);
				tmp    = parent;
				parent = node;
				node   = tmp;
			}
			parent->rb_color  = RB_BLACK;
			gparent->rb_color = RB_RED;
			__rbtree_rotate_left(gparent, tree);
		}
	}
	tree->rb_root->rb_color = RB_BLACK;
}

static inline void __rbtree_erase_color(util_rbtree_node_t* node, util_rbtree_node_t* parent, util_rbtree_t* tree)
{
	util_rbtree_node_t* other;

	while ((!node || node->rb_color == RB_BLACK) && node != tree->rb_root)
	{
		if (parent->rb_left == node)
		{
			other = parent->rb_right;
			if (other->rb_color == RB_RED)
			{
				other->rb_color  = RB_BLACK;
				parent->rb_color = RB_RED;
				__rbtree_rotate_left(parent, tree);
				other = parent->rb_right;
			}
			if ((!other->rb_left || other->rb_left->rb_color == RB_BLACK)
				&& (!other->rb_right || other->rb_right->rb_color == RB_BLACK))
			{
				other->rb_color = RB_RED;
				node   = parent;
				parent = node->rb_parent;
			}
			else
			{
				if (!other->rb_right || other->rb_right->rb_color == RB_BLACK)
				{
					if (other->rb_left) {
						other->rb_left->rb_color = RB_BLACK;
					}
					other->rb_color = RB_RED;
					__rbtree_rotate_right(other, tree);
					other = parent->rb_right;
				}
				other->rb_color  = parent->rb_color;
				parent->rb_color = RB_BLACK;

				if (other->rb_right) {
					other->rb_right->rb_color = RB_BLACK;
				}
				__rbtree_rotate_left(parent, tree);
				node = tree->rb_root;
				break;
			}
		}
		else
		{
			other = parent->rb_left;

			if (other) {
				if (other->rb_color == RB_RED)
				{
					other->rb_color  = RB_BLACK;
					parent->rb_color = RB_RED;
					__rbtree_rotate_right(parent, tree);
					other = parent->rb_left;
				}
				if ((!other->rb_left || other->rb_left->rb_color == RB_BLACK)
					&& (!other->rb_right || other->rb_right->rb_color == RB_BLACK))
				{
					other->rb_color = RB_RED;
					node   = parent;
					parent = node->rb_parent;
				}
				else
				{
					if (!other->rb_left || other->rb_left->rb_color == RB_BLACK)
					{
						if (other->rb_right) {
							other->rb_right->rb_color = RB_BLACK;
						}
						other->rb_color = RB_RED;
						__rbtree_rotate_left(other, tree);
						other = parent->rb_left;
					}
					other->rb_color  = parent->rb_color;
					parent->rb_color = RB_BLACK;

					if (other->rb_left) {
						other->rb_left->rb_color = RB_BLACK;
					}
					__rbtree_rotate_right(parent, tree);
					node = tree->rb_root;
					break;
				}
			}
		}
	}
	if (node) {
		node->rb_color = RB_BLACK;
	}
}

util_rbtree_key_t util_rbtree_key_str(char *str)
{
    size_t len = strlen(str);
    util_rbtree_key_t key;

    strncpy(key.str, str, UTILS_RBTREE_KEY_STR_MAX);
    key.str[len+1] = '\0';

    return key;
}

util_rbtree_key_t util_rbtree_key_i8(int8_t i8)
{
    util_rbtree_key_t key;
    key.i8 = i8;
    return key;
}

util_rbtree_key_t util_rbtree_key_i16(int16_t i16)
{
    util_rbtree_key_t key;
    key.i16 = i16;
    return key;
}

util_rbtree_key_t util_rbtree_key_i32(int32_t i32)
{
    util_rbtree_key_t key;
    key.i32 = i32;
    return key;
}

util_rbtree_key_t util_rbtree_key_i64(int64_t i64)
{
    util_rbtree_key_t key;
    key.i64 = i64;
    return key;
}

util_rbtree_key_t util_rbtree_key_u8(uint8_t u8)
{
    util_rbtree_key_t key;
    key.u8 = u8;
    return key;
}

util_rbtree_key_t util_rbtree_key_u16(uint16_t u16)
{
    util_rbtree_key_t key;
    key.u16 = u16;
    return key;
}

util_rbtree_key_t util_rbtree_key_u32(uint32_t u32)
{
    util_rbtree_key_t key;
    key.u32 = u32;
    return key;
}

util_rbtree_key_t util_rbtree_key_u64(uint64_t u64)
{
    util_rbtree_key_t key;
    key.u64 = u64;
    return key;
}

util_rbtree_key_t util_rbtree_key_ptr(void *ptr)
{
    util_rbtree_key_t key;
    key.ptr = ptr;
    return key;
}

int util_rbtree_create(util_rbtree_t** tree, util_rbtree_key_type_t type)
{
    util_rbtree_t *temp_tree = NULL;
	int status = 0;

	if (NULL == tree) {
		*tree = NULL;
		return -1;
	}

    temp_tree = (util_rbtree_t*)malloc(sizeof(util_rbtree_t));
    if (NULL != temp_tree) {
        temp_tree->rb_root = NULL;

        switch (type) {
        case UTIL_RBTREE_KEY_TYPE_STR:
            temp_tree->rb_keycmp = keycmp_str;
            break;

        case UTIL_RBTREE_KEY_TYPE_I8:
            temp_tree->rb_keycmp = keycmp_i8;
            break;

        case UTIL_RBTREE_KEY_TYPE_I16:
            temp_tree->rb_keycmp = keycmp_i16;
            break;

        case UTIL_RBTREE_KEY_TYPE_I32:
            temp_tree->rb_keycmp = keycmp_i32;
            break;

        case UTIL_RBTREE_KEY_TYPE_I64:
            temp_tree->rb_keycmp = keycmp_i64;
            break;

        case UTIL_RBTREE_KEY_TYPE_U8:
            temp_tree->rb_keycmp = keycmp_u8;
            break;

        case UTIL_RBTREE_KEY_TYPE_U16:
            temp_tree->rb_keycmp = keycmp_u16;
            break;

        case UTIL_RBTREE_KEY_TYPE_U32:
            temp_tree->rb_keycmp = keycmp_u32;
            break;

        case UTIL_RBTREE_KEY_TYPE_U64:
            temp_tree->rb_keycmp = keycmp_u64;
            break;

        case UTIL_RBTREE_KEY_TYPE_PTR:
            temp_tree->rb_keycmp = keycmp_ptr;
            break;

        default:
            temp_tree->rb_keycmp = NULL;
			status = -1;
			free(temp_tree);
            break;
        }
    }

	*tree = (0 == status)? temp_tree: NULL;

    return status;
}

int util_rbtree_delete(util_rbtree_t** tree)
{
	if (NULL == tree || NULL == *tree) {
		return -1;
	}

	util_rbtree_node_t *node = util_rbtree_first(*tree);
	while (NULL != node) {
		util_rbtree_erase(*tree, node);
		node = util_rbtree_first(*tree);
	}

	util_rbtree_t *temp = *tree;
	*tree = NULL;
	free(temp);

	return 0;
}

void util_rbtree_erase(util_rbtree_t* tree, util_rbtree_node_t* node)
{
	util_rbtree_node_t* child, * parent;
	int color;

	if (NULL == tree || NULL == node) {
		return;
	}

	if (!node->rb_left) {
		child = node->rb_right;
	}
	else if (!node->rb_right)
	{
		child = node->rb_left;
	}
	else
	{
		util_rbtree_node_t* current = node, * left;

		current = current->rb_right;
		while ((left = current->rb_left)) {
			current = left;
		}

		if (node->rb_parent) {
			if (node->rb_parent->rb_left == node) {

				node->rb_parent->rb_left = current;
			}
			else {
				node->rb_parent->rb_right = current;
			}
		}
		else {
			tree->rb_root = current;
		}
		child  = current->rb_right;
		parent = current->rb_parent;
		color  = current->rb_color;

		if (parent == node) {
			parent = current;
		}
		else {
			if (child) {
				child->rb_parent = parent;
			}
			parent->rb_left = child;
			current->rb_right  = node->rb_right;
			node->rb_right->rb_parent = current;
		}
		current->rb_parent = node->rb_parent;
		current->rb_color  = node->rb_color;
		current->rb_left   = node->rb_left;
		node->rb_left->rb_parent = current;

		goto color;
	}

	parent = node->rb_parent;
	color  = node->rb_color;

	if (child)
		child->rb_parent = parent;
	if (parent)
	{
		if (parent->rb_left == node) {
			parent->rb_left = child;
		}
		else {
			parent->rb_right = child;
		}
	}
	else {
		tree->rb_root = child;
	}
color:
	if (color == RB_BLACK) {
		__rbtree_erase_color(child, parent, tree);
	}

	free(node);
}

int util_rbtree_insert(util_rbtree_t* tree, util_rbtree_key_t key, void *data)
{
    util_rbtree_data_t *t_node = NULL;

	if (NULL == tree || NULL == data) {
		return -1;
	}

	t_node = (util_rbtree_data_t*)malloc(sizeof(util_rbtree_data_t));
    if (NULL != t_node) {
        util_rbtree_node_t** p = &(tree->rb_root);
        util_rbtree_node_t* parent = NULL;

        t_node->data = data;
		memcpy(&t_node->node.rb_key.str, &key.str, sizeof(util_rbtree_key_t));

        while (*p) {

            parent = *p;

            int r = tree->rb_keycmp(&t_node->node.rb_key, &parent->rb_key);
            if (r < 0) {
                p = &(*p)->rb_left;
            }
            else if (r > 0) {
                p = &(*p)->rb_right;
            }
            else {
                return -1;
            }
        }
        __rbtree_link_node(&t_node->node, parent, p);
        __rbtree_insert_color(tree, &t_node->node);
    }

	return 0;
}

util_rbtree_node_t* util_rbtree_find(util_rbtree_t* tree, util_rbtree_key_t key)
{
	util_rbtree_node_t* n = tree->rb_root;

	if (NULL == tree) {
		return NULL;
	}

	while (n) {
		int r = tree->rb_keycmp(&key, &n->rb_key);
		if (r < 0) {
			n = n->rb_left;
		}
		else if (r > 0) {
			n = n->rb_right;
		}
		else {
			return n;
		}
	}
	return NULL;
}

util_rbtree_node_t* util_rbtree_first(util_rbtree_t* tree)
{
	util_rbtree_node_t* n;

	if (NULL == tree) {
		return NULL;
	}

	n = tree->rb_root;
	if (!n) {
		return NULL;
	}
	while (n->rb_left) {
		n = n->rb_left;
	}
	return n;
}

util_rbtree_node_t* util_rbtree_last(util_rbtree_t* tree)
{
	util_rbtree_node_t* n;

	if (NULL == tree) {
		return NULL;
	}

	n = tree->rb_root;
	if (!n) {
		return NULL;
	}
	while (n->rb_right) {
		n = n->rb_right;
	}
	return n;
}

util_rbtree_node_t* util_rbtree_next(util_rbtree_node_t* node)
{
	util_rbtree_node_t* parent;

	if (NULL == node || node->rb_parent == node) {
		return NULL;
	}
	/* If we have a right-hand child, go down and then left as far
	   as we can. */
	if (node->rb_right) {
		node = node->rb_right;
		while (node->rb_left) {
			node = node->rb_left;
		}
		return (util_rbtree_node_t*)node;
	}

	/* No right-hand children.  Everything down and left is
	   smaller than us, so any 'next' node must be in the general
	   direction of our parent. Go up the tree; any time the
	   ancestor is a right-hand child of its parent, keep going
	   up. First time it's a left-hand child of its parent, said
	   parent is our 'next' node. */
	while ((parent = node->rb_parent) && node == parent->rb_right) {
		node = parent;
	}
	return parent;
}

util_rbtree_node_t* util_rbtree_prev(util_rbtree_node_t* node)
{
	util_rbtree_node_t* parent;

	if (NULL == node || node->rb_parent == node)
		return NULL;

	/* If we have a left-hand child, go down and then right as far
	   as we can. */
	if (node->rb_left) {
		node = node->rb_left;
		while (node->rb_right) {
			node = node->rb_right;
		}
		return (util_rbtree_node_t*)node;
	}

	/* No left-hand children. Go up till we find an ancestor which
	   is a right-hand child of its parent */
	while ((parent = node->rb_parent) && node == parent->rb_left) {
		node = parent;
	}
	return parent;
}

bool util_rbtree_empty(util_rbtree_t* tree)
{
	if (NULL == tree) {
		return NULL;
	}

	return (NULL == util_rbtree_first(tree));
}

void* util_rbtree_data(util_rbtree_node_t* node)
{
    if (NULL == node) {
        return NULL;
    }

    return util_list_data(node, util_rbtree_data_t, node)->data;
}

int util_rbtree_get_key(util_rbtree_node_t* node, util_rbtree_key_type_t type, void *buf, size_t size)
{
    if (NULL == node || NULL == buf || 0 == size) {
        return -1;
    }

	switch (type) {
	case UTIL_RBTREE_KEY_TYPE_STR:
		if (strlen(node->rb_key.str) > size) {
			return -1;
		}
		strncpy((char*)buf, node->rb_key.str, size);
		break;

	case UTIL_RBTREE_KEY_TYPE_I8:
		*(int8_t*)buf = node->rb_key.i8;
		break;

	case UTIL_RBTREE_KEY_TYPE_I16:
		*(int16_t*)buf = node->rb_key.i16;
		break;

	case UTIL_RBTREE_KEY_TYPE_I32:
		*(int32_t*)buf = node->rb_key.i32;
		break;

	case UTIL_RBTREE_KEY_TYPE_I64:
		*(int64_t*)buf = node->rb_key.i64;
		break;

	case UTIL_RBTREE_KEY_TYPE_U8:
		*(uint8_t*)buf = node->rb_key.u8;
		break;

	case UTIL_RBTREE_KEY_TYPE_U16:
		*(uint16_t*)buf = node->rb_key.u16;
		break;

	case UTIL_RBTREE_KEY_TYPE_U32:
		*(uint32_t*)buf = node->rb_key.u32;
		break;

	case UTIL_RBTREE_KEY_TYPE_U64:
		*(uint64_t*)buf = node->rb_key.u64;
		break;

	case UTIL_RBTREE_KEY_TYPE_PTR:
		buf = node->rb_key.ptr;
		break;

	default:
		break;
	}

	return 0;
}
