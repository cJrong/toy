

#include <sb/sb_config.h>
#include <sb/container/sb_avltree.h>


#define sb_avltree_height(node)                                               \
    ((node) ? (node)->height : 0)

#define sb_avltree_max_height(left, right)                                    \
    (sb_avltree_height((left)) > sb_avltree_height((right)) ?                 \
     sb_avltree_height((left)) : sb_avltree_height((right)))

#define sb_avltree_balance(node)                                              \
    (sb_avltree_height((node)->left) - sb_avltree_height((node)->right))


static
sb_void_t __sb_avltree_left_rotate__(sb_avltree_t *tree,
    sb_avltree_node_t *node);
static
sb_void_t __sb_avltree_right_rotate__(sb_avltree_t *tree,
    sb_avltree_node_t *node);


sb_bool_t sb_avltree_insert_if(sb_avltree_t *tree,
    sb_avltree_node_t *node, sb_avltree_compare_pt compare)
{
    sb_avltree_node_t **link = &tree->root;
    sb_avltree_node_t *parent = nullptr;

    while (*link) {
        sb_int_t compare_value = compare(*link, node);

        if (compare_value == 0) {
            return false;
        }

        parent = *link;
        link = compare_value > 0 ? &(*link)->left : &(*link)->right;
    }

    node->left = nullptr;
    node->right = nullptr;
    node->parent = parent;
    node->height = 1;
    *link = node;

    while (parent) {
        sb_int_t balance = sb_avltree_balance(parent);

        parent->height =
            sb_avltree_max_height(parent->left, parent->right) + 1;

        if (balance > 1) {
            if (sb_avltree_balance(parent->left) > 0) {
                __sb_avltree_right_rotate__(tree, parent);

            } else {
                __sb_avltree_left_rotate__(tree, parent->left);
                __sb_avltree_right_rotate__(tree, parent);
            }

            break;

        } else if (balance < -1) {
            if (sb_avltree_balance(parent->right) < 0) {
                __sb_avltree_left_rotate__(tree, parent);

            } else {
                __sb_avltree_right_rotate__(tree, parent->right);
                __sb_avltree_left_rotate__(tree, parent);
            }

            break;
        }

        parent = parent->parent;
    }

    return true;
}


sb_avltree_node_t *sb_avltree_update_if(sb_avltree_t* tree,
    sb_avltree_node_t* node, sb_avltree_compare_pt compare)
{
    sb_avltree_node_t **link = &tree->root;
    sb_avltree_node_t *parent = nullptr;

    while (*link) {
        sb_int_t compare_value = compare(*link, node);

        if (compare_value == 0) {
            if (*link != node) {
                sb_avltree_node_t *old = *link;
                sb_avltree_replace(tree, old, node);
                return old;
            }

            return nullptr;
        }

        parent = *link;
        link = compare_value > 0 ? &(*link)->left : &(*link)->right;
    }

    node->left = nullptr;
    node->right = nullptr;
    node->parent = parent;
    node->height = 1;
    *link = node;

    while (parent) {
        sb_int_t balance = sb_avltree_balance(parent);

        parent->height =
            sb_avltree_max_height(parent->left, parent->right) + 1;

        if (balance > 1) {
            if (sb_avltree_balance(parent->left) > 0) {
                __sb_avltree_right_rotate__(tree, parent);

            } else {
                __sb_avltree_left_rotate__(tree, parent->left);
                __sb_avltree_right_rotate__(tree, parent);
            }

            break;

        } else if (balance < -1) {
            if (sb_avltree_balance(parent->right) < 0) {
                __sb_avltree_left_rotate__(tree, parent);

            } else {
                __sb_avltree_right_rotate__(tree, parent->right);
                __sb_avltree_left_rotate__(tree, parent);
            }

            break;
        }

        parent = parent->parent;
    }

    return nullptr;
}


