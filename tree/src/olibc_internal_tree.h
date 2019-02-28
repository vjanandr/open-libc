#ifndef __OLIBC_INTERNAL_TREE_H__
#define __OLIBC_INTERNAL_TREE_H__
#include <olibc_tree.h>

typedef struct olibc_tree_node_ {
    void* data;
    struct olibc_tree_node_ *left;
    struct olibc_tree_node_ *right;
} olibc_tree_node_t;

typedef struct olibc_tree_head_t_ {
    olibc_tree_node_t  *head;
    int count;
    olibc_tree_type_t tree_type;
    olibc_tree_cmp_func cmp_func;
    char *name;
} olibc_tree_head_t;

#endif //__OLIBC_INTERNAL_TREE_H__
