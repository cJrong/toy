

#include <sb/os/sb_time.h>
#include <sb/io/sb_console.h>
#include <sb/math/sb_random.h>
#include <sb/memory/sb_pool.h>
#include <sb/container/sb_avltree.h>


typedef struct {
    sb_int_t            key;
    sb_int_t            value;
    sb_avltree_node_t   node;
}sb_test_avltree_t;

sb_pool_t* pool;

static sb_int_t compare(const sb_avltree_node_t* lhs, const sb_avltree_node_t* rhs)
{
    sb_test_avltree_t *left = sb_avltree_element(lhs, sb_test_avltree_t, node);
    sb_test_avltree_t *right = sb_avltree_element(rhs, sb_test_avltree_t, node);
    return left->key - right->key;
}

static sb_bool_t clear(sb_avltree_node_t* node) 
{
    sb_pool_free(pool, sb_avltree_element(node, sb_test_avltree_t, node), sizeof(sb_test_avltree_t));
    return true;
}

static sb_bool_t magic(sb_avltree_node_t* node)
{
    sb_test_avltree_t *v = sb_avltree_element(node, sb_test_avltree_t, node);
    v->value = -1;
    return true;
}

static sb_void_t sb_test_avltree_insert(sb_avltree_t *tree, sb_test_avltree_t *node)
{
    sb_avltree_node_t** current = &tree->root, *parent = nullptr;

    while (*current) {
        sb_int_t compare;
        parent = *current;
       
        compare = node->key - sb_avltree_element(parent, sb_test_avltree_t, node)->key;

        if (compare == 0) {
            return;
        }
        
        current = compare < 0 ? &parent->left : &parent->right;
    }

    sb_avltree_link_node(current, parent, &node->node);
    sb_avltree_insert(tree, &node->node);
}

static sb_void_t sb_test_avltree_remove(sb_avltree_t* tree, sb_int_t key)
{
    sb_avltree_node_t* erase = tree->root;
    sb_test_avltree_t* current;

    while (erase) {
        current = sb_avltree_element(erase, sb_test_avltree_t, node);

        if (current->key == key) {
            break;
        }

        erase = current->key > key ? erase->left : erase->right;
    }

    if (!erase) {
        return;
    }

    sb_avltree_remove(tree, erase);

}




