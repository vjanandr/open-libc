#include "olibc_internal_tree.h"
#include <malloc.h>
#include <string.h>

olibc_tree_handle olibc_tree_create (char *name)
{
    olibc_tree_head_t *tree = NULL;
    tree = malloc(sizeof(olibc_tree_head_t));
    if (!tree) return (NULL);
    memset(tree, 0, sizeof(*tree));
    tree->name = name;
    return tree;
}

olibc_retval_t olibc_tree_get_name (olibc_tree_handle handle , const char **name)
{
    olibc_tree_head_t *tree = NULL;
    if (!handle) {
        return OLIBC_RETVAL_FAILURE;
    }
    tree = handle;
    *name = tree->name;
    return (OLIBC_RETVAL_SUCCESS);
}

olibc_retval_t olibc_tree_get_count (olibc_tree_handle handle, int *count)
{
    olibc_tree_head_t *tree = NULL;
    if (!handle || ! count) {
        return OLIBC_RETVAL_FAILURE; 
    }
    tree = handle;
    *count = tree->count;
    return (OLIBC_RETVAL_SUCCESS);
}
