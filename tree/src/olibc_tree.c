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

olibc_tree_node_t*
olibc_get_inorder_successor_util (olibc_tree_node_t * tree_node)
{
    if (tree_node)
        while(tree_node->left) {
            tree_node = tree_node->left;
        }
    return tree_node;
}
// preorder traversal -- root L R
// A node's inorder successor is its right successors left most node.
// A node's i}norder predecessor is its left successors right most node.
// IF there is a match the function will return the new node that will replace the matched node.
olibc_tree_node_t*
olibc_tree_delete_data_util (olibc_tree_node_t *tree_node,
                             olibc_tree_cmp_cbk cmp_cbk,
                             olibc_tree_node_dlt_cbk dlt_cbk,
                             bool *data_found,
                             void *data)
{
    olibc_tree_node_t *new_node = NULL;
//    olibc_tree_node_t *new_node = NULL;

    if (!tree_node) {
        return NULL;
    }
    olibc_cbk_ret_type_t cbk_retval = cmp_cbk(tree_node->data, data);

    switch (cbk_retval) {
        case OLIBC_CBK_RET_EQUAL:
            *data_found = true;
            if (dlt_cbk)
                dlt_cbk(data);
            if (tree_node->left == NULL) {
                new_node = tree_node->right;
                free(tree_node);
                return new_node;
            }
            if (tree_node->right == NULL) {
                new_node = tree_node->left;
                free(tree_node);
                return new_node;
            }
            /*
            // Both left and right nodes exists.
            // find the inorder successor ie left most node in the right subtree.
            curr = tree_node->right;
            prev = tree_node;
            while (curr->left) {
                prev = curr;
                curr = curr->left;
            }
            if (tree_node->right == curr) {
                tree_node->right = curr->right;
            }
            prev->left = curr->right;
            tree_node->data = curr->data;
            free(curr);
            */
            new_node = olibc_get_inorder_successor_util(tree_node->right);
            tree_node->data = new_node->data;
            tree_node->right = olibc_tree_delete_data_util(tree_node->right,
                    cmp_cbk, NULL, data_found,tree_node->data);

            // We could have called delete_data_util again recursively with curr->data as key.
            // But we already know that curr->left is NULL and hence prev->node left should be curr->right.
            // This is just to avoid recursively calling again until we reach the left most node.
            return tree_node;
        case OLIBC_CBK_RET_GRTR:
            tree_node->right = olibc_tree_delete_data_util(tree_node->right, cmp_cbk,
                                               dlt_cbk, data_found, data);
        case OLIBC_CBK_RET_LSR:
            tree_node->left = olibc_tree_delete_data_util(tree_node->left, cmp_cbk,
                                               dlt_cbk, data_found, data);
        default:
            return tree_node;
    }
    return tree_node;
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

    tree->head = olibc_tree_delete_data_util(tree->head, tree->cmp_cbk,
                                             tree->dlt_cbk, &data_found, data);
    if (!data_found) {
        return OLIBC_RETVAL_DATA_NOT_FOUND;
    }
    tree->count -= 1;
    return OLIBC_RETVAL_SUCCESS;
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
olibc_tree_node_t*
olibc_tree_postorder_del (olibc_tree_node_t *node, 
                          olibc_tree_node_dlt_cbk dlt_cbk)
{
    if (!node) {
        return NULL;
    }

    if (node->left)
        node->left = olibc_tree_postorder_del(node->left, 
                                               dlt_cbk);

    if (node->right)
        node->right = olibc_tree_postorder_del(node->right, 
                                                dlt_cbk);

    dlt_cbk(node->data);
    free(node);
    return NULL;
}

olibc_retval_t
olibc_tree_destroy (olibc_tree_handle *handle)
{
    olibc_tree_head_t *tree = *handle;
    if (!tree)
        return OLIBC_RETVAL_FAILURE;
    // postorder walk.
    tree->head = olibc_tree_postorder_del(tree->head, tree->dlt_cbk);
    free(tree);
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
