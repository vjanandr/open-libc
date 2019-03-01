#include "olibc_internal_tree.h"
#include <malloc.h>
#include <string.h>

olibc_retval_t
olibc_tree_bst_add_data(olibc_tree_head_t *tree, void *data);

olibc_tree_handle
olibc_tree_create (olibc_tree_type_t tree_type,
                                     olibc_tree_init_t *init_struct)
{
    if (!init_struct) {
        return NULL;
    }
    olibc_tree_head_t *tree = NULL;
    tree = malloc(sizeof(olibc_tree_head_t));
    if (!tree) return (NULL);
    memset(tree, 0, sizeof(*tree));
    tree->name = init_struct->name;
    tree->cmp_func = init_struct->cmp_func;
    tree->tree_type = tree_type;
    return tree;
}

olibc_retval_t
olibc_tree_get_name (olibc_tree_handle handle , const char **name)
{
    olibc_tree_head_t *tree = NULL;
    if (!handle) {
        return OLIBC_RETVAL_FAILURE;
    }
    tree = handle;
    *name = tree->name;
    return (OLIBC_RETVAL_SUCCESS);
}

olibc_retval_t
olibc_tree_get_count (olibc_tree_handle handle, int *count)
{
    olibc_tree_head_t *tree = NULL;
    if (!handle || ! count) {
        return OLIBC_RETVAL_FAILURE;
    }
    tree = handle;
    *count = tree->count;
    return (OLIBC_RETVAL_SUCCESS);
}

olibc_retval_t
olibc_tree_destroy (olibc_tree_handle *handle)
{
    olibc_tree_head_t *tree = *handle;
    free (tree);
    *handle = NULL;
    return (OLIBC_RETVAL_SUCCESS);
}

olibc_retval_t
olibc_tree_get_type (olibc_tree_handle handle,
                     olibc_tree_type_t *type)
{
    olibc_tree_head_t *tree = NULL;

    if (!handle || !type) {
        return OLIBC_RETVAL_FAILURE;
    }
    tree = handle;
    *type = tree->tree_type;
    return (OLIBC_RETVAL_SUCCESS);
}

olibc_retval_t
olibc_tree_add_data (olibc_tree_handle handle,
                     void *data)
{
    olibc_tree_head_t *tree = NULL;

    if (!handle) {
        return OLIBC_RETVAL_FAILURE;
    }
    tree = handle;
    switch (tree->tree_type) {
        case OLIBC_TREE_TYPE_BST:
            return (olibc_tree_bst_add_data(tree,data));
        case OLIBC_TREE_TYPE_AVL:
        default:
            return OLIBC_RETVAL_FAILURE;
    }
    return OLIBC_RETVAL_FAILURE;
}

void
olibc_tree_walk_indorder (olibc_tree_node_t *node,
                          olibc_tree_walk_func walk_cbk)
{
    if (!node) {
        return;
    }
    if (node->left)
        olibc_tree_walk_indorder(node->left, walk_cbk);

    walk_cbk(node->data);

    if (node->right)
        olibc_tree_walk_indorder(node->right, walk_cbk);
}

void
olibc_tree_walk_preorder (olibc_tree_node_t *node,
                          olibc_tree_walk_func walk_cbk)
{
    if (!node) {
        return;
    }
    walk_cbk(node->data);

    if (node->left)
        olibc_tree_walk_preorder(node->left, walk_cbk);

    if (node->right)
        olibc_tree_walk_preorder(node->right, walk_cbk);
}


void
olibc_tree_walk_postorder (olibc_tree_node_t *node,
                          olibc_tree_walk_func walk_cbk)
{
    if (!node) {
        return;
    }

    if (node->left)
        olibc_tree_walk_postorder(node->left, walk_cbk);

    if (node->right)
        olibc_tree_walk_postorder(node->right, walk_cbk);

    walk_cbk(node->data);
}

olibc_retval_t
olibc_tree_walk (olibc_tree_handle handle,
                 olibc_tree_walk_func walk_cbk,
                 olibc_tree_walk_type_t walk_type)
{

    olibc_tree_head_t *tree = NULL;
    olibc_tree_node_t *tree_node = NULL;

    if (!handle) {
        return OLIBC_RETVAL_FAILURE;
    }
    tree = handle;
    tree_node = tree->head;
    switch (walk_type) {
        case OLIBC_TREE_WALK_INORDER:
            olibc_tree_walk_indorder(tree_node, walk_cbk);
            break;
        case OLIBC_TREE_WALK_PREORDER:
            olibc_tree_walk_preorder(tree_node, walk_cbk);
            break;
        case OLIBC_TREE_WALK_POSTORDER:
            olibc_tree_walk_postorder(tree_node, walk_cbk);
            break;
    }
    return OLIBC_RETVAL_SUCCESS;
}

olibc_tree_node_t*
olibc_tree_node_new (void *data)
{
    olibc_tree_node_t *new_node =
        malloc(sizeof(olibc_tree_node_t));
    memset(new_node, 0, sizeof(olibc_tree_node_t));
    new_node->data = data;
    return (new_node);

}

olibc_retval_t
olibc_tree_insert_node (olibc_tree_node_t *tree_node,
                        void *data,
                        olibc_tree_cmp_func cbk)
{
    if (!tree_node) {
        return OLIBC_RETVAL_FAILURE;
    }

    olibc_cbk_ret_type_t cbk_retval = cbk(tree_node->data, data);

    switch (cbk_retval) {
        case OLIBC_CBK_RET_EQUAL:
            return OLIBC_RETVAL_DUPLICATE_DATA;
        case OLIBC_CBK_RET_GRTR:
            if (tree_node->right) {
                return olibc_tree_insert_node(tree_node->right, data, cbk);
            } else {
                tree_node->right =  olibc_tree_node_new(data);
                return OLIBC_RETVAL_SUCCESS;
            }
            break;
        case OLIBC_CBK_RET_LSR:
            if (tree_node->left) {
                return olibc_tree_insert_node(tree_node->left, data, cbk);
            } else {
                tree_node->left = olibc_tree_node_new(data);
                return OLIBC_RETVAL_SUCCESS;
            }
            break;
        default:
            return OLIBC_RETVAL_INVALID_SWITCH;
    }
}

olibc_retval_t
olibc_tree_bst_add_data (olibc_tree_head_t *tree, void *data)
{
    olibc_retval_t retval;
    if (!tree->head) {
        tree->head = olibc_tree_node_new(data);
        tree->count++;
        return (OLIBC_RETVAL_SUCCESS);
    }
    retval = olibc_tree_insert_node(tree->head, data,
                                    tree->cmp_func);
    if (retval == OLIBC_RETVAL_SUCCESS)
        tree->count++;
    return retval;
}

