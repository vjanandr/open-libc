#include <olibc_tree.h>
#include <CUnit/Basic.h>
olibc_tree_handle handle;
char *tree_name_ = "Test Tree";
int tree_data_[10] = {5, 7, 4, 2, 8, 1, 9, 3, 6, 10};

olibc_cbk_ret_type_t test_tree_cmp_func (void *tree_data, void *data)
{
   int tree_int, data_int;
   tree_int = *(int *)tree_data;
   data_int = *(int *)data;
   if (data_int > tree_int)
       return OLIBC_CBK_RET_GRTR;
   if (data_int < tree_int)
       return OLIBC_CBK_RET_LSR;
   return OLIBC_CBK_RET_EQUAL;
}

int test_pinit () {
    handle = NULL;
    return 1;
}
int test_pclean() {
    return 1;
}

void test_tree_create ()
{
    olibc_tree_init_t init_struct;
    memset(&init_struct, 0,sizeof(olibc_tree_init_t));
    init_struct.name = tree_name_;
    init_struct.cmp_func = test_tree_cmp_func;
    handle = olibc_tree_create(OLIBC_TREE_TYPE_BST, &init_struct);
    CU_ASSERT_PTR_NOT_NULL(handle);
}

void test_tree_name ()
{
    const char *name;
    olibc_retval_t retval;
    retval = olibc_tree_get_name(handle, &name);
    CU_ASSERT_TRUE(retval == OLIBC_RETVAL_SUCCESS);
    CU_ASSERT_STRING_EQUAL(name, tree_name_);
}
void test_tree_count ()
{
    olibc_retval_t retval;
    int count= 10;
    retval = olibc_tree_get_count(handle, &count);
    CU_ASSERT_TRUE(retval == OLIBC_RETVAL_SUCCESS);
    CU_ASSERT_EQUAL(count, 0);
}
void test_tree_destroy ()
{
    olibc_retval_t retval;
    retval = olibc_tree_destroy(&handle);
    CU_ASSERT_TRUE(retval == OLIBC_RETVAL_SUCCESS);
    CU_ASSERT_PTR_NULL(handle);
}

void test_tree_type ()
{
    olibc_retval_t retval;
    olibc_tree_type_t type;
    retval = olibc_tree_get_type(handle, &type);
    CU_ASSERT_TRUE(retval == OLIBC_RETVAL_SUCCESS);
    CU_ASSERT_EQUAL(type, OLIBC_TREE_TYPE_BST);
}
void test_tree_add ()
{
    int count, i =0;
    olibc_retval_t retval;

    while (i < 10) {
        retval = olibc_tree_add_data(handle, tree_data_+i);
        CU_ASSERT_TRUE(retval == OLIBC_RETVAL_SUCCESS);
        i++;
    }

    retval = olibc_tree_get_count(handle, &count);
    CU_ASSERT_TRUE(retval == OLIBC_RETVAL_SUCCESS);
    CU_ASSERT_EQUAL(count, 10);
}
void test_tree_walk_cbk (void *data)
{
    int d = *(int *)data;
    printf("%d ", d);
}

void test_tree_walk_inorder ()
{
    olibc_retval_t retval;

    retval = olibc_tree_walk(handle, test_tree_walk_cbk,
                             OLIBC_TREE_WALK_INORDER);
    CU_ASSERT_TRUE(retval == OLIBC_RETVAL_SUCCESS);
}

void test_tree_walk_preorder ()
{
    olibc_retval_t retval;

    retval = olibc_tree_walk(handle, test_tree_walk_cbk,
                             OLIBC_TREE_WALK_PREORDER);
    CU_ASSERT_TRUE(retval == OLIBC_RETVAL_SUCCESS);
}
void test_tree_walk_postorder ()
{
    olibc_retval_t retval;

    retval = olibc_tree_walk(handle, test_tree_walk_cbk,
                             OLIBC_TREE_WALK_POSTORDER);
    CU_ASSERT_TRUE(retval == OLIBC_RETVAL_SUCCESS);
}
void test_tree_dup_check ()
{
    int count;
    olibc_retval_t retval;

    retval = olibc_tree_add_data(handle, tree_data_+0);
    CU_ASSERT_TRUE(retval == OLIBC_RETVAL_DUPLICATE_DATA);

    retval = olibc_tree_get_count(handle, &count);
    CU_ASSERT_TRUE(retval == OLIBC_RETVAL_SUCCESS);
    CU_ASSERT_EQUAL(count, 10);
}

void test_tree_level ()
{
    int level = 0;
    olibc_retval_t retval;
    printf(" data = %d - ", *(tree_data_+1));
    retval = olibc_tree_get_type_level(handle, tree_data_+1, &level);
    CU_ASSERT_TRUE(retval == OLIBC_RETVAL_SUCCESS);
    CU_ASSERT_TRUE(level != 0);
    printf(" %d ", level);
    level = 0;
    retval = olibc_tree_get_level(handle, tree_data_+1, &level);
    CU_ASSERT_TRUE(retval == OLIBC_RETVAL_SUCCESS);
    CU_ASSERT_TRUE(level != 0);
    printf(" %d ", level);
}
void test_print_func (void *data, int level)
{
    level--;
    level *= 10;
    while (level--) printf(" ");
    printf("%d\n\n\n", *(int *)data);
}
void test_tree_print ()
{
    olibc_retval_t retval;
    printf("\n");
    retval = olibc_tree_print(handle, test_print_func);
    CU_ASSERT_TRUE(retval == OLIBC_RETVAL_SUCCESS);
}
int main ()
{
    if (CUE_SUCCESS != CU_initialize_registry()) {
        return CU_get_error();
    }
    CU_pSuite  psuite = NULL;

    psuite = CU_add_suite("Tree test", NULL, NULL);
    if (psuite == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (CU_add_test(psuite, "test_tree_create",
                test_tree_create) == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    if (CU_add_test(psuite, "test_tree_name",
                test_tree_name) == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    if (CU_add_test(psuite, "test_tree_count",
                test_tree_count) == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    if (CU_add_test(psuite, "test_tree_type",
                test_tree_type) == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    if (CU_add_test(psuite, "test_tree_add",
                test_tree_add) == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    if (CU_add_test(psuite, "test_tree_dup_check",
                test_tree_dup_check) == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    if (CU_add_test(psuite, "test_tree_walk_inorder",
                test_tree_walk_inorder) == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    if (CU_add_test(psuite, "test_tree_walk_preorder",
                test_tree_walk_preorder) == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    if (CU_add_test(psuite, "test_tree_walk_postorder",
                test_tree_walk_postorder) == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    if (CU_add_test(psuite, "test_tree_level",
                test_tree_level) == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    if (CU_add_test(psuite, "test_tree_print",
                test_tree_print) == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    if (CU_add_test(psuite, "test_tree_destroy",
                test_tree_destroy) == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    handle = NULL;
    return CU_get_error();
}