sb_avltree_node_t *sb_avltree_remove_if(sb_avltree_t *tree,
    sb_avltree_node_t *node, sb_avltree_compare_pt compare)
{
    sb_avltree_node_t *current = tree->root;
    sb_avltree_node_t *child;
    sb_avltree_node_t *parent;
    sb_avltree_node_t *old;

    while (current) {
        sb_int_t compare_value = compare(current, node);

        if (compare_value == 0) {
            break;
        }

        current = compare_value > 0 ? current->left : current->right;
    }

    if (current == nullptr) {
        return nullptr;
    }

    old = current;

    if (!current->left) {
        child = current->right;

    } else if (!current->right) {
        child = current->left;

    } else {
        current = current->right;
        while (current->left) {
            current = current->left;
        }

        if (old->parent) {
            if (old->parent->left == old) {
                old->parent->left = current;

            } else {
                old->parent->right = current;
            }

        } else {
            tree->root = current;
        }

        child = current->right;
        parent = current->parent;

        if (parent == old) {
            parent = current;

        } else {
            if (child) {
                child->parent = parent;
            }
            parent->left = child;

            current->right = old->right;
            old->right->parent = current;
        }

        current->left     = old->left;
        old->left->parent = current;
        current->parent   = old->parent;
        goto maintain;
    }

    parent = current->parent;

    if (child) {
        child->parent = parent;
    }

    if (parent) {
        if (parent->left == current) {
            parent->left = child;

        } else {
            parent->right = child;
        }

    } else {
        tree->root = child;
    }

maintain:
    while (parent) {
        sb_int_t balance = sb_avltree_balance(parent);

        parent->height =
            sb_avltree_max_height(parent->left, parent->right) + 1;

        if (balance > 1) {
            if (sb_avltree_balance(parent->left) > 0) {
                __sb_avltree_right_rotate__(tree, parent);

            } else {
                __sb_avltree_left_rotate__(tree, parent->left);
                __sb_avltree_right_rotate__(tree, parent);
            }

        } else if (balance < -1) {
            if (sb_avltree_balance(parent->right) < 0) {
                __sb_avltree_left_rotate__(tree, parent);

            } else {
                __sb_avltree_right_rotate__(tree, parent->right);
                __sb_avltree_left_rotate__(tree, parent);
            }
        }

        parent = parent->parent;
    }

    return old;
}


sb_avltree_node_t *sb_avltree_search_if(sb_avltree_t *tree,
    sb_avltree_node_t *node, sb_avltree_compare_pt compare)
{
    sb_avltree_node_t *current = tree->root;

    while (current) {
        sb_int_t compare_value = compare(current, node);

        if (compare_value == 0) {
            return current;
        }

        current = compare_value > 0 ? current->left : current->right;
    }

    return nullptr;
}


/**
 *
 * typedef struct {
 *    sb_int_t          key;
 *    sb_int_t          data;
 *    sb_avltree_node_t node;
 * }sb_avltest_t;
 *
 * static sb_void_t sb_avltree_insert_example(sb_avltree_t *tree, sb_avltest_t *node)
 * {
 *   sb_avltree_node_t **current = &tree->root, *parent = nullptr;
 *
 *    while (*current) {
 *        sb_int_t compare;
 *        parent = *current;
 *
 *        compare = node->key - sb_avltree_element(parent, sb_avltest_t, node)->key;
 *        if (compare == 0) {
 *            return;
 *        }
 *
 *        current = compare < 0 ? &parent->left : &parent->right;
 *    }
 *    
 *    sb_avltree_link_node(current, parent, &node->node);
 *    sb_avltree_insert(tree, &node->node);
 * }
 *
 * static sb_void_t sb_avltree_remove_example(sb_avltree_t *tree, sb_int_t key)
 * {
 *    sb_avltree_node_t *erase = tree->root;
 *    sb_avltest_t *current;
 *
 *    while (erase) {
 *       current = sb_avltree_element(erase, sb_avltest_t, node);
 *
 *        if (current->key == key) {
 *            break;
 *        }
 *
 *        erase = current->key > key ? erase->left : erase->right;
 *    }
 *
 *    if (!erase) {
 *        return;
 *    }
 *
 *    sb_avltree_remove(tree, erase);
 * }
 *
 * sb_avltree_node_t *sb_avltree_search_example(sb_avltree_t *tree, sb_int_t *key)
 * {
 *     sb_avltree_node_t *current = tree->root;
 *     sb_avltest_t *current;
 *
 *     while (current) {
 *         current = sb_avltree_element(current, sb_avltest_t, node);
 *
 *         if (current->key == key) {
 *             return current;
 *         }
 *
 *         current = current->key > key ? current->left : current->right;
 *     }
 *
 *     return nullptr;
 * }
 */


