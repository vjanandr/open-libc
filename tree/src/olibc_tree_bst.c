#include "olibc_internal_tree.h"
#include <malloc.h>

olibc_tree_node_t *
olibc_tree_node_new (void *data)
{
    olibc_tree_node_t *new_node =
        malloc(sizeof(olibc_tree_node_t));
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
}

