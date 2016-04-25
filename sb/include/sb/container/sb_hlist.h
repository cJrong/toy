

#ifndef __SB__HLIST__H__INCLUDE__H__
#define __SB__HLIST__H__INCLUDE__H__

#include <sb/sb_config.h>

typedef struct sb_hlist_node_s {
    struct sb_hlist_node_s *next;
    struct sb_hlist_node_s **pprev;
}sb_hlist_node_t;

typedef struct sb_hlist_s {
    struct sb_hlist_node_s *first;
}sb_hlist_t;

static sb_inline
sb_bool_t sb_hlist_is_empty(sb_hlist_t *hlist)
{
    return (!hlist->first);
}

static sb_inline
sb_void_t sb_hlist_remove(sb_hlist_node_t *node)
{
    sb_hlist_node_t *next = node->next;
    sb_hlist_node_t **pprev = node->pprev;

    *pprev = next;
    if (next) {
        next->pprev = pprev;
    }
}

static sb_inline 
sb_void_t sb_hlist_replace(sb_hlist_node_t *o, sb_hlist_node_t *n)
{
    n->next = o->next;
    n->pprev = o->pprev;
    o->next->pprev = &n->next;
    *(n->pprev) = n;
}

static sb_inline
sb_void_t sb_hlist_insert(sb_hlist_t *hlist, sb_hlist_node_t *node)
{
    sb_hlist_node_t *first = hlist->first;

    node->next = first;
    if (first) {
        first->pprev = &node->next;
    }
    hlist->first = node;
    node->pprev = &hlist->first;
}

static sb_inline
sb_void_t sb_hlist_insert_front(sb_hlist_node_t *next, sb_hlist_node_t *node)
{
    node->next = next;
    node->pprev = next->pprev;
    next->pprev = &node->next;
    *(node->pprev) = node;
}

static sb_inline
sb_void_t sb_hlist_insert_back(sb_hlist_node_t *prev, sb_hlist_node_t *node)
{
    node->next = prev->next;
    prev->next = node;
    node->pprev = &prev->next;

    if (node->next) {
        node->next->pprev = &node->next;
    }
}

static sb_inline
sb_void_t sb_hlist_move(sb_hlist_t *o, sb_hlist_t *n)
{
    n->first = o->first;
    if (n->first) {
        n->first->pprev = &n->first;
    }
    o->first = nullptr;
}

#define sb_hlist_init(hlist)                                                  \
    do {                                                                      \
        (hlist)->first = nullptr;                                             \
    } while (false)

#define sb_hlist_element(ptr, type, member)                                   \
    ((type*)(((sb_bytep_t)(ptr)) - (sb_bytep_t)(&(((type*)0)->member))))

#define sb_hlist_for_each(hlist, iterator)                                    \
    for ((iterator) = (hlist)->first; (iterator); (iterator)=(iterator)->next)

#define sb_hlist_safe_for_each(hlist, iterator, next_iterator)                \
    for ((iterator) = (hlist)->first, (next_iterator) = (iterator)->next;     \
         (iterator);                                                          \
         (iterator) = (next_iterator),                                        \
         (next_iterator) = (next_iterator) ? (next_iterator)->next : nullptr)

#endif
