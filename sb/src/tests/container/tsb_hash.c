

#include <sb/container/sb_hash.h>
#include <sb/container/sb_hash_func.h>
#include <sb/memory/sb_pool.h>
#include <sb/io/sb_console.h>
#include <time.h>

typedef struct sb_node_s {
    sb_int_t key;
    sb_int_t value;
    sb_hlist_node_t hnode;
}sb_node_t;

sb_uint_t hashfn(const sb_hlist_node_t *node)
{
    sb_node_t *p = sb_hlist_element(node, sb_node_t, hnode);
    return sb_thomas_wangs_hash_32(p->key);
    return p->key;
}

sb_int_t compare(const sb_hlist_node_t *lhs, const sb_hlist_node_t *rhs)
{
    sb_node_t *l = sb_hlist_element(lhs, sb_node_t, hnode);
    sb_node_t *r = sb_hlist_element(rhs, sb_node_t, hnode);
    return l->key - r->key;
}

sb_void_t destructor(sb_hlist_node_t *node)
{
    (void)node;
    /*static int i = 0;
    sb_node_t *n = sb_hlist_element(node, sb_node_t, hnode);*/
}

sb_hash_ops_t hash_ops = {
    hashfn,
    compare,
    nullptr,
    destructor,
};

sb_node_t nodes[10240000];

int main(void)
{
    sb_int_t i;
    sb_hash_t *hash;
    sb_hash_iterator_t begin, end;
    clock_t start;
    hash = sb_hash_create(&hash_ops, 4);


    for (i = 0; i < 10240; i++) {
        nodes[i].key = i;
        nodes[i].value = i * i;
        sb_hash_update(hash, &nodes[i].hnode);
    }


    sb_hash_shrink(hash);

    while (sb_hash_is_rehashing(hash)) {
        sb_hash_rehash(hash, 1);
    }

    begin = sb_hash_iterator_create(hash, true);
    end = sb_hash_iterator_create(hash, false);

    i = 0;
    sb_hash_for_each(hash, begin) {
        sb_node_t *p = sb_hash_iterator_element(begin, sb_node_t, hnode);
        if (p->key == 953) {
            break;
        }
        sb_console_fmt("%d, key:%d, value:%d\n", i++, p->key, p->value);
    }

    i = 0;
    start = clock();
    sb_hash_for_each(hash, begin) {
        sb_node_t *p = sb_hash_iterator_element(begin, sb_node_t, hnode);
        sb_console_fmt("%d, key:%d, value:%d\n", i++, p->key, p->value);
    }

    printf("%d\n", clock() - start);

    sb_hash_iterator_destroy(begin);
    sb_hash_iterator_destroy(end);

    sb_hash_destroy(hash);
    return 0;
}
