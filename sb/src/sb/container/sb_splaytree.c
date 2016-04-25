

#include <sb/sb_config.h>
#include <sb/container/sb_splaytree.h>


static
sb_void_t __sb_splaytree_left_rotate__(sb_splaytree_t *tree,
    sb_splaytree_node_t *node);
static
sb_void_t __sb_splaytree_right_rotate__(sb_splaytree_t *tree,
    sb_splaytree_node_t *node);
static
sb_void_t __sb_splaytree_splay__(sb_splaytree_t *tree,
    sb_splaytree_node_t *node, sb_splaytree_compare_pt compare);


sb_bool_t sb_splaytree_insert_if(sb_splaytree_t *tree,
    sb_splaytree_node_t *node, sb_splaytree_compare_pt compare)
{
    sb_int_t compare_value;

    __sb_splaytree_splay__(tree, node, compare);

    if (tree->root == nullptr) {
        node->left = node->right = node->parent = nullptr;
        goto done;
    }

    if ((compare_value = compare(tree->root, node)) == 0) {
        if (tree->root != node) {
           return false;
        }
        goto done;
    }

    node->parent = nullptr;

    if (compare_value > 0) {
        node->left = tree->root->left;
        if (node->left) {
            node->left->parent = node;
        }

        node->right = tree->root;
        if (node->right) {
            node->right->parent = node;
        }

        tree->root->left = nullptr;

    } else {
        node->right = tree->root->right;
        if (node->right) {
            node->right->parent = node;
        }

        node->left = tree->root;
        if (node->left) {
            node->left->parent = node;
        }

        tree->root->right = nullptr;
    }

done:
    tree->root = node;
    return true;
}


sb_splaytree_node_t *sb_splaytree_update_if(sb_splaytree_t *tree,
    sb_splaytree_node_t *node, sb_splaytree_compare_pt compare)
{
    sb_int_t compare_value;

    __sb_splaytree_splay__(tree, node, compare);

    if (tree->root == nullptr) {
        node->left = node->right = node->parent = nullptr;
        goto done;
    }

    if ((compare_value = compare(tree->root, node)) == 0) {
        if (tree->root != node) {
            sb_splaytree_node_t *old = tree->root;
            sb_splaytree_replace(tree, old, node);
            return old;
        }
        goto done;
    }

    node->parent = nullptr;

    if (compare_value > 0) {
        node->left = tree->root->left;
        if (node->left) {
            node->left->parent = node;
        }

        node->right = tree->root;
        if (node->right) {
            node->right->parent = node;
        }

        tree->root->left = nullptr;

    } else {
        node->right = tree->root->right;
        if (node->right) {
            node->right->parent = node;
        }

        node->left = tree->root;
        if (node->left) {
            node->left->parent = node;
        }

        tree->root->right = nullptr;
    }

done:
    tree->root = node;
    return nullptr;
}


sb_splaytree_node_t *sb_splaytree_remove_if(sb_splaytree_t *tree,
    sb_splaytree_node_t *node, sb_splaytree_compare_pt compare)
{
    sb_int_t compare_value;

    __sb_splaytree_splay__(tree, node, compare);

    if ((tree->root) && ((compare_value = compare(tree->root, node)) == 0)) {
        sb_splaytree_node_t *newroot = tree->root;
        sb_splaytree_node_t *old = newroot;
        sb_splaytree_node_t *child;
        sb_splaytree_node_t *parent;

        if (!newroot->left) {
            child = newroot->right;

        } else if (!newroot->right) {
            child = newroot->left;

        } else {
            newroot = newroot->right;
            while (newroot->left) {
                newroot = newroot->left;
            }

            tree->root = newroot;

            child = newroot->right;
            parent = newroot->parent;

            if (parent == old) {
                parent = child;

            } else {
                if (child) {
                    child->parent = parent;
                }

                parent->left = child;

                newroot->right = old->right;
                newroot->right->parent = newroot;
            }

            newroot->left = old->left;
            newroot->left->parent = newroot;
            newroot->parent = old->parent;
            goto done;
        }

        if (child) {
            child->parent = nullptr;
        }

        tree->root = child;

    done:
        return old;
    }

    return nullptr;
}


sb_splaytree_node_t *sb_splaytree_search_if(sb_splaytree_t *tree,
    sb_splaytree_node_t *node, sb_splaytree_compare_pt compare)
{
    __sb_splaytree_splay__(tree, node, compare);

    if (compare(tree->root, node) == 0) {
        return tree->root;
    }

    return nullptr;
}


