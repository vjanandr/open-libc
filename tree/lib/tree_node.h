#ifndef __TREE_NODE_H__
#define __TREE_NODE_H__
#include <stdio.h>


typedef struct t_node_ {
    int data;
    struct t_node_ *left;
    struct t_node_ *right;
} t_node;

typdef struct t_head_ {
    t_node *head;
    int count;
} t_handle;

extern t_handle* tree_create_new();
extern tree_handle

#endif //__TREE_NODE_H__
