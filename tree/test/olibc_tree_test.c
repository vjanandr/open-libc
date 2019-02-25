#include <olibc_tree.h>
#include <CUnit/Basic.h>
olibc_tree_handle handle;
char *tree_name = "Test Tree";

int test_pinit () {
    handle = NULL;
    return 1;
}
int test_pclean() {
    return 1;
}

void test_tree_create ()
{
    handle = olibc_tree_create(tree_name);
    CU_ASSERT_PTR_NOT_NULL(handle);
}

void test_tree_name ()
{
    const char *name;
    olibc_retval_t retval;
    retval = olibc_tree_get_name(handle, &name);
    CU_ASSERT_TRUE(retval == OLIBC_RETVAL_SUCCESS);
    CU_ASSERT_STRING_EQUAL(name, tree_name);
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

    if (CU_add_test(psuite, "test_tree_create", test_tree_create) == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    if (CU_add_test(psuite, "test_tree_name", test_tree_name) == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    if (CU_add_test(psuite, "test_tree_count", test_tree_count) == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    if (CU_add_test(psuite, "test_tree_destroy", test_tree_destroy) == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    handle = NULL;
    return CU_get_error();
}