/**
 *
 * typedef struct {
 *     sb_int_t            key;
 *     sb_int_t            data;
 *     sb_splaytree_node_t node;
 * }sb_splaytree_test_t;
 *
 * sb_void_t sb_splaytree_insert_example(sb_splaytree_t *tree, sb_splaytree_test_t *node)
 * {
 *    sb_splaytree_node_t **current = &tree->root, *parent = nullptr;
 *
 *     while (*current) {
 *         sb_int_t compare;
 *         parent = *current;
 *
 *         compare = node->key - sb_splaytree_element(parent, sb_splaytree_test_t, node)->key;
 *         if (compare == 0) {
 *             return;
 *         }
 *
 *         current = compare < 0 ? &parent->left : &parent->right;
 *     }
 *
 *     sb_splaytree_link_node(current, parent, &node->node);
 *     sb_splaytree_splay(tree, &node->node);
 * }
 *
 * sb_void_t sb_splaytree_remove_example(sb_splaytree_t *tree, sb_int_t key)
 * {
 *     sb_splaytree_node_t *current = tree->root;
 *
 *     while (current) {
 *         sb_int_t compare_value = sb_splaytree_element(current, sb_splaytree_test_t, node)->key - key;
 *         if (compare_value == 0) {
 *             break;
 *         }
 *
 *         current = compare_value > 0 ? current->left : current->right;
 *     }
 *
 *     if (!current) {
 *         return;
 *     }
 *
 *     sb_splaytree_remove(tree, current);
 * }
 *
 * sb_splaytree_test_t* sb_splaytree_search_example(sb_splaytree_t *tree, sb_int_t key)
 * {
 *     sb_splaytree_node_t *current = tree->root;
 *
 *     while (current) {
 *         sb_int_t compare_value = sb_splaytree_element(current, sb_splaytree_test_t, node)->key - key;
 *         if (compare_value == 0) {
 *            break;
 *         }
 *
 *         current = compare_value > 0 ? current->left : current->right;
 *     }
 *
 *     if (!current) {
 *         return nullptr;
 *     }
 *
 *     sb_splaytree_splay(tree, current);
 *
 *     return sb_splaytree_entry(current, sb_splaytree_test_t, node);
 * }
 */


sb_void_t sb_splaytree_splay(sb_splaytree_t *tree, sb_splaytree_node_t *node)
{
    while (node && node->parent) {
        sb_splaytree_node_t *parent = node->parent;

        if (node == parent->left) {
            __sb_splaytree_right_rotate__(tree, node->parent);

        } else {
            __sb_splaytree_left_rotate__(tree, node->parent);
        }

        parent = node->parent;
        if (parent && parent->parent) {
            if (parent == parent->parent->left) {
                __sb_splaytree_right_rotate__(tree, parent->parent);

            } else {
                __sb_splaytree_left_rotate__(tree, parent->parent);
            }
        }
    }
}


sb_void_t sb_splaytree_remove(sb_splaytree_t *tree, sb_splaytree_node_t *node)
{
    sb_splaytree_node_t *child;
    sb_splaytree_node_t *parent;
    sb_splaytree_node_t *old;

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

        child = node->right;
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
        goto done;
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

done:
    return ;
}


sb_void_t __sb_splaytree_splay__(sb_splaytree_t *tree,
    sb_splaytree_node_t *node, sb_splaytree_compare_pt compare)
{
    sb_splaytree_node_t *middle = tree->root;
    sb_splaytree_node_t *llink = nullptr, *rlink = nullptr;
    sb_splaytree_node_t *ltree = nullptr, *rtree = nullptr;
    sb_int_t compare_value;

    while ((middle) && ((compare_value = compare(middle, node)) != 0)) {
        if (compare_value > 0) {
            if (!middle->left)  {
                break;
            }

            if (compare(middle->left, node) > 0) {
                __sb_splaytree_right_rotate__(tree, middle);
                middle = middle->parent;

                if (middle->left == nullptr) {
                    break;
                }
            }

            if (rlink) {
                rlink->left = middle;
                middle->parent = rlink;

            } else {
                rtree = middle;
            }

            rlink = middle;
            middle = middle->left;
            middle->parent = nullptr;

        } else {
            if (!middle->right) {
                break;
            }

            if (compare(middle->right, node) < 0) {
                __sb_splaytree_left_rotate__(tree, middle);
                middle = middle->parent;

                if (middle->right == nullptr) {
                    break;
                }
            }

            if (llink) {
                llink->right = middle;
                middle->parent = llink;

            } else {
                ltree = middle;
            }

            llink = middle;
            middle = middle->right;
            middle->parent = nullptr;
        }
    }

    if (middle != nullptr) {
        if (llink != nullptr) {
            llink->right = middle->left;

        } else {
            llink = middle->left;
            ltree = llink;
        }
        if (rlink != nullptr) {
            rlink->left = middle->right;

        } else {
            rlink = middle->right;
            rtree = rlink;
        }

        if (llink && llink->right) {
            llink->right->parent = llink;
        }
        if (rlink && rlink->left) {
            rlink->left->parent = rlink;
        }

        if (ltree) {
            ltree->parent = middle;
        }
        if (rtree) {
            rtree->parent = middle;
        }

        middle->left = ltree;
        middle->right = rtree;
    }

    tree->root = middle;
}


