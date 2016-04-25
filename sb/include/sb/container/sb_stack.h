

#ifndef __SB__STACK__H__INCLUDE__
#define __SB__STACK__H__INCLUDE__

#include <sb/sb_config.h>

typedef struct sb_stack_node_s {
    struct sb_stack_node_s *next;
}sb_stack_node_t;

typedef struct sb_stack_t {
    sb_stack_node_t *top;
}sb_stack_t;

static sb_inline
sb_void_t sb_stack_push(sb_stack_t *stack, sb_stack_node_t *node)
{
    node->next = stack->top;
    stack->top = node;
}

static sb_inline 
sb_stack_node_t *sb_stack_pop(sb_stack_t *stack)
{
    sb_stack_node_t *node = stack->top;

    if (node) {
        stack->top = node->next;
        node->next = nullptr;
    }

    return node;
}

#define sb_stack_init(stack)                                                  \
    do {                                                                      \
        (stack)->top = nullptr;                                               \
    } while (false)

#define sb_stack_element(ptr, type, member)                                   \
    ((type*)(((sb_bytep_t)(ptr)) - (sb_bytep_t)(&(((type*)0)->member))))

#define sb_stack_is_empty(stack)                                              \
    ((stack)->top == nullptr)

#define sb_stack_peek(stack)                                                  \
    ((stack)->top)

#endif
