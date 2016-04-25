

#include <sb/container/sb_slist.h>
#include <sb/io/sb_console.h>
#include <sb/os/sb_time.h>
#include <sb/math/sb_random.h>

typedef struct {
    sb_int_t key;
    sb_slist_node_t node;
}sb_node_t;


sb_int_t compare(const sb_slist_node_t *lhs, const sb_slist_node_t *rhs)
{
    sb_node_t *p = sb_slist_element(lhs, sb_node_t, node);
    sb_node_t *q = sb_slist_element(rhs, sb_node_t, node);

    return p->key - q->key;
}

sb_node_t nodes[1024];

int main(void)
{
    sb_int_t i;
    sb_slist_t list;
    sb_slist_node_t *iter;

    sb_srand(sb_time(nullptr));
    sb_slist_init(&list);

    for (i = 0; i < 1024; i++) {
        nodes[i].key = sb_rand() % 1024;
        sb_slist_push_back(&list, &nodes[i].node);
    }

    sb_slist_for_each(&list, iter) {
        sb_node_t *p = sb_slist_element(iter, sb_node_t, node);
        sb_console_fmt("%d ", p->key);
    }

    sb_slist_insertion_sort(&list, compare);
    sb_console_fmt("\n");
    sb_slist_for_each(&list, iter) {
        sb_node_t *p = sb_slist_element(iter, sb_node_t, node);
        sb_console_fmt("%d ", p->key);
    }

    return 0;
}