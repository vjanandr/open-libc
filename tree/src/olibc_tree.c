#include "olibc_internal_tree.h"
#include <malloc.h>
#include <string.h>

olibc_retval_t
olibc_tree_bst_add_data(olibc_tree_head_t *tree, void *data);
//olibc_retval_t
//olibc_tree_bst_get_level(olibc_tree_head_t *tree, void *data);

olibc_tree_handle
olibc_tree_create (olibc_tree_type_t tree_type,
                                     olibc_tree_init_t *init_struct)
{
    if (!init_struct) {
        return NULL;
    }
    if (!init_struct->cmp_cbk || !init_struct->dlt_cbk)
        return NULL;

    olibc_tree_head_t *tree = NULL;
    tree = malloc(sizeof(olibc_tree_head_t));
    if (!tree) return (NULL);
    memset(tree, 0, sizeof(*tree));
    tree->name = init_struct->name;
    tree->cmp_cbk = init_struct->cmp_cbk;
    tree->dlt_cbk = init_struct->dlt_cbk;
    tree->tree_type = tree_type;
//    tree->level_cbk = olibc_tree_bst_get_level;
    return tree;
}

// preorder traversal -- root L R
// A node's inorder successor is its right successors left most node.
// A node's inorder predecessor is its left successors right most node.
olibc_retval_t
olibc_tree_delete_data_util (olibc_tree_node_t *tree_node,
                             olibc_tree_cmp_cbk cmp_cbk,
                             olibc_tree_dlt_cbk dlt_cbk,
                             bool *data_found,
                             void *data)
{
    if (!tree_node) {
        return OLIBC_RETVAL_SUCCESS;
    }
    olibc_cbk_ret_type_t cbk_retval = cmp_cbk(tree_node->data, data);

    switch (cbk_retval) {
        case OLIBC_CBK_RET_EQUAL:
            *data_found = true;
            dlt_cbk(data);
            return OLIBC_RETVAL_SUCCESS;
        case OLIBC_CBK_RET_GRTR:
            return olibc_tree_delete_data_util(tree_node->right, cmp_cbk,
                                               dlt_cbk, data_found, data);
        case OLIBC_CBK_RET_LSR:
            return olibc_tree_delete_data_util(tree_node->left, cmp_cbk,
                                               dlt_cbk,data_found,data);
        default:
            return 0;
    }
    return OLIBC_RETVAL_SUCCESS;
}

olibc_retval_t
olibc_tree_delete_data (olibc_tree_handle handle, void *data)
{
    olibc_tree_head_t *tree = NULL;
    olibc_retval_t retval;
    bool data_found = false;
    if (!handle) {
        return OLIBC_RETVAL_FAILURE;
    }
    tree = handle;

    retval = olibc_tree_delete_data_util(tree->head, tree->cmp_cbk,
            tree->dlt_cbk, &data_found, data);
    if (!data_found && retval == OLIBC_RETVAL_SUCCESS) {
        return OLIBC_RETVAL_DATA_NOT_FOUND;
    }
    return retval;
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
    if (!tree)
        return OLIBC_RETVAL_FAILURE;
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
                          olibc_tree_walk_cbk walk_cbk)
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
                          olibc_tree_walk_cbk walk_cbk)
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
                          olibc_tree_walk_cbk walk_cbk)
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
                 olibc_tree_walk_cbk walk_cbk,
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

int
olibc_tree_bst_get_level (olibc_tree_node_t *tree_node, void *data,
                          olibc_tree_cmp_cbk cbk, int curr_level)
{
    if (!tree_node)
        return 0;

    olibc_cbk_ret_type_t cbk_retval = cbk(tree_node->data, data);

    switch (cbk_retval) {
        case OLIBC_CBK_RET_EQUAL:
            return curr_level;
        case OLIBC_CBK_RET_GRTR:
            return olibc_tree_bst_get_level(tree_node->right, data,
                                            cbk, curr_level+1);
        case OLIBC_CBK_RET_LSR:
            return olibc_tree_bst_get_level(tree_node->left, data,
                                            cbk, curr_level+1);
        default:
            return 0;
    }

}
int
olibc_tree_get_level_util (olibc_tree_node_t *tree_node, void *data,
                           olibc_tree_cmp_cbk cbk, int curr_level)
{
    int ret_level;
    if (!tree_node)
        return 0;

    if (cbk(tree_node->data, data) == OLIBC_CBK_RET_EQUAL) {
        return curr_level;
    }

    ret_level = olibc_tree_get_level_util(tree_node->left, data, cbk, curr_level+1);

    if (ret_level != 0) {
        // Found the node return it.
        return (ret_level);
    }
    ret_level = olibc_tree_get_level_util(tree_node->right, data, cbk, curr_level+1);
    return (ret_level);
}

olibc_retval_t
olibc_tree_get_type_level (olibc_tree_handle handle, void *data,
                      uint32_t *level)
{
    olibc_tree_head_t *tree = NULL;
    olibc_tree_node_t *tree_node = NULL;

    if (!handle ||!level || !data) {
        return OLIBC_RETVAL_FAILURE;
    }
    tree = handle;
    tree_node = tree->head;
    *level = 0;
    *level = olibc_tree_bst_get_level(tree_node, data, tree->cmp_cbk, 1);
    return OLIBC_RETVAL_SUCCESS;
}

olibc_retval_t
olibc_tree_get_level (olibc_tree_handle handle, void *data,
                      uint32_t *level)
{
    olibc_tree_head_t *tree = NULL;
    olibc_tree_node_t *tree_node = NULL;

    if (!handle ||!level || !data) {
        return OLIBC_RETVAL_FAILURE;
    }
    tree = handle;
    tree_node = tree->head;
    *level = 0;
    *level = olibc_tree_get_level_util(tree_node, data, tree->cmp_cbk, 1);
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
                        olibc_tree_cmp_cbk cbk)
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
                                    tree->cmp_cbk);
    if (retval == OLIBC_RETVAL_SUCCESS)
        tree->count++;
    return retval;
}

void  olibc_tree_print_util (olibc_tree_node_t *node,
                             olibc_tree_print_cbk print_cbk,
                             int level)
{
    if (!node)
        return;
    olibc_tree_print_util(node->right, print_cbk,level+1);
    print_cbk(node->data, level);
    olibc_tree_print_util(node->left, print_cbk,level+1);
}

olibc_retval_t olibc_tree_print (olibc_tree_handle handle,
                                olibc_tree_print_cbk print_cbk)
{
    olibc_tree_head_t *tree =handle;
    if (!handle || !print_cbk)
        return OLIBC_RETVAL_FAILURE;

    olibc_tree_print_util(tree->head, print_cbk, 1);
    return OLIBC_RETVAL_SUCCESS;
}
