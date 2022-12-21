#include "umgmt/group.h"
#include "umgmt/types.h"
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdlib.h>

#include <umgmt.h>

#include "umgmt/group.c"

#define UM_GROUP_T_SIZE sizeof(um_group_t)

void *__wrap_malloc(size_t size);
void *__real_malloc(size_t size);

static void test_group_new_correct(void **state);
static void test_group_new_incorrect(void **state);

int main(void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_group_new_correct),
        cmocka_unit_test(test_group_new_incorrect),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}

static void test_group_new_correct(void **state)
{
    (void)state;

    um_group_t *group = NULL;

    assert_null(group);

    expect_value(__wrap_malloc, size, UM_GROUP_T_SIZE);
    will_return(__wrap_malloc, __real_malloc(UM_GROUP_T_SIZE));

    group = um_group_new();
    assert_non_null(group);

    // assert all fields are set to NULL
    assert_null(um_group_get_name(group));
    assert_null(um_group_get_password(group));
    assert_null(um_group_get_password_hash(group));
    assert_null(um_group_get_members_head(group));
    assert_null(um_group_get_admin_head(group));
    assert_int_equal(um_group_get_gid(group), 0);

    um_group_free(group);
}

static void test_group_new_incorrect(void **state)
{
    (void)state;

    um_group_t *group = NULL;

    assert_null(group);

    expect_value(__wrap_malloc, size, UM_GROUP_T_SIZE);
    will_return(__wrap_malloc, NULL);

    group = um_group_new();
    assert_null(group);
}

void *__wrap_malloc(size_t size)
{
    check_expected(size);
    return mock_ptr_type(void *);
}