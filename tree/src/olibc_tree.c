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