int main(void)
{
    sb_avltree_t        tree;
    sb_test_avltree_t   *p;
    sb_test_avltree_t   *old;
    sb_avltree_node_t   *it;
    sb_avltree_node_t   *next_it;
    
    /*sb_srand((sb_uint_t)sb_time((sb_time_t*)nullptr));*/

    pool = sb_pool_default();
    sb_avltree_init(&tree);

    {
        sb_console_fmt("%s\n", sb_avltree_is_empty(&tree) ? "true" : "false");
    }

    {
        sb_int_t i;
       
        for (i = 0; i < 100; i++) {
            p = (sb_test_avltree_t*) sb_pool_alloc(pool, sizeof(sb_test_avltree_t));
            p->key = sb_rand() % 1000;
            p->value = i;
            sb_avltree_insert_if(&tree, &p->node, compare);
        }

        sb_console_fmt("%s\n", sb_avltree_is_empty(&tree) ? "true" : "false");

        i = 0;
        sb_avltree_for_each(&tree, it) {
            p = sb_avltree_element(it, sb_test_avltree_t, node);
            sb_console_fmt("index = %d, key = %d, value = %d\n", ++i, p->key, p->value);
        }

        i = 0;
        sb_avltree_reverse_for_each(&tree, it) {
            p = sb_avltree_element(it, sb_test_avltree_t, node);
            sb_console_fmt("index = %d, key = %d, value = %d\n", ++i, p->key, p->value);
        }
      
        sb_avltree_safe_for_each(&tree, it, next_it) {
            p = sb_avltree_element(it, sb_test_avltree_t, node);
            if (p->value == 40) {
                sb_avltree_remove(&tree, &p->node);
                sb_pool_free(pool, p, sizeof(sb_test_avltree_t));
            }
        }

        i = 0;
        sb_avltree_safe_for_each(&tree, it, next_it) {
            p = sb_avltree_element(it, sb_test_avltree_t, node);
            sb_console_fmt("index = %d, key = %d, value = %d\n", ++i, p->key, p->value);
        }

        sb_avltree_safe_reverse_for_each(&tree, it, next_it) {
            p = sb_avltree_element(it, sb_test_avltree_t, node);
            if (p->value == 20) {
                sb_avltree_remove(&tree, &p->node);
                sb_pool_free(pool, p, sizeof(sb_test_avltree_t));
            }
        }

        i = 0;
        sb_avltree_safe_reverse_for_each(&tree, it, next_it) {
            p = sb_avltree_element(it, sb_test_avltree_t, node);
            sb_console_fmt("index = %d, key = %d, value = %d\n", ++i, p->key, p->value);
        }

        sb_avltree_clear(&tree, clear);

        sb_console_fmt("%s\n", sb_avltree_is_empty(&tree) ? "true" : "false");

        for (it = sb_avltree_begin(&tree); it != sb_avltree_end(&tree); it = sb_avltree_next(it)) {
            p = sb_avltree_element(it, sb_test_avltree_t, node);
            sb_console_fmt("key = %d, value = %d\n", p->key, p->value);
        }
    }

    {
        sb_int_t i;
        sb_int_t count = 0;

        for (i = 0; i < 100; i++) {
            p = (sb_test_avltree_t*)sb_pool_alloc(pool, sizeof(sb_test_avltree_t));
            p->key = sb_rand() % 1000;
            p->value = i;
            sb_avltree_insert_if(&tree, &p->node, compare);
            count++;
        }

        sb_console_fmt("insert count: %d\n", count);

        i = 0;
        sb_avltree_for_each(&tree, it) {
            p = sb_avltree_element(it, sb_test_avltree_t, node);
            sb_console_fmt("index = %d, key = %d, value = %d\n", ++i, p->key, p->value);
        }

        it = sb_avltree_begin(&tree);
        p = sb_avltree_element(it, sb_test_avltree_t, node);
        sb_console_fmt("get max: key= %d, value= %d\n",  p->key, p->value);

        it = sb_avltree_rbegin(&tree);
        p = sb_avltree_element(it, sb_test_avltree_t, node);
        sb_console_fmt("get min: key= %d, value= %d\n", p->key, p->value);

        sb_avltree_magic(&tree, magic);

        i = 0;
        sb_avltree_for_each(&tree, it) {
            p = sb_avltree_element(it, sb_test_avltree_t, node);
            sb_console_fmt("index = %d, key = %d, value = %d\n", ++i, p->key, p->value);
        }

        for (it = sb_avltree_rbegin(&tree); it != sb_avltree_rend(&tree); it = sb_avltree_rnext(it)) {
            p = sb_avltree_element(it, sb_test_avltree_t, node);
            sb_console_fmt("key = %d, value = %d\n", p->key, p->value);
        }

        sb_avltree_clear(&tree, clear);
    }

    {
        sb_avltree_t ltree;
        sb_avltree_t rtree;
        sb_int_t i;

        sb_avltree_init(&ltree);
        sb_avltree_init(&rtree);

       
        for (i = 0; i < 100; i++) {
            p = (sb_test_avltree_t*)sb_pool_alloc(pool, sizeof(sb_test_avltree_t));
            p->key = sb_rand() % 10000;
            p->value = sb_rand() % 100;
            sb_avltree_insert_if(&ltree, &p->node, compare);
        }

        for (i = 0; i < 100; i++) {
            p = (sb_test_avltree_t*)sb_pool_alloc(pool, sizeof(sb_test_avltree_t));
            p->key = sb_rand() % 10000;
            p->value = sb_rand() % 100;
            sb_avltree_insert_if(&rtree, &p->node, compare);
        }

        sb_console_fmt("same tree: %s\n", sb_avltree_is_same_tree(ltree.root, rtree.root, compare) ? "true" : "false");

        sb_avltree_clear(&rtree, clear);
        
        sb_avltree_for_each(&ltree, it) {
            p = (sb_test_avltree_t*)sb_pool_alloc(pool, sizeof(sb_test_avltree_t));
            p->key = sb_avltree_element(it, sb_test_avltree_t, node)->key;
            p->value = sb_avltree_element(it, sb_test_avltree_t, node)->value;
            sb_avltree_insert_if(&rtree, &p->node, compare);
        }

        sb_console_fmt("same tree: %s\n", sb_avltree_is_same_tree(ltree.root, rtree.root, compare) ? "true" : "false");

        sb_avltree_clear(&ltree, clear);
        sb_avltree_clear(&rtree, clear);

       
        for (i = 0; i < 100; i++) {
            sb_test_avltree_t *l;
            sb_test_avltree_t *r;
            l = (sb_test_avltree_t*)sb_pool_alloc(pool, sizeof(sb_test_avltree_t));
            r = (sb_test_avltree_t*)sb_pool_alloc(pool, sizeof(sb_test_avltree_t));
            r->key = l->key = sb_rand() % 10000;
            r->value = l->value = sb_rand() % 100;
            sb_avltree_insert_if(&ltree, &l->node, compare);
            sb_avltree_insert_if(&rtree, &r->node, compare);
        }

        sb_console_fmt("same tree: %s\n", sb_avltree_is_same_tree(ltree.root, rtree.root, compare) ? "true" : "false");

        sb_avltree_clear(&ltree, clear);
        sb_avltree_clear(&rtree, clear);
    }

    {
        sb_int_t i;
        sb_test_avltree_t search_node;
        
        p = (sb_test_avltree_t*)sb_pool_alloc(pool, sizeof(sb_test_avltree_t));
        p->key = 520;
        p->value = sb_rand() % 100;
        sb_test_avltree_insert(&tree, p);

        for (i = 0; i < 100; i++) {
            p = (sb_test_avltree_t*)sb_pool_alloc(pool, sizeof(sb_test_avltree_t));
            p->key = sb_rand() % 1000;
            p->value = sb_rand() % 100;
            sb_test_avltree_insert(&tree, p);
        }

        i = 0;
        sb_avltree_for_each(&tree, it) {
            p = sb_avltree_element(it, sb_test_avltree_t, node);
            sb_console_fmt("index = %d, key = %d, value = %d\n", ++i, p->key, p->value);
        }

        search_node.key = 520;
        it = sb_avltree_search_if(&tree, &search_node.node, compare);
        old = sb_avltree_element(it, sb_test_avltree_t, node);
        sb_console_fmt("search : index = %d, key = %d, value = %d\n", ++i, old->key, old->value);

        p = (sb_test_avltree_t*)sb_pool_alloc(pool, sizeof(sb_test_avltree_t));
        p->key = 520;
        p->value = 0xffff;

        sb_avltree_replace(&tree, &old->node, &p->node);

        search_node.key = 520;
        it = sb_avltree_search_if(&tree, &search_node.node, compare);
        p = sb_avltree_element(it, sb_test_avltree_t, node);
        sb_console_fmt("replace : index = %d, key = %d, value = %d\n", ++i, p->key, p->value);

        sb_test_avltree_remove(&tree, 520);
        i = 0;
        sb_avltree_for_each(&tree, it) {
            p = sb_avltree_element(it, sb_test_avltree_t, node);
            sb_console_fmt("index = %d, key = %d, value = %d\n", ++i, p->key, p->value);
        }

        sb_avltree_clear(&tree, clear);
    }

    sb_pool_destroy(pool);
    return 0;
}
