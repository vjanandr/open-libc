#include <olibc_tree.h>
#include <CUnit/Basic.h>

int test_pinit () {
    printf("\n in test_pinit");
    return 1;
}
int test_pclean() {
    printf("\n in test_clean");
    return 1;
}

void test_tree_create () {
    CU_ASSERT_EQUAL(1,1);
    CU_ASSERT_EQUAL(1,1);
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
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    return CU_get_error();
}
