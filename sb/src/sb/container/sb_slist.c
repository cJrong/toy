

#include <sb/sb_config.h>
#include <sb/container/sb_slist.h>


sb_void_t sb_slist_insertion_sort(sb_slist_t *head, sb_slist_compare_pt compare)
{
    sb_slist_node_t **current = head;

    while (*current) {
        sb_slist_node_t **next = &(*current)->next;

        while (*next) {
            sb_slist_node_t *entry = *next;

            if (compare(*current, entry) > 0) {
                sb_slist_node_t *insertion = entry;
                *next = entry->next;
                insertion->next = (*current);
                *current = insertion;

            } else {
                next = &(*next)->next;
            }
        }

        current = &(*current)->next;
    }
}


sb_void_t sb_slist_reverse(sb_slist_t *head)
{
    sb_slist_node_t *newhead = nullptr;
    sb_slist_node_t *node;

    while (!sb_slist_is_empty(head)) {
        node = sb_slist_begin(head);
        sb_slist_pop_back(head);
        node->next = newhead;
        newhead = node;
    }

    *head = newhead;
}


sb_void_t sb_slist_remove(sb_slist_t *head, sb_slist_node_t *node)
{
    sb_slist_node_t **current = head;

    while (*current) {
        sb_slist_node_t *entry = *current;
        if (entry == node) {
            *current = entry->next;
        }
        current = &entry->next;
    }
}


sb_void_t sb_slist_clear(sb_slist_t *head, sb_slist_magic_pt magic)
{
    sb_slist_node_t *current = *head;

    while (current) {
        *head = current->next;

        if (magic) {
            if (!magic(current)) {
                break;
            }
        }

        current = current->next;
    }
}