/******************************************************************************
sb_void_t
sb_splaytree_td_splay(sb_splaytree_t *tree, sb_splaytree_node_t *node,
    sb_splaytree_compare_pt compare)
{
    sb_int_t compare_value;
    sb_splaytree_node_t *root = tree->root;
    sb_splaytree_node_t field = {nullptr, nullptr, nullptr, 0};
    sb_splaytree_node_t *ltree = &field, *rtree = &field;

    while ((root) && (compare_value = compare(root, node)) != 0) {

        if (compare_value > 0) {
            if (root->left == nullptr) {
                break;
            }

            if (compare(root->left, node) > 0) {
                __sb_splaytree_right_rotate__(tree, root);
                root = root->parent;
                if (root->left == nullptr) {
                    break;
                }
            }

            rtree->left = root;
            root->parent = rtree;
            rtree = rtree->left;
            root = root->left;
            root->parent = nullptr;
        } else {
            if (root->right == nullptr) {
                break;
            }

            if (compare(root->right, node) < 0) {
                __sb_splaytree_left_rotate__(tree, root);
                root = root->parent;
                if (root->right == nullptr) {
                    break;
                }
            }

            ltree->right = root;
            root->parent = ltree;
            ltree = ltree->right;
            root = root->right;
            root->parent = nullptr;
        }
    }

    if (root) {
        ltree->right = root->left;
        rtree->left = root->right;
        if (root->left) ltree->right->parent = ltree;
        if (root->right) rtree->left->parent = rtree;
        root->left = field.right;
        root->right = field.left;
        if (root->left) root->left->parent = root;
        if (root->right) root->right->parent = root;
    }

    tree->root = root;
}
******************************************************************************/


sb_void_t sb_splaytree_clear(sb_splaytree_t *tree, sb_splaytree_magic_pt magic)
{
    sb_splaytree_node_t *iterator, *next_iterator;

    if (tree->root) {
        sb_splaytree_safe_for_each(tree, iterator, next_iterator) {
            sb_splaytree_remove(tree, iterator);

            if (magic) {
                if (!magic(iterator)) {
                    return;
                }
            }
        }

        tree->root = nullptr;
    }
}


sb_void_t sb_splaytree_magic(sb_splaytree_t *tree, sb_splaytree_magic_pt magic)
{
    sb_splaytree_node_t *iterator;

    if (magic) {
        sb_splaytree_for_each(tree, iterator) {
            if (!magic(iterator)) {
                return;
            }
        }
    }
}


sb_bool_t sb_splaytree_is_same_tree(sb_splaytree_node_t *lhs,
    sb_splaytree_node_t *rhs, sb_splaytree_compare_pt compare)
{
    if (!lhs && !rhs)
        return true;

    if (lhs && rhs) {
        if ((compare(lhs, rhs) != 0) ||
            !sb_splaytree_is_same_tree(lhs->left, rhs->left, compare) ||
            !sb_splaytree_is_same_tree(lhs->right, rhs->right, compare)) {
            return false;
        }
        return true;
    }

    return false;
}


sb_splaytree_node_t *sb_splaytree_begin(sb_splaytree_t *tree)
{
    sb_splaytree_node_t *begin = tree->root;

    if (begin == nullptr) {
        return nullptr;
    }

    while (begin->left) {
        begin = begin->left;
    }

    return begin;
}


sb_splaytree_node_t *sb_splaytree_rbegin(sb_splaytree_t *tree)
{
    sb_splaytree_node_t *rbegin = tree->root;
    
    if (rbegin == nullptr) {
        return nullptr;
    }

    while (rbegin->right) {
        rbegin = rbegin->right;
    }

    return rbegin;
}


sb_splaytree_node_t *sb_splaytree_next(sb_splaytree_node_t *node)
{
    sb_splaytree_node_t *parent;

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


sb_splaytree_node_t *sb_splaytree_rnext(sb_splaytree_node_t *node)
{
    sb_splaytree_node_t *parent;

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
sb_void_t __sb_splaytree_left_rotate__(sb_splaytree_t *tree,
    sb_splaytree_node_t *node)
{
    sb_splaytree_node_t *right = node->right;
    sb_splaytree_node_t *parent = node->parent;

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
sb_void_t __sb_splaytree_right_rotate__(sb_splaytree_t *tree,
    sb_splaytree_node_t *node)
{
    sb_splaytree_node_t *left = node->left;
    sb_splaytree_node_t *parent = node->parent;

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
