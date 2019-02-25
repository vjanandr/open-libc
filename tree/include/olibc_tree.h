#ifndef __OLIBC_TREE_H__
#define __OLIBC_TREE_H__
#include <stdio.h>
#include <olibc_defs.h>

typedef struct olibc_tree_head_t_ *olibc_tree_handle;
typedef struct olibc_tree_node_t_ *olibc_tree_node_handle;

extern olibc_tree_handle olibc_tree_create(char *name);
extern olibc_retval_t olibc_tree_get_count(olibc_tree_handle handle, int *count);
extern olibc_retval_t olibc_tree_get_name(olibc_tree_handle handle,
                                        const char **name);
extern olibc_retval_t olibc_tree_destroy(olibc_tree_handle *handle);
#endif //__OLIBC_TREE_H__
