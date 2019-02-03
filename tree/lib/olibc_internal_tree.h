#ifndef __OLIBC_INTERNAL_TREE_H__
#define __OLIBC_INTERNAL_TREE_H__
#include <include/olibc_tree.h>

typedef struct olibc_tree_node_ {
    int data;
    struct olibc_tree_node_ *left;
    struct olibc_tree_node_ *right;
} olibc_tree_node_t;

typedef struct olibc_tree_head_t_ {
    olibc_tree_node_t  *head;
    int count;
} olibc_tree_head_t;

extern t_handle* tree_create_new();
extern tree_handle

//__OLIBC_INTERNAL_TREE_H__
