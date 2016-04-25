

#ifndef __SB__LIST__H__INCLUDE__
#define __SB__LIST__H__INCLUDE__

#include <sb/sb_config.h>

typedef struct sb_list_s {
    struct sb_list_s *prev;
    struct sb_list_s *next;
} sb_list_t;

typedef sb_list_t  sb_list_node_t;
typedef sb_int_t (*sb_list_compare_pt)(const sb_list_node_t*,
    const sb_list_node_t*);
typedef sb_bool_t (*sb_list_magic_pt)(sb_list_node_t*);

static sb_inline
sb_void_t __sb_list_add__(sb_list_node_t *prev, 
    sb_list_node_t *next, sb_list_node_t *node)
{
    next->prev = node;
    node->next = next;
    node->prev = prev;
    prev->next = node;
}

static sb_inline
sb_void_t __sb_list_del__(sb_list_node_t *prev, sb_list_node_t *next)
{
    next->prev = prev;
    prev->next = next;
}

static sb_inline
sb_void_t __sb_list_replace__(sb_list_node_t *o, sb_list_node_t *n)
{
    n->next = o->next;
    n->next->prev = n;
    n->prev = o->prev;
    n->prev->next = n;
}

static sb_inline
sb_void_t __sb_list_splice__(sb_list_node_t *prev, sb_list_node_t *next,
    const sb_list_t *list)
{
    sb_list_node_t *first = list->next;
    sb_list_node_t *last = list->prev;

    first->prev = prev;
    prev->next  = first;

    last->next = next;
    next->prev = last;
}

static sb_inline
sb_void_t __sb_list_slice__(sb_list_t *n, sb_list_t *o, 
    sb_list_t *cut)
{
    sb_list_t *first = cut->next;

    n->next = o->next;
    n->next->prev = n;

    n->prev = cut;
    cut->next = n;

    o->next = first;
    first->prev = o;
}

static sb_inline
sb_void_t sb_list_init(sb_list_t *head)
{
    head->prev = head;
    head->next = head;
}

static sb_inline
sb_bool_t sb_list_is_empty(sb_list_t *head)
{
    sb_list_node_t *next = head->next;
    return (next == head) && (next == head->prev);
}

static sb_inline
sb_bool_t sb_list_is_singular(sb_list_t *head)
{
    return (!sb_list_is_empty(head)) && (head->next == head->prev);
}

static sb_inline
sb_void_t sb_list_add(sb_list_t *head, sb_list_node_t *node)
{
    __sb_list_add__(head, head->next, node);
}

static sb_inline
sb_void_t sb_list_add_tail(sb_list_t *head, sb_list_node_t *node)
{
    __sb_list_add__(head->prev, head, node);
}

static sb_inline
sb_void_t sb_list_del(sb_list_node_t *node)
{
    __sb_list_del__(node->prev, node->next);
    sb_list_init(node);
}

static sb_inline
sb_void_t sb_list_replace(sb_list_node_t *o, sb_list_node_t *n)
{
    __sb_list_replace__(o, n);
    sb_list_init(o);
}

static sb_inline
sb_void_t sb_list_move(sb_list_t *head, sb_list_node_t *node)
{
    __sb_list_del__(node->prev, node->next);
    sb_list_add(head, node);
}

static sb_inline
sb_void_t sb_list_move_tail(sb_list_t *head, sb_list_node_t *node)
{
    __sb_list_del__(node->prev, node->next);
    sb_list_add_tail(head, node);
}

static sb_inline
sb_void_t sb_list_rotate_left(sb_list_node_t *node)
{
    sb_list_node_t *left;

    if (!sb_list_is_empty(node)) {
        left = node->next;
        sb_list_move_tail(node, left);
    }
}

static sb_inline
sb_void_t sb_list_rotate_right(sb_list_node_t *node)
{
    sb_list_node_t *right;

    if (!sb_list_is_empty(node)) {
        right = node->prev;
        sb_list_move_tail(right, node);
    }
}

static sb_inline
sb_void_t sb_list_splice(sb_list_t *list1, sb_list_t *list2)
{
    if (!sb_list_is_empty(list2)) {
        __sb_list_splice__(list1, list1->next, list2);
        sb_list_init(list2);
    }
}

static sb_inline
sb_void_t sb_list_splice_tail(sb_list_t *list1, sb_list_t *list2)
{
    if (!sb_list_is_empty(list2)) {
        __sb_list_splice__(list1->prev, list1, list2);
        sb_list_init(list2);
    }
}

static sb_inline
sb_void_t sb_list_slice(sb_list_t *n, sb_list_t *o, 
    sb_list_node_t *cut)
{
    if (sb_list_is_empty(o)) {
        return;
    }

    if (sb_list_is_singular(o) &&
        (o->next != cut && o != cut)) {
        return;
    }

    if (o == cut) {
        sb_list_init(n);

    } else {
        __sb_list_slice__(n, o, cut);
    }
}

#define sb_list_element(ptr, type, member)                                    \
    ((type*)(((sb_bytep_t)(ptr)) - (sb_bytep_t)(&(((type*)0)->member))))

#define sb_list_for_each(head, iterator)                                      \
    for ((iterator) = (head)->next;                                           \
         (iterator) != (head);                                                \
         (iterator) = (iterator)->next)

#define sb_list_safe_for_each(head, iterator, next_iterator)                  \
    for ((iterator) = (head)->next, (next_iterator) = (iterator)->next;       \
         (iterator) != (head);                                                \
         (iterator) = (next_iterator), (next_iterator) = (iterator)->next)

#define sb_list_reverse_for_each(head, iterator)                              \
    for ((iterator) = (head)->prev;                                           \
         (iterator) != (head);                                                \
         (iterator) = (iterator)->prev)

#define sb_list_safe_reverse_for_each(head, iterator, next_iterator)          \
    for ((iterator) = (head)->prev, (next_iterator) = (iterator)->prev;       \
         (iterator) != (head);                                                \
         (iterator) = (next_iterator), (next_iterator) = (iterator)->prev)

#define sb_list_push_back(head, node)                                         \
    sb_list_add_tail(head, node)

#define sb_list_push_front(head, node)                                        \
    sb_list_add(head, node)

#define sb_list_pop_back(head)                                                \
    sb_list_del((head)->prev);

#define sb_list_pop_front(head)                                               \
    sb_list_del((head)->next)

#define sb_list_begin(head)     ((head)->next)

#define sb_list_rbegin(head)    ((head)->prev)

#define sb_list_end(head)       (head)

#define sb_list_rend(head)      (head)

#define sb_list_next(iterator)  ((iterator)->next)

#define sb_list_rnext(iterator) ((iterator)->prev)

#define sb_list_prev(iterator)  ((iterator)->prev)

#define sb_list_rprev(iterator) ((iterator)->next)

sb_void_t sb_list_reverse(sb_list_t *head);
sb_void_t sb_list_magic(sb_list_t *head, sb_list_magic_pt magic);
sb_void_t sb_list_clear(sb_list_t *head, sb_list_magic_pt magic);
sb_void_t sb_list_insertion_sort(sb_list_t *head, sb_list_compare_pt compare);

#endif
