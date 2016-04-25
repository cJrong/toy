

#ifndef __SB__QUEUE__H__INCLUDE__
#define __SB__QUEUE__H__INCLUDE__

#include <sb/sb_config.h>

typedef struct sb_queue_node_s {
    struct sb_queue_node_s *next;
}sb_queue_node_t;

typedef struct sb_queue_s {
    sb_queue_node_t *tail;
    sb_queue_node_t *head;
}sb_queue_t;

static sb_inline 
sb_void_t sb_queue_enqueue(sb_queue_t *queue, sb_queue_node_t *node)
{
    if (queue->tail) {
        queue->tail->next = node;
    }

    if (!queue->head) {
        queue->head = node;
    }

    node->next = nullptr;
    queue->tail = node;
}

static sb_inline 
sb_queue_node_t *sb_queue_dequeue(sb_queue_t *queue)
{
    sb_queue_node_t* node = queue->head;

    if (node) {
        queue->head = node->next;

        if (!queue->head) {
            queue->tail = nullptr;
        }
    }

    return node;
}

#define sb_queue_init(queue)                                                  \
    do {                                                                      \
        (queue)->head = (queue)->tail = nullptr;                              \
    } while (false)

#define sb_queue_is_empty(queue)                                              \
    ((((queue)->head) == nullptr) && (((queue)->tail) == nullptr))

#define sb_queue_peek(queue)                                                  \
    ((queue)->head)

#define sb_queue_element(ptr, type, member)                                   \
    ((type*)(((sb_bytep_t)(ptr)) - (sb_bytep_t)(&(((type*)0)->member))))

#endif
