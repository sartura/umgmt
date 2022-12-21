#include "umgmt/group.h"
#include "umgmt/types.h"
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdio.h>
#include <stdlib.h>

#include <umgmt.h>
#include "common.h"
#include "umgmt/group.c"

#define UM_GROUP_T_SIZE sizeof(um_group_t)

static void test_group_new_correct(void **state);
static void test_group_new_incorrect(void **state);

static void test_group_set_name_correct(void **state);
static void test_group_set_name_incorrect(void **state);

static void test_group_set_password_correct(void **state);
static void test_group_set_password_incorrect(void **state);

static void test_group_set_password_hash_correct(void **state);
static void test_group_set_password_hash_incorrect(void **state);

int main(void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_group_new_correct),
        cmocka_unit_test(test_group_new_incorrect),
        cmocka_unit_test(test_group_set_name_correct),
        cmocka_unit_test(test_group_set_name_incorrect),
        cmocka_unit_test(test_group_set_password_correct),
        cmocka_unit_test(test_group_set_password_incorrect),
        cmocka_unit_test(test_group_set_password_hash_correct),
        cmocka_unit_test(test_group_set_password_hash_incorrect),
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

static void test_group_set_name_correct(void **state)
{
    (void)state;

    int error = 0;
    um_group_t *group = NULL;
    const char *group_name = "Group1";

    assert_null(group);

    expect_value(__wrap_malloc, size, UM_GROUP_T_SIZE);
    will_return(__wrap_malloc, __real_malloc(UM_GROUP_T_SIZE));

    group = um_group_new();
    assert_non_null(group);

    expect_string(__wrap_strdup, s, group_name);
    will_return(__wrap_strdup, __real_strdup(group_name));

    error = um_group_set_name(group, group_name);
    assert_int_equal(error, 0);

    um_group_free(group);
}

static void test_group_set_name_incorrect(void **state)
{
    (void)state;

    int error = 0;
    um_group_t *group = NULL;
    const char *group_name = "Group1";

    assert_null(group);

    expect_value(__wrap_malloc, size, UM_GROUP_T_SIZE);
    will_return(__wrap_malloc, __real_malloc(UM_GROUP_T_SIZE));

    group = um_group_new();
    assert_non_null(group);

    expect_string(__wrap_strdup, s, group_name);
    will_return(__wrap_strdup, NULL);

    error = um_group_set_name(group, group_name);
    assert_int_equal(error, -1);

    um_group_free(group);
}

static void test_group_set_password_correct(void **state)
{
    (void)state;

    int error = 0;
    um_group_t *group = NULL;
    const char *group_password = "";

    assert_null(group);

    expect_value(__wrap_malloc, size, UM_GROUP_T_SIZE);
    will_return(__wrap_malloc, __real_malloc(UM_GROUP_T_SIZE));

    group = um_group_new();
    assert_non_null(group);

    expect_string(__wrap_strdup, s, group_password);
    will_return(__wrap_strdup, __real_strdup(group_password));

    error = um_group_set_password(group, group_password);
    assert_int_equal(error, 0);

    um_group_free(group);
}

static void test_group_set_password_incorrect(void **state)
{
    (void)state;

    int error = 0;
    um_group_t *group = NULL;
    const char *group_password = "";

    assert_null(group);

    expect_value(__wrap_malloc, size, UM_GROUP_T_SIZE);
    will_return(__wrap_malloc, __real_malloc(UM_GROUP_T_SIZE));

    group = um_group_new();
    assert_non_null(group);

    expect_string(__wrap_strdup, s, group_password);
    will_return(__wrap_strdup, NULL);

    error = um_group_set_password(group, group_password);
    assert_int_equal(error, -1);

    um_group_free(group);
}

static void test_group_set_password_hash_correct(void **state)
{
    (void)state;

    int error = 0;
    um_group_t *group = NULL;
    const char *password_hash = "d80361e865ac54b8e409f3c908bc9803adff7633c8793881877ae46295c5746e965f39e2ddce8f42db1afd"
                                "65cbc86b54f638e1fd37e3c44343428337e05f59ab";

    assert_null(group);

    expect_value(__wrap_malloc, size, UM_GROUP_T_SIZE);
    will_return(__wrap_malloc, __real_malloc(UM_GROUP_T_SIZE));

    group = um_group_new();
    assert_non_null(group);

    expect_string(__wrap_strdup, s, password_hash);
    will_return(__wrap_strdup, __real_strdup(password_hash));

    error = um_group_set_password_hash(group, password_hash);
    assert_int_equal(error, 0);

    um_group_free(group);
}

static void test_group_set_password_hash_incorrect(void **state)
{
    (void)state;

    int error = 0;
    um_group_t *group = NULL;
    const char *password_hash = "d80361e865ac54b8e409f3c908bc9803adff7633c8793881877ae46295c5746e965f39e2ddce8f42db1afd"
                                "65cbc86b54f638e1fd37e3c44343428337e05f59ab";

    assert_null(group);

    expect_value(__wrap_malloc, size, UM_GROUP_T_SIZE);
    will_return(__wrap_malloc, __real_malloc(UM_GROUP_T_SIZE));

    group = um_group_new();
    assert_non_null(group);

    expect_string(__wrap_strdup, s, password_hash);
    will_return(__wrap_strdup, NULL);

    error = um_group_set_password_hash(group, password_hash);
    assert_int_equal(error, -1);

    um_group_free(group);
}