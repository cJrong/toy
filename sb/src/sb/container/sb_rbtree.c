

#include <sb/sb_config.h>
#include <sb/container/sb_rbtree.h>


#define sb_rbtree_set_red(node)         ((node)->color = SB_RBT_RED)
#define sb_rbtree_set_black(node)       ((node)->color = SB_RBT_BLACK)
#define sb_rbtree_is_red(node)          ((node)->color == SB_RBT_RED)
#define sb_rbtree_is_black(node)        ((node)->color == SB_RBT_BLACK)
#define sb_rbtree_copy_color(dst, src)  ((dst)->color = (src)->color)

static sb_void_t __sb_rbtree_left_rotate__(sb_rbtree_t *tree,
    sb_rbtree_node_t *node);
static sb_void_t __sb_rbtree_right_rotate__(sb_rbtree_t *tree,
    sb_rbtree_node_t *node);
 

sb_bool_t sb_rbtree_insert_if(sb_rbtree_t *tree,
    sb_rbtree_node_t *node, sb_rbtree_compare_pt compare)
{
    sb_rbtree_node_t **link = &tree->root;
    sb_rbtree_node_t  *grandpa = nullptr;
    sb_rbtree_node_t  *parent = nullptr;

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
    sb_rbtree_set_red(node);
    *link = node;

    while ((parent = node->parent) && sb_rbtree_is_red(parent)) {
        grandpa = parent->parent;

        if (parent == grandpa->left) {
            sb_rbtree_node_t *uncle = grandpa->right;

            if (uncle && sb_rbtree_is_red(uncle)) {
                sb_rbtree_set_black(uncle);
                sb_rbtree_set_black(parent);
                sb_rbtree_set_red(grandpa);
                node = grandpa;
                continue;

            } else {
                if (node == parent->right) {
                    parent = node;
                    node = node->parent;
                    __sb_rbtree_left_rotate__(tree, node);
                }
                sb_rbtree_set_black(parent);
                sb_rbtree_set_red(grandpa);
                __sb_rbtree_right_rotate__(tree, grandpa);
                break;
            }

        } else {
            sb_rbtree_node_t *uncle = grandpa->left;

            if (uncle && sb_rbtree_is_red(uncle)) {
                sb_rbtree_set_black(uncle);
                sb_rbtree_set_black(parent);
                sb_rbtree_set_red(grandpa);
                node = grandpa;
                continue;

            } else {
                if (node == parent->left) {
                    parent = node;
                    node = node->parent;
                    __sb_rbtree_right_rotate__(tree, node);
                }
                sb_rbtree_set_black(parent);
                sb_rbtree_set_red(grandpa);
                __sb_rbtree_left_rotate__(tree, grandpa);
                break;
            }
        }
    }

    sb_rbtree_set_black(tree->root);

    return true;
}


