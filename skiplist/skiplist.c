#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_LEVEL 16

typedef struct skiplist_node_s {
    int key;
    int value;
    struct skiplist_node_s *forwards[];
}skiplist_node_t;

typedef struct skiplist_s {
    skiplist_node_t *head;
    int level;
}skiplist_t;


static int skiplist_randomlevel(void)
{
    int level = 1;

    while ((random() & 0xffff) < (0.2*0xffff)) {
        level += 1;
    }

    return level < MAX_LEVEL ? level : MAX_LEVEL;
}


static skiplist_node_t *skiplist_create_node(int level, int key, int value)
{
    skiplist_node_t *node = (skiplist_node_t*)
            malloc(sizeof(skiplist_node_t) + level * sizeof(skiplist_node_t*));
    if (node == NULL) {
        return NULL;
    }

    node->key = key;
    node->value = value;

    return node;
}


skiplist_t *skiplist_create(void) {
    skiplist_t *list;
    int i;

    list = (skiplist_t*) malloc(sizeof(skiplist_t));
    if (list == NULL) {
        return list;
    }

    list->level = 1;
    list->head = skiplist_create_node(MAX_LEVEL, 0, 0);
    if (list->head == NULL) {
        free(list);
        return NULL;
    }

    for (i = 0; i < MAX_LEVEL; i++) {
        list->head->forwards[i] = NULL;
    }

    return list;
}


void skiplist_destory(skiplist_t *list)
{
    skiplist_node_t *prev = list->head, *next;

    while (prev) {
        next =prev->forwards[0];
        free(prev);
        prev = next;
    }

    free(list);
}


void skiplist_insert(skiplist_t *list, int key, int value)
{
    skiplist_node_t *update[MAX_LEVEL];
    skiplist_node_t *prev = list->head;
    int level, i;

    for (level = list->level - 1; level >= 0; level--) {
        while (prev->forwards[level] && prev->forwards[level]->key < key) {
            prev = prev->forwards[level];
        }

        update[level] = prev;
    }

    level = skiplist_randomlevel();
    if (level > list->level) {
        for (i = level; i >= list->level; i--) {
            update[i] = list->head;
        }
        list->level = level;
    }

    prev = skiplist_create_node(level, key, value);
    if (prev == NULL) {
        return ;
    }

    for (i = 0; i < level; i++) {
        prev->forwards[i] = update[i]->forwards[i];
        update[i]->forwards[i] = prev;
    }
}


void skiplist_delete(skiplist_t *list, int key)
{
    skiplist_node_t *update[MAX_LEVEL];
    skiplist_node_t *prev = list->head;
    skiplist_node_t *erase;
    int level = list->level - 1;
    int i;

    for (; level >= 0; level--) {
        while(prev->forwards[level] && prev->forwards[level]->key < key) {
            prev = prev->forwards[level];
        }

        update[level] = prev;
    }

    erase = prev->forwards[0];
    if (erase && (erase->key != key)) {
        return ;
    }

    for (i = 0; i < list->level; i++) {
        if (update[i]->forwards[i] == erase) {
            update[i]->forwards[i] = erase->forwards[i];
        }
    }

    while (list->level > 1 && list->head->forwards[list->level - 1]) {
        list->level --;
    }

    free(erase);
}


int skiplist_search(skiplist_t *list, int key)
{
    skiplist_node_t *node = list->head;
    int level = list->level - 1;

    for (; level >= 0; level--) {
        while (node->forwards[level] && node->forwards[level]->key < key) {
            node = node->forwards[level];
        }
    }

    node = node->forwards[0];

    return (node && node->key == key) ? node->value : -1;
}


int main(void)
{
    skiplist_t *list = skiplist_create();
    skiplist_node_t *iterator;
    int keys[100];
    int i;
    int r;

    for (i = 0; i < 100; i++) {
        r = random();
        skiplist_insert(list, r, random());
        keys[i] = r;
    }

    skiplist_insert(list, 10, 30);

    printf("search key=10, value=%d\n", skiplist_search(list, 10));

    for (i = 0; i < 100; i++) {
        skiplist_delete(list, keys[i]);
    }

    skiplist_insert(list, 10, 20);

    printf("%d\n", skiplist_search(list, 10));

    iterator = list->head->forwards[0];
    while (iterator) {
        printf("key=%d, value=%d\n", iterator->key, iterator->value);
        iterator = iterator->forwards[0];
    }

    skiplist_destory(list);

    return 0;
}
