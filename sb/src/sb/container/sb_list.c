

#include <sb/sb_config.h>
#include <sb/container/sb_list.h>


sb_void_t sb_list_insertion_sort(sb_list_t *head, sb_list_compare_pt compare)
{
    sb_list_node_t *node;
    sb_list_node_t *prev;

    if (sb_list_is_empty(head) || sb_list_is_singular(head)) {
        return ;
    }

    node = sb_list_begin(head);

    for (node = sb_list_next(node); node != sb_list_end(head); node = sb_list_next(node)) {

        prev = sb_list_prev(node);

        sb_list_del(node);

        do {
            if (compare(node, prev) >= 0) {
                break;
            }

            prev = sb_list_prev(prev);

        } while(prev != sb_list_end(head));

        sb_list_add(prev, node);
    }
}


sb_void_t sb_list_reverse(sb_list_t *head)
{
    sb_list_node_t *current = head, *next = current->next;
    sb_list_node_t *temp;
    
    do {
        
        temp = current->next;
        current->next = current->prev;
        current->prev = temp;
        current = next, next = next->next;

    } while (current != head);
}


sb_void_t sb_list_magic(sb_list_t *head, sb_list_magic_pt magic)
{
    sb_list_node_t *iterator;

    sb_list_for_each(head, iterator) {
        if (!magic(iterator)) {
            break;
        }
    }
}


sb_void_t sb_list_clear(sb_list_t *head, sb_list_magic_pt magic)
{
    sb_list_node_t *node;

    while (!sb_list_is_empty(head)) {

        node = head->next;
        
        sb_list_pop_front(head);
        
        if (magic) {
            if (!magic(node))
                break;
        }
    }
}