sb_rbtree_node_t *sb_rbtree_update_if(sb_rbtree_t *tree, 
    sb_rbtree_node_t *node, sb_rbtree_compare_pt compare)
{
    sb_rbtree_node_t **link = &tree->root;
    sb_rbtree_node_t  *grandpa = nullptr;
    sb_rbtree_node_t  *parent = nullptr;
    
    while (*link) {
        sb_int_t compare_value = compare(*link, node);

        if (compare_value == 0) {
            if (*link != node) {
                sb_rbtree_node_t *old = *link;
                sb_rbtree_replace(tree, old, node);
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
    sb_rbtree_set_red(node);
    *link = node;

    while ((parent = node->parent) && sb_rbtree_is_red(parent)) {
        grandpa = parent->parent;

        if (parent == grandpa->left) {
            sb_rbtree_node_t *uncle = grandpa->right;

            if (uncle && sb_rbtree_is_red(uncle)) {
                sb_rbtree_set_black(uncle);
                sb_rbtree_set_black(parent);
                sb_rbtree_set_red(grandpa);
                node = grandpa;
                continue;

            } else {
                if (node == parent->right) {
                    parent = node;
                    node = node->parent;
                    __sb_rbtree_left_rotate__(tree, node);
                }
                sb_rbtree_set_black(parent);
                sb_rbtree_set_red(grandpa);
                __sb_rbtree_right_rotate__(tree, grandpa);
                break;
            }

        } else {
            sb_rbtree_node_t *uncle = grandpa->left;

            if (uncle && sb_rbtree_is_red(uncle)) {
                sb_rbtree_set_black(uncle);
                sb_rbtree_set_black(parent);
                sb_rbtree_set_red(grandpa);
                node = grandpa;
                continue;

            } else {
                if (node == parent->left) {
                    parent = node;
                    node = node->parent;
                    __sb_rbtree_right_rotate__(tree, node);
                }
                sb_rbtree_set_black(parent);
                sb_rbtree_set_red(grandpa);
                __sb_rbtree_left_rotate__(tree, grandpa);
                break;
            }
        }
    }

    sb_rbtree_set_black(tree->root);

    return nullptr;
}


sb_rbtree_node_t *sb_rbtree_remove_if(sb_rbtree_t *tree, 
    sb_rbtree_node_t *node, sb_rbtree_compare_pt compare)
{
    sb_rbtree_node_t  *current = tree->root;
    sb_rbtree_node_t  *parent;
    sb_rbtree_node_t  *child;
    sb_rbtree_node_t  *old;
    sb_rbtree_color_t  color;

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
        color = current->color;

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

        current->left = old->left;
        old->left->parent = current;
        current->parent = old->parent;
        sb_rbtree_copy_color(current, old);
        
        goto maintain;
    }

    parent = current->parent;
    color = current->color;

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
    if (color == SB_RBT_RED) {
        return old;
    }

    while ((!child || sb_rbtree_is_black(child)) && (child != tree->root)) {
        if (child == parent->left) {
            sb_rbtree_node_t *brother = parent->right;

            if (sb_rbtree_is_red(brother)) {
                sb_rbtree_set_black(brother);
                sb_rbtree_set_red(parent);
                __sb_rbtree_left_rotate__(tree, parent);
                brother = parent->right;
            }

            if ((!brother->left || sb_rbtree_is_black(brother->left)) &&
                (!brother->right || sb_rbtree_is_black(brother->right))) {
                sb_rbtree_set_black(brother);
                child = parent;
                parent = child->parent;

            } else {
                if ((!brother->right || sb_rbtree_is_black(brother->right))) {
                    sb_rbtree_set_black(brother->left);
                    sb_rbtree_set_red(brother);
                    __sb_rbtree_right_rotate__(tree, brother);
                    brother = parent->right;
                }

                sb_rbtree_copy_color(brother, parent);
                sb_rbtree_set_black(parent);
                sb_rbtree_set_black(brother->right);
                __sb_rbtree_left_rotate__(tree, parent);
                child = tree->root;
                break;
            }

        } else {
            sb_rbtree_node_t *brother = parent->left;

            if (sb_rbtree_is_red(brother)) {
                sb_rbtree_set_black(brother);
                sb_rbtree_set_red(parent);
                __sb_rbtree_right_rotate__(tree, parent);
                brother = parent->left;
            }

            if ((!brother->left || sb_rbtree_is_black(brother->left)) &&
                (!brother->right || sb_rbtree_is_black(brother->right))) {
                sb_rbtree_set_red(brother);
                child = parent;
                parent = child->parent;

            } else {
                if (!brother->left || sb_rbtree_is_black(brother->left)) {
                    sb_rbtree_set_black(brother->right);
                    sb_rbtree_set_red(brother);
                    __sb_rbtree_left_rotate__(tree, brother);
                    brother = parent->left;
                }

                sb_rbtree_copy_color(brother, parent);
                sb_rbtree_set_black(parent);
                sb_rbtree_set_black(brother->left);
                __sb_rbtree_right_rotate__(tree, parent);
                child = tree->root;
                break;
            }
        }
    }

    if (child) {
        sb_rbtree_set_black(child);
    }

    return old;
}


sb_rbtree_node_t *sb_rbtree_search_if(sb_rbtree_t *tree,
    sb_rbtree_node_t *node, sb_rbtree_compare_pt compare)
{
    sb_rbtree_node_t *current = tree->root;

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
 * typedef struct sb_rbtest_s {
 *      sb_int_t         key;
 *      sb_int_t         data;
 *      sb_rbtree_node_t rb;
 * }sb_rbtest_t;
 *
 * static sb_void_t sb_rbtree_insert_example(sb_rbtree_t *tree, sb_rbtest_t *node)
 * {
 *      sb_rbtree_node_t **current = &tree->root, *parent = nullptr;
 *
 *      while (*current) {
 *          parent = *current;
 *          current = node->key < sb_rbtree_element(parent, sb_rbtest_t, rb)->key ?
 *              &parent->left : &parent->right;
 *      }
 *
 *      sb_rbtree_link_node(current, parent, &node->rb);
 *      sb_rbtree_insert(tree, &node->rb);
 * }
 *
 * sb_void_t sb_rbtree_remove_example(sb_rbtree_t *tree, sb_int_t key)
 * {
 *      sb_rbtree_node_t *current = tree->root;
 *
 *      while (current) {
 *          sb_int_t compare_value = sb_rbtree_element(current, sb_rbtest_t, rb)->key - key;
 *          if (compare_value == 0) {
 *              break;
 *          }
 *
 *           current = compare_value > 0 ? current->left : current->right;
 *       }
 *
 *      if (!current) {
 *          return;
 *      }
 *
 *      sb_rbtree_remove(tree, current);
 * }
 *
 * sb_rbtree_node_t *sb_rbtree_search_example(sb_rbtree_t *tree, sb_int_t key)
 * {
 *     sb_rbtree_node_t *current = tree->root;
 *     sb_rbtest_t *current;
 *
 *     while (current) {
 *         current = sb_rbtree_element(current, sb_rbtest_t, node);
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


sb_void_t sb_rbtree_insert(sb_rbtree_t *tree, sb_rbtree_node_t *node)
{
    sb_rbtree_node_t *parent, *grandpa;

    while ((parent = node->parent) && sb_rbtree_is_red(parent)) {
        grandpa = parent->parent;

        if (parent == grandpa->left) {
            sb_rbtree_node_t *uncle = grandpa->right;

            if (uncle && sb_rbtree_is_red(uncle)) {
                sb_rbtree_set_black(uncle);
                sb_rbtree_set_black(parent);
                sb_rbtree_set_red(grandpa);
                node = grandpa;
                continue;

            } else {
                if (node == parent->right) {
                    parent = node;
                    node = node->parent;
                    __sb_rbtree_left_rotate__(tree, node);
                }
                sb_rbtree_set_black(parent);
                sb_rbtree_set_red(grandpa);
                __sb_rbtree_right_rotate__(tree, grandpa);
                break;
            }
            

        } else {
            sb_rbtree_node_t *uncle = grandpa->left;

            if (uncle && sb_rbtree_is_red(uncle)) {
                sb_rbtree_set_black(uncle);
                sb_rbtree_set_black(parent);
                sb_rbtree_set_red(grandpa);
                node = grandpa;
                continue;

            } else {
                if (node == parent->left) {
                    parent = node;
                    node = node->parent;
                    __sb_rbtree_right_rotate__(tree, node);
                }
                sb_rbtree_set_black(parent);
                sb_rbtree_set_red(grandpa);
                __sb_rbtree_left_rotate__(tree, grandpa);
                break;
            }
        }
    }

    sb_rbtree_set_black(tree->root);
}


sb_void_t sb_rbtree_remove(sb_rbtree_t *tree, sb_rbtree_node_t *node)
{
    sb_rbtree_node_t *child, *parent;
    sb_rbtree_color_t color;

    if (!node->left) {
        child = node->right;

    } else if (!node->right) {
        child = node->left;

    } else {
        sb_rbtree_node_t *old = node;

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

        child = node->right;
        parent = node->parent;
        color = node->color;

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
        sb_rbtree_copy_color(node, old);

        goto maintain;
    }

    parent = node->parent;
    color = node->color;

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
    if (color == SB_RBT_RED) {
        return ;
    }

    while ((!child || sb_rbtree_is_black(child)) && child != tree->root) {
        if (parent->left == child) {
            sb_rbtree_node_t *brother = parent->right;

            if (sb_rbtree_is_red(brother)) {
                sb_rbtree_set_black(brother);
                sb_rbtree_set_red(parent);
                __sb_rbtree_left_rotate__(tree, parent);
                brother = parent->right;
            }

            if ((!brother->left || sb_rbtree_is_black(brother->left)) &&
                (!brother->right || sb_rbtree_is_black(brother->right))) {
                sb_rbtree_set_red(brother);
                child = parent;
                parent = child->parent;

            } else {
                if (!brother->right || sb_rbtree_is_black(brother->right)) {
                    sb_rbtree_set_black(brother->left);
                    sb_rbtree_set_red(brother);
                    __sb_rbtree_right_rotate__(tree, brother);
                    brother = parent->right;
                }

                sb_rbtree_copy_color(brother, parent);
                sb_rbtree_set_black(parent);
                sb_rbtree_set_black(brother->right);
                __sb_rbtree_left_rotate__(tree, parent);
                child = tree->root;
                break;
            }

        } else {
            sb_rbtree_node_t *brother = parent->left;

            if (sb_rbtree_is_red(brother)) {
                sb_rbtree_set_black(brother);
                sb_rbtree_set_red(parent);
                __sb_rbtree_right_rotate__(tree, parent);
                brother = parent->left;
            }

            if ((!brother->left || sb_rbtree_is_black(brother->left)) &&
                (!brother->right || sb_rbtree_is_black(brother->right))) {
                sb_rbtree_set_red(brother);
                child = parent;
                parent = child->parent;

            } else {
                if (!brother->left || sb_rbtree_is_black(brother->left)) {
                    sb_rbtree_set_black(brother->right);
                    sb_rbtree_set_red(brother);
                    __sb_rbtree_left_rotate__(tree, brother);
                    brother = parent->left;
                }

                sb_rbtree_copy_color(brother, parent);
                sb_rbtree_set_black(parent);
                sb_rbtree_set_black(brother->left);
                __sb_rbtree_right_rotate__(tree, parent);
                child = tree->root;
                break;
            }
        }
    }

    if (child) {
        sb_rbtree_set_black(child);
    }
}


/**
 * note !!
 * ban change Key
 */
sb_void_t sb_rbtree_magic(sb_rbtree_t *tree, sb_rbtree_magic_pt magic)
{
    sb_rbtree_node_t *iterator;

    if (magic) {
        sb_rbtree_for_each(tree, iterator) {
             if (!magic(iterator)) {
                return;
             }
        }
    }
}


sb_void_t sb_rbtree_clear(sb_rbtree_t *tree, sb_rbtree_magic_pt magic)
{
    sb_rbtree_node_t *iterator, *next_iterator;

    if (tree->root) {
        sb_rbtree_safe_for_each(tree,  iterator, next_iterator) {
            sb_rbtree_remove(tree, iterator);

            if (magic) {
                if (!magic(iterator)) {
                    return;
                }
            }
        }

        tree->root = nullptr;
    }
}


sb_bool_t sb_rbtree_is_same_tree(sb_rbtree_node_t *lhs, sb_rbtree_node_t *rhs,
    sb_rbtree_compare_pt compare)
{
    if (!lhs && !rhs)
        return true;

    if (lhs && rhs) {
        if ((compare(lhs, rhs) != 0) ||
            !sb_rbtree_is_same_tree(lhs->left, rhs->left, compare) ||
            !sb_rbtree_is_same_tree(lhs->right, rhs->right, compare)) {
            return false;
        }
        return true;
    }

    return false;
}


sb_rbtree_node_t *sb_rbtree_begin(sb_rbtree_t *tree)
{
    sb_rbtree_node_t *begin = tree->root;

    if (begin == nullptr) {
        return nullptr;
    }

    while (begin->left) {
        begin = begin->left;
    }

    return begin;
}


sb_rbtree_node_t *sb_rbtree_rbegin(sb_rbtree_t *tree)
{
    sb_rbtree_node_t *rbegin = tree->root;

    if (rbegin == nullptr) {
        return nullptr;
    }

    while (rbegin->right) {
        rbegin = rbegin->right;
    }

    return rbegin;
}


sb_rbtree_node_t *sb_rbtree_next(sb_rbtree_node_t *node)
{
    sb_rbtree_node_t *parent = nullptr;

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


sb_rbtree_node_t *sb_rbtree_rnext(sb_rbtree_node_t *node)
{
    sb_rbtree_node_t *parent = nullptr;

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
sb_void_t __sb_rbtree_left_rotate__(sb_rbtree_t *tree, sb_rbtree_node_t *node)
{
    sb_rbtree_node_t *right = node->right;
    sb_rbtree_node_t *parent = node->parent;

    if ((node->right = right->left)) {
        right->left->parent = node;
    }

    right->left = node;
    node->parent = right;

    if (parent) {
        if (node == parent->left) {
            parent->left  = right;
        } else {
            parent->right = right;
        }

    } else {
        tree->root = right;
    }

    right->parent = parent;
}


static
sb_void_t __sb_rbtree_right_rotate__(sb_rbtree_t *tree, sb_rbtree_node_t *node)
{
    sb_rbtree_node_t *left = node->left;
    sb_rbtree_node_t *parent = node->parent;

    if ((node->left = left->right)) {
        left->right->parent = node;
    }

    left->right = node;
    node->parent = left;

    if (parent) {
        if (node == parent->left) {
            parent->left  = left;
        } else {
            parent->right = left;
        }

    } else {
        tree->root = left;
    }

    left->parent = parent;
}
