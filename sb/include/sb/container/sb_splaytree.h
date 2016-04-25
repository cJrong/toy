

#ifndef __SB__SPLAYTREE__H__INCLUDE__
#define __SB__SPLAYTREE__H__INCLUDE__

#include <sb/sb_config.h>

typedef struct sb_splaytree_node_s {
    struct sb_splaytree_node_s *left;
    struct sb_splaytree_node_s *right;
    struct sb_splaytree_node_s *parent;
}sb_splaytree_node_t;

typedef sb_int_t (*sb_splaytree_compare_pt)
    (const sb_splaytree_node_t *, const sb_splaytree_node_t *);
typedef sb_bool_t (*sb_splaytree_magic_pt)(sb_splaytree_node_t*);

typedef struct sb_splaytree_s {
    sb_splaytree_node_t *root;
}sb_splaytree_t;

static sb_inline
sb_void_t sb_splaytree_link_node(sb_splaytree_node_t **link,
    sb_splaytree_node_t *parent, sb_splaytree_node_t *node)
{
    node->left = nullptr;
    node->right = nullptr;
    node->parent = parent;
    *link = node;
}

static sb_inline
sb_void_t sb_splaytree_replace(sb_splaytree_t *tree,
    sb_splaytree_node_t *o, sb_splaytree_node_t *n)
{
    sb_splaytree_node_t *parent = o->parent;

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

static sb_inline
sb_bool_t sb_splaytree_is_empty(sb_splaytree_t *tree)
{
    return (!tree->root);
}

#define sb_splaytree_init(tree)                                               \
    do {                                                                      \
        ((tree)->root = nullptr);                                             \
    } while (false)

#define sb_splaytree_element(ptr, type, member)                               \
    ((type*)(((sb_bytep_t)(ptr)) - (sb_bytep_t)(&(((type*)0)->member))))

#define sb_splaytree_for_each(tree, iterator)                                 \
    for ((iterator) = sb_splaytree_begin(tree);                               \
         (iterator);                                                          \
         (iterator) = sb_splaytree_next((iterator)))

#define sb_splaytree_reverse_for_each(tree, iterator)                         \
    for ((iterator) = sb_splaytree_rbegin(tree);                              \
         (iterator);                                                          \
         (iterator) = sb_splaytree_rnext((iterator)))

#define sb_splaytree_safe_for_each(tree, iterator, next_iterator)             \
    for ((iterator) = sb_splaytree_begin(tree),                               \
         (next_iterator) = sb_splaytree_next((iterator));                     \
         (iterator);                                                          \
         (iterator) = (next_iterator),                                        \
         (next_iterator) =                                                    \
         (next_iterator) ? sb_splaytree_next((next_iterator)) : nullptr)

#define sb_splaytree_safe_reverse_for_each(tree, iterator, next_iterator)     \
    for ((iterator) = sb_splaytree_rbegin(tree),                              \
         (next_iterator) = sb_splaytree_rnext((iterator));                    \
         (iterator);                                                          \
         (iterator) = (next_iterator),                                        \
         (next_iterator) =                                                    \
         (next_iterator) ? sb_splaytree_rnext((next_iterator)) : nullptr)

#define sb_splaytree_end(tree)      (nullptr)

#define sb_splaytree_rend(tree)     (nullptr)

sb_bool_t sb_splaytree_insert_if(sb_splaytree_t *tree,
    sb_splaytree_node_t *node, sb_splaytree_compare_pt compare);
sb_splaytree_node_t *sb_splaytree_update_if(sb_splaytree_t *tree,
    sb_splaytree_node_t *node, sb_splaytree_compare_pt compare);
sb_splaytree_node_t *sb_splaytree_remove_if(sb_splaytree_t *tree,
    sb_splaytree_node_t *node, sb_splaytree_compare_pt compare);
sb_splaytree_node_t *sb_splaytree_search_if(sb_splaytree_t *tree,
    sb_splaytree_node_t *node, sb_splaytree_compare_pt compare);
sb_bool_t sb_splaytree_is_same_tree(sb_splaytree_node_t *p,
    sb_splaytree_node_t *q, sb_splaytree_compare_pt compare);
sb_void_t sb_splaytree_splay(sb_splaytree_t *tree, sb_splaytree_node_t *node);
sb_void_t sb_splaytree_remove(sb_splaytree_t *tree, sb_splaytree_node_t *node);
sb_void_t sb_splaytree_clear(sb_splaytree_t *tree, sb_splaytree_magic_pt magic);
sb_void_t sb_splaytree_magic(sb_splaytree_t *tree, sb_splaytree_magic_pt magic);
sb_splaytree_node_t *sb_splaytree_begin(sb_splaytree_t *tree);
sb_splaytree_node_t *sb_splaytree_rbegin(sb_splaytree_t *tree);
sb_splaytree_node_t *sb_splaytree_next(sb_splaytree_node_t *node);
sb_splaytree_node_t *sb_splaytree_rnext(sb_splaytree_node_t *node);

#endif
