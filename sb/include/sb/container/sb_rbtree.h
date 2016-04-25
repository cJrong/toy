

#ifndef __SB__RBTREE__H__INCLUDE__
#define __SB__RBTREE__H__INCLUDE__

#include <sb/sb_config.h>

typedef enum sb_rbtree_color_e {
    SB_RBT_RED,
    SB_RBT_BLACK
}sb_rbtree_color_t;

typedef struct sb_rbtree_node_s {
    struct sb_rbtree_node_s  *left;
    struct sb_rbtree_node_s  *right;
    struct sb_rbtree_node_s  *parent;
    sb_rbtree_color_t         color;
}sb_rbtree_node_t;

typedef sb_int_t (*sb_rbtree_compare_pt)(const sb_rbtree_node_t*,
    const sb_rbtree_node_t*);
typedef sb_bool_t (*sb_rbtree_magic_pt)(sb_rbtree_node_t*);

typedef struct sb_rbtree_s {
    sb_rbtree_node_t *root;
}sb_rbtree_t;

static sb_inline
sb_void_t sb_rbtree_link_node(sb_rbtree_node_t **link,
    sb_rbtree_node_t *parent, sb_rbtree_node_t *node)
{
    node->left = nullptr;
    node->right = nullptr;
    node->parent = parent;
    node->color = SB_RBT_RED;
    *link = node;
}

static sb_inline
sb_void_t sb_rbtree_replace(sb_rbtree_t *tree, sb_rbtree_node_t *o, 
    sb_rbtree_node_t *n)
{
    sb_rbtree_node_t *parent = o->parent;

    if (parent) {
        if (parent->left == o) {
            parent->left = n;

        } else {
            parent->right = n;
        }

    } else {
        tree->root = n;
    }

    if (o->left) {
        o->left->parent = n;
    }

    if (o->right) {
        o->right->parent = n;
    }

    *n = *o;
}

#define sb_rbtree_init(tree)                                                  \
    do {                                                                      \
        ((tree)->root = nullptr);                                             \
    } while (false)

#define sb_rbtree_is_empty(tree)                                              \
    (!(tree)->root)

#define sb_rbtree_element(ptr, type, member)                                  \
    ((type*)(((sb_bytep_t)(ptr)) - (sb_bytep_t)(&(((type*)0)->member))))

#define sb_rbtree_for_each(tree, iterator)                                    \
    for ((iterator) = sb_rbtree_begin(tree);                                  \
         (iterator);                                                          \
         (iterator) = sb_rbtree_next((iterator)))

#define sb_rbtree_reverse_for_each(tree, iterator)                            \
    for ((iterator) = sb_rbtree_rbegin(tree);                                 \
         (iterator);                                                          \
         (iterator) = sb_rbtree_rnext((iterator)))

#define sb_rbtree_safe_for_each(tree, iterator, next_iterator)                \
    for ((iterator) = sb_rbtree_begin(tree),                                  \
         (next_iterator) = sb_rbtree_next((iterator));                        \
         (iterator);                                                          \
         (iterator) = (next_iterator),                                        \
         (next_iterator) =                                                    \
         (next_iterator) ? sb_rbtree_next((next_iterator)) : nullptr)

#define sb_rbtree_safe_reverse_for_each(tree, iterator, next_iterator)        \
    for ((iterator) = sb_rbtree_rbegin(tree),                                 \
         (next_iterator) = sb_rbtree_rnext((iterator));                       \
         (iterator);                                                          \
         (iterator) = (next_iterator),                                        \
         (next_iterator) =                                                    \
         (next_iterator) ? sb_rbtree_rnext((next_iterator)) : nullptr)

#define sb_rbtree_end(tree)     nullptr

#define sb_rbtree_rend(tree)    nullptr

sb_bool_t sb_rbtree_insert_if(sb_rbtree_t *tree,
    sb_rbtree_node_t *node, sb_rbtree_compare_pt compare);
sb_rbtree_node_t *sb_rbtree_update_if(sb_rbtree_t *tree,
    sb_rbtree_node_t *node, sb_rbtree_compare_pt compare);
sb_rbtree_node_t *sb_rbtree_remove_if(sb_rbtree_t *tree, 
    sb_rbtree_node_t *node, sb_rbtree_compare_pt compare);
sb_rbtree_node_t *sb_rbtree_search_if(sb_rbtree_t *tree, 
    sb_rbtree_node_t *node, sb_rbtree_compare_pt compare);
sb_bool_t sb_rbtree_is_same_tree(sb_rbtree_node_t *lhs, sb_rbtree_node_t *rhs,
    sb_rbtree_compare_pt compare);
sb_void_t sb_rbtree_insert(sb_rbtree_t *tree, sb_rbtree_node_t *node);
sb_void_t sb_rbtree_remove(sb_rbtree_t *tree, sb_rbtree_node_t *node);
sb_void_t sb_rbtree_clear(sb_rbtree_t *tree, sb_rbtree_magic_pt magic);
sb_void_t sb_rbtree_magic(sb_rbtree_t *tree, sb_rbtree_magic_pt magic);
sb_rbtree_node_t *sb_rbtree_begin(sb_rbtree_t *tree);
sb_rbtree_node_t *sb_rbtree_rbegin(sb_rbtree_t *tree);
sb_rbtree_node_t *sb_rbtree_next(sb_rbtree_node_t *node);
sb_rbtree_node_t *sb_rbtree_rnext(sb_rbtree_node_t *node);

#endif
