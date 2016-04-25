

#ifndef __SB__SLIST__H__INCLUDE__
#define __SB__SLIST__H__INCLUDE__

#include <sb/sb_config.h>

typedef struct sb_slist_node_s {
    struct sb_slist_node_s *next;
}sb_slist_node_t;

typedef sb_slist_node_t* sb_slist_t;
typedef sb_bool_t (*sb_slist_magic_pt)(sb_slist_node_t *);
typedef sb_int_t  (*sb_slist_compare_pt)
    (const sb_slist_node_t*, const sb_slist_node_t*);

static sb_inline 
sb_bool_t sb_slist_is_empty(sb_slist_t *head)
{
    return ((*head) == nullptr);
}

static sb_inline
sb_void_t sb_slist_push_back(sb_slist_t *head, sb_slist_node_t *node)
{
    node->next = *head;
    *head = node;
}

static sb_inline 
sb_void_t sb_slist_pop_back(sb_slist_t *head)
{
    if (*head) {
        *head = (*head)->next;
    }
}

#define sb_slist_init(head)                                                   \
    do {                                                                      \
        ((*(head)) = nullptr);                                                \
    } while (false)

#define sb_slist_element(ptr, type, member)                                   \
    ((type*)(((sb_bytep_t)(ptr)) - (sb_bytep_t)(&(((type*)0)->member))))

#define sb_slist_begin(head)            (*head)

#define sb_slist_end(head)              (nullptr)

#define sb_slist_next(node)             (node)->next

#define sb_slist_for_each(head, iterator)                                     \
    for ((iterator) = sb_slist_begin(head);                                   \
         (iterator) != sb_slist_end(head);                                    \
         (iterator) = sb_slist_next(iterator))

#define sb_slist_safe_for_each(head, iterator, next_iterator)                 \
    for ((iterator) = sb_slist_begin(head), (next_iterator) = (iterator);     \
         (iterator) != sb_slist_end(head);                                    \
         (iterator) = (next_iterator),                                        \
         (next_iterator) = (next_iterator) ? sb_slist_next(next_iterator) : nullptr)

sb_void_t sb_slist_reverse(sb_slist_t *head);
sb_void_t sb_slist_clear(sb_slist_t *head, sb_slist_magic_pt magic);
sb_void_t sb_slist_remove(sb_slist_t *head, sb_slist_node_t *node);
sb_void_t sb_slist_insertion_sort(sb_slist_t *head, sb_slist_compare_pt compare);

#endif
