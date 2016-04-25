

#ifndef __SB__AVLTREE__H__INCLUDE__
#define __SB__AVLTREE__H__INCLUDE__

#include <sb/sb_config.h>

typedef struct sb_avltree_node_s {
    struct sb_avltree_node_s *left;
    struct sb_avltree_node_s *right;
    struct sb_avltree_node_s *parent;
    sb_int_t                  height;
}sb_avltree_node_t;

typedef sb_int_t (*sb_avltree_compare_pt)(
    const sb_avltree_node_t*, const sb_avltree_node_t*);
typedef sb_bool_t (*sb_avltree_magic_pt)(sb_avltree_node_t*);

typedef struct sb_avltree_s {
    sb_avltree_node_t *root;
}sb_avltree_t;

static sb_inline
sb_void_t sb_avltree_link_node(sb_avltree_node_t **link,
    sb_avltree_node_t *parent, sb_avltree_node_t *node)
{
    node->left   = nullptr;
    node->right  = nullptr;
    node->parent = parent;
    node->height = 1;
    *link        = node;
}

static sb_inline
sb_void_t sb_avltree_replace(sb_avltree_t *tree, sb_avltree_node_t *o,
    sb_avltree_node_t *n)
{
    sb_avltree_node_t *parent = o->parent;

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

#define sb_avltree_init(tree)                                                 \
    do {                                                                      \
        ((tree)->root = nullptr);                                             \
    } while (false)

#define sb_avltree_is_empty(tree)                                             \
    (!(tree)->root)

#define sb_avltree_element(ptr, type, member)                                 \
    ((type*)(((sb_bytep_t)(ptr)) - (sb_bytep_t)(&(((type*)0)->member))))

#define sb_avltree_for_each(tree, iterator)                                   \
    for ((iterator) = sb_avltree_begin(tree);                                 \
         (iterator);                                                          \
         (iterator) = sb_avltree_next((iterator)))

#define sb_avltree_reverse_for_each(tree, iterator)                           \
    for ((iterator) = sb_avltree_rbegin(tree);                                \
         (iterator);                                                          \
         (iterator) = sb_avltree_rnext(iterator))

#define sb_avltree_safe_for_each(tree, iterator, next_iterator)               \
    for ((iterator) = sb_avltree_begin(tree),                                 \
         (next_iterator) = sb_avltree_next((iterator));                       \
         (iterator);                                                          \
         (iterator) = (next_iterator),                                        \
         (next_iterator) =                                                    \
         (next_iterator) ? sb_avltree_next((next_iterator)) : nullptr)

#define sb_avltree_safe_reverse_for_each(tree, iterator, next_iterator)       \
    for ((iterator) = sb_avltree_rbegin(tree),                                \
         (next_iterator) = sb_avltree_rnext((iterator));                      \
         (iterator);                                                          \
         (iterator) = (next_iterator),                                        \
         (next_iterator) =                                                    \
         (next_iterator) ? sb_avltree_rnext((next_iterator)) : nullptr)

#define sb_avltree_end(tree)        nullptr

#define sb_avltree_rend(tree)       nullptr

sb_bool_t sb_avltree_insert_if(sb_avltree_t *tree,
    sb_avltree_node_t *node, sb_avltree_compare_pt compare);
sb_avltree_node_t *sb_avltree_update_if(sb_avltree_t *tree,
    sb_avltree_node_t *node, sb_avltree_compare_pt compare);
sb_avltree_node_t *sb_avltree_remove_if(sb_avltree_t *tree, 
    sb_avltree_node_t *node, sb_avltree_compare_pt compare);
sb_avltree_node_t *sb_avltree_search_if(sb_avltree_t *tree, 
    sb_avltree_node_t *node, sb_avltree_compare_pt compare);
sb_bool_t sb_avltree_is_same_tree(sb_avltree_node_t *lhs,
    sb_avltree_node_t *rhs, sb_avltree_compare_pt compare);
sb_void_t sb_avltree_insert(sb_avltree_t *tree, sb_avltree_node_t *node);
sb_void_t sb_avltree_remove(sb_avltree_t *tree, sb_avltree_node_t *node);
sb_void_t sb_avltree_magic(sb_avltree_t *tree, sb_avltree_magic_pt magic);
sb_void_t sb_avltree_clear(sb_avltree_t *tree, sb_avltree_magic_pt magic);
sb_avltree_node_t *sb_avltree_begin(sb_avltree_t *tree);
sb_avltree_node_t *sb_avltree_rbegin(sb_avltree_t *tree);
sb_avltree_node_t *sb_avltree_next(sb_avltree_node_t *iterator);
sb_avltree_node_t *sb_avltree_rnext(sb_avltree_node_t *iterator);

#endif
