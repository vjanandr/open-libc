#ifndef __OLIBC_TREE_H__
#define __OLIBC_TREE_H__
#include <stdio.h>
#include <olibc_defs.h>
#include <stdbool.h>

#define FOREACH_TREETYPE(TYPE) \
        TYPE(OLIBC_TREE_TYPE_BST) \
        TYPE(OLIBC_TREE_TYPE_AVL)

typedef enum olibc_tree_walk_type_ {
    OLIBC_TREE_WALK_INORDER,
    OLIBC_TREE_WALK_PREORDER,
    OLIBC_TREE_WALK_POSTORDER
} olibc_tree_walk_type_t;

typedef olibc_cbk_ret_type_t (*olibc_tree_cmp_func)(void *treedata,
                                                    void *data);
typedef void (*olibc_tree_print_func)(void *data,
                                                      int level);

typedef void (*olibc_tree_walk_func)(void *data);

typedef enum olibc_tree_type_t_ {
    FOREACH_TREETYPE(GENERATE_ENUM)
} olibc_tree_type_t;

static const char *olibc_tree_type_string[] = {
    FOREACH_TREETYPE(GENERATE_STRING)
};

typedef struct olibc_tree_init_t_ {
    char *name;
    olibc_tree_cmp_func cmp_func;
} olibc_tree_init_t;
/*
typedef enum olibc_tree_type_t_ {
    OLIBC_TREE_AVL,
} olibc_tree_type_t;
*/
typedef struct olibc_tree_head_t_ *olibc_tree_handle;

extern olibc_tree_handle olibc_tree_create(olibc_tree_type_t type,
                                           olibc_tree_init_t *init_struct);
extern olibc_retval_t olibc_tree_get_count(olibc_tree_handle handle,
                                           int *count);
extern olibc_retval_t olibc_tree_get_name(olibc_tree_handle handle,
                                          const char **name);
extern olibc_retval_t olibc_tree_destroy(olibc_tree_handle *handle);
extern olibc_retval_t olibc_tree_get_type(olibc_tree_handle handle,
                                          olibc_tree_type_t *type);
extern olibc_retval_t olibc_tree_add_data(olibc_tree_handle handle,
                                          void *data);
extern olibc_retval_t olibc_tree_walk(olibc_tree_handle handle,
                                      olibc_tree_walk_func walk_cbk,
                                      olibc_tree_walk_type_t walk_type);
extern olibc_retval_t olibc_tree_get_type_level(olibc_tree_handle handle,
                                           void *data, uint32_t *level);
extern olibc_retval_t olibc_tree_get_level(olibc_tree_handle handle,
                                           void *data, uint32_t *level);
extern olibc_retval_t olibc_tree_print(olibc_tree_handle handle,
                                       olibc_tree_print_func print_func);
#endif //__OLIBC_TREE_H__