sb_void_t sb_avltree_insert(sb_avltree_t *tree, sb_avltree_node_t *node)
{
    while (node) {
        sb_int_t balance = sb_avltree_balance(node);

        node->height = sb_avltree_max_height(node->left, node->right) + 1;

        if (balance > 1) {
            if (sb_avltree_balance(node->left) > 0) {
                __sb_avltree_right_rotate__(tree, node);

            } else {
                __sb_avltree_left_rotate__(tree, node->left);
                __sb_avltree_right_rotate__(tree, node);
            }

            return ;

        } else if (balance < -1) {
            if (sb_avltree_balance(node->right) < 0) {
                __sb_avltree_left_rotate__(tree, node);

            } else {
                __sb_avltree_right_rotate__(tree, node->right);
                __sb_avltree_left_rotate__(tree, node);
            }

            return;
        }

        node = node->parent;
    }
}


sb_void_t sb_avltree_remove(sb_avltree_t *tree, sb_avltree_node_t *node)
{
    sb_avltree_node_t *child;
    sb_avltree_node_t *parent;
    sb_avltree_node_t *old;
 
    old = node;

    if (!node->left) {
        child = node->right;

    } else if (!node->right) {
        child = node->left;

    } else {
        node = node->right;
        while (node->left) {
            node = node->left;
        }

        if (old->parent) {
            if (old->parent->left == old) {
                old->parent->left = node;

            } else {
                old->parent->right = node;
            }

        } else {
            tree->root = node;
        }

        child  = node->right;
        parent = node->parent;

        if (parent == old) {
            parent = node;

        } else {
            if (child) {
                child->parent = parent;
            }
            parent->left = child;

            node->right = old->right;
            old->right->parent = node;
        }

        node->left = old->left;
        old->left->parent = node;
        node->parent = old->parent;
        goto maintain;
    }

    parent = node->parent;

    if (child) {
        child->parent = parent;
    }

    if (parent) {
        if (parent->left == node) {
            parent->left = child;

        } else {
            parent->right = child;
        }

    } else {
        tree->root = child;
    }

maintain:
    while (parent) {
        sb_int_t balance = sb_avltree_balance(parent);

        parent->height =
            sb_avltree_max_height(parent->left, parent->right) + 1;

        if (balance > 1) {
            if (sb_avltree_balance(parent->left) > 0) {
                __sb_avltree_right_rotate__(tree, parent);

            } else {
                __sb_avltree_left_rotate__(tree, parent->left);
                __sb_avltree_right_rotate__(tree, parent);
            }

        } else if (balance < -1) {
            if (sb_avltree_balance(parent->right) < 0) {
                __sb_avltree_left_rotate__(tree, parent);

            } else {
                __sb_avltree_right_rotate__(tree, parent->right);
                __sb_avltree_left_rotate__(tree, parent);
            }
        }

        parent = parent->parent;
    }
}


sb_void_t sb_avltree_magic(sb_avltree_t *tree, sb_avltree_magic_pt magic) 
{
    sb_avltree_node_t *iterator;

    sb_avltree_for_each(tree, iterator) {
        if (!magic(iterator)) {
            return ;
        }
    }
}


