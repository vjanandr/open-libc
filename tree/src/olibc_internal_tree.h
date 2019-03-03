#ifndef __OLIBC_INTERNAL_TREE_H__
#define __OLIBC_INTERNAL_TREE_H__
#include <olibc_tree.h>

typedef int (*olibc_tree_level_func) (struct olibc_tree_head_t_ *tree,
                                      void *data);

typedef struct olibc_tree_node_ {
    void* data;
    struct olibc_tree_node_ *left;
    struct olibc_tree_node_ *right;
} olibc_tree_node_t;

typedef struct olibc_tree_head_t_ {
    olibc_tree_node_t  *head;
    int count;
    olibc_tree_type_t tree_type;
    olibc_tree_cmp_cbk cmp_cbk;
    olibc_tree_dlt_cbk dlt_cbk;
    olibc_tree_level_func level_func;
    char *name;
} olibc_tree_head_t;

#endif //__OLIBC_INTERNAL_TREE_H__
