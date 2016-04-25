

#include <sb/container/sb_list.h>
#include <sb/io/sb_console.h>
#include <sb/math/sb_random.h>
#include <sb/os/sb_time.h>

typedef struct sb_node_s {
    sb_int_t key;
    sb_list_node_t node;
}sb_node_t;

sb_node_t nodes[1024];

sb_int_t compare(const sb_list_node_t *lhs, const sb_list_node_t *rhs)
{
    sb_node_t *p = sb_list_element(lhs, sb_node_t, node);
    sb_node_t *q = sb_list_element(rhs, sb_node_t, node);

    return p->key - q->key;
}
sb_void_t sb_list_qsort(sb_list_t *head, sb_list_compare_pt compare);
int main(void)
{
    sb_list_t list;
    sb_int_t i;
    sb_list_node_t* iterator;
    sb_list_node_t* cut;
    sb_list_t newlist;
    sb_list_t *p;

    sb_list_init(&list);
    sb_list_init(&newlist);

    sb_srand(sb_time(nullptr));

    for (i = 0; i < 10; i++) {
        nodes[i].key = sb_rand()%1024;
        sb_list_push_back(&list, &nodes[i].node);
    }

    sb_list_for_each(&list, iterator) {
        sb_node_t *p = sb_list_element(iterator, sb_node_t, node);
        sb_console_fmt("%d ", p->key);
    }
    sb_console_fmt("\n");
    
    sb_list_insertion_sort(&list, compare);
    /*
    for (i = 100; i < 200; i++) {
        nodes[i].key = i;
        sb_list_push_back(&newlist, &nodes[i].node);
    }

   
    sb_list_rotate_right(&nodes[98].node);*/
   // sb_list_rotate_left(&nodes[98].node);

    //sb_list_splice(&newlist, &list);
    //sb_list_insertion_sort(&list, compare);
    //sb_list_slice(&newlist, &list, &nodes[0].node);

    /*
    sb_list_reverse_for_each(&list, iterator) {
        sb_node_t *p = sb_list_element(iterator, sb_node_t, node);
        sb_console_fmt("%d\n", p->key);
    }
    */
    sb_list_for_each(&list, iterator) {
        sb_node_t *p = sb_list_element(iterator, sb_node_t, node);
        sb_console_fmt("%d ", p->key);
    }
    sb_console_fmt("\n");
}