sb_void_t sb_avltree_clear(sb_avltree_t *tree, sb_avltree_magic_pt magic)
{
    sb_avltree_node_t *iterator, *next_iterator;

    if (tree->root) {
        sb_avltree_safe_for_each(tree, iterator, next_iterator) {
            sb_avltree_remove(tree, iterator);

            if (magic) {
                if (!magic(iterator)) {
                    return ;
                }
            }
        }

        tree->root = nullptr;
    }
}


sb_bool_t sb_avltree_is_same_tree(sb_avltree_node_t *lhs,
    sb_avltree_node_t *rhs, sb_avltree_compare_pt compare)
{
    if (!lhs && !rhs)
        return true;

    if (lhs && rhs) {
        if ((compare(lhs, rhs) != 0) ||
            !sb_avltree_is_same_tree(lhs->left, rhs->left, compare) ||
            !sb_avltree_is_same_tree(lhs->right, rhs->right, compare)) {
            return false;
        }
        return true;
    }

    return false;
}


sb_avltree_node_t *sb_avltree_begin(sb_avltree_t *tree)
{
    sb_avltree_node_t *begin = tree->root;

    if (begin == nullptr) {
        return nullptr;
    }

    while (begin->left) {
        begin = begin->left;
    }

    return begin;
}


sb_avltree_node_t *sb_avltree_rbegin(sb_avltree_t *tree)
{
    sb_avltree_node_t *rbegin = tree->root;

    if (rbegin == nullptr) {
        return nullptr;
    }

    while (rbegin->right) {
        rbegin = rbegin->right;
    }

    return rbegin;
}


sb_avltree_node_t* sb_avltree_next(sb_avltree_node_t *node)
{
    sb_avltree_node_t *parent = nullptr;

    if (node->right != nullptr) {
        node = node->right;

        while (node->left) {
            node = node->left;
        }

        return node;
    }

    while ((parent = node->parent) && (node == parent->right)) {
        node = parent;
    }

    return parent;
}


sb_avltree_node_t *sb_avltree_rnext(sb_avltree_node_t *node)
{
    sb_avltree_node_t *parent = nullptr;

    if (node->left != nullptr) {
        node = node->left;

        while (node->right) {
            node = node->right;
        }

        return node;
    }

    while ((parent = node->parent) && (node == parent->left)) {
        node = parent;
    }

    return parent;
}


static 
sb_void_t __sb_avltree_left_rotate__(sb_avltree_t *tree,
    sb_avltree_node_t *node)
{
    sb_avltree_node_t *right = node->right;
    sb_avltree_node_t *parent = node->parent;

    if ((node->right = right->left)) {
        right->left->parent = node;
    }

    right->left = node;
    node->parent = right;

    if (parent) {
        if (node == parent->left) {
            parent->left = right;

        } else {
            parent->right = right;
        }

    } else {
        tree->root = right;
    }

    right->parent = parent;

    node->height = sb_avltree_max_height(node->left, node->right) + 1;
    right->height = sb_avltree_max_height(right->left, right->right) + 1;
}


static 
sb_void_t __sb_avltree_right_rotate__(sb_avltree_t *tree,
    sb_avltree_node_t *node)
{
    sb_avltree_node_t *left = node->left;
    sb_avltree_node_t *parent = node->parent;

    if ((node->left = left->right)) {
        left->right->parent = node;
    }

    left->right = node;
    node->parent = left;

    if (parent) {
        if (node == parent->left) {
            parent->left = left;

        } else {
            parent->right = left;
        }

    } else {
        tree->root = left;
    }

    left->parent = parent;

    node->height = sb_avltree_max_height(node->left, node->right) + 1;
    left->height = sb_avltree_max_height(left->left, left->right) + 1;
}
