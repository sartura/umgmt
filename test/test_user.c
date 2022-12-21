#include "umgmt/user.h"
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdio.h>

#include <umgmt.h>

// wrapped functions
char *__wrap_strdup(const char *s)
{
    check_expected(s);
    return (char *)mock();
}

static void test_user_new_correct(void **state);
static void test_user_new_incorrect(void **state);

static void test_user_set_name_correct(void **state);
static void test_user_set_name_incorrect(void **state);

static void test_user_set_password_correct(void **state);
static void test_user_set_password_incorrect(void **state);

static void test_user_set_gecos_correct(void **state);
static void test_user_set_gecos_incorrect(void **state);

static void test_user_set_home_path_correct(void **state);
static void test_user_set_home_path_incorrect(void **state);

static void test_user_set_shell_path_correct(void **state);
static void test_user_set_shell_path_incorrect(void **state);

int main(void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_user_new_correct),
        cmocka_unit_test(test_user_new_incorrect),
        cmocka_unit_test(test_user_set_name_correct),
        cmocka_unit_test(test_user_set_name_incorrect),
        cmocka_unit_test(test_user_set_password_correct),
        cmocka_unit_test(test_user_set_password_incorrect),
        cmocka_unit_test(test_user_set_gecos_correct),
        cmocka_unit_test(test_user_set_gecos_incorrect),
        cmocka_unit_test(test_user_set_home_path_correct),
        cmocka_unit_test(test_user_set_home_path_incorrect),
        cmocka_unit_test(test_user_set_shell_path_correct),
        cmocka_unit_test(test_user_set_shell_path_incorrect),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}

static void test_user_new_correct(void **state)
{
    (void)state;
    um_user_t *user = NULL;

    assert_null(user);

    // allocate a user
    user = um_user_new();
    assert_non_null(user);

    // assert all fields are set to NULL
    assert_null(um_user_get_name(user));
    assert_null(um_user_get_password(user));
    assert_null(um_user_get_gecos(user));
    assert_null(um_user_get_home_path(user));
    assert_null(um_user_get_shell_path(user));

    // integer data
    assert_int_equal(um_user_get_uid(user), 0);
    assert_int_equal(um_user_get_gid(user), 0);

    // shadow data
    assert_null(um_user_get_password_hash(user));
    assert_int_equal(um_user_get_last_change(user), 0);
    assert_int_equal(um_user_get_change_min(user), 0);
    assert_int_equal(um_user_get_change_min(user), 0);
    assert_int_equal(um_user_get_change_max(user), 0);
    assert_int_equal(um_user_get_warn_days(user), 0);
    assert_int_equal(um_user_get_inactive_days(user), 0);
    assert_int_equal(um_user_get_expiration(user), 0);
    assert_int_equal(um_user_get_flags(user), 0);
}

static void test_user_new_incorrect(void **state)
{
    (void)state;
}

static void test_user_set_name_correct(void **state)
{
    (void)state;

    int error = 0;
    char *name = "user1";
    um_user_t *user = um_user_new();

    expect_string(__wrap_strdup, s, name);
    will_return(__wrap_strdup, name);

    error = um_user_set_name(user, name);
    assert_int_equal(error, 0);
}

static void test_user_set_name_incorrect(void **state)
{
    (void)state;

    int error = 0;
    char *name = "user1";
    um_user_t *user = um_user_new();

    expect_string(__wrap_strdup, s, name);
    will_return(__wrap_strdup, NULL);

    error = um_user_set_name(user, name);
    assert_int_equal(error, -1);
}

static void test_user_set_password_correct(void **state)
{
    (void)state;

    int error = 0;
    char *password = "";
    um_user_t *user = um_user_new();

    expect_string(__wrap_strdup, s, password);
    will_return(__wrap_strdup, password);

    error = um_user_set_password(user, password);
    assert_int_equal(error, 0);
}

static void test_user_set_password_incorrect(void **state)
{
    (void)state;

    int error = 0;
    char *password = "";
    um_user_t *user = um_user_new();

    expect_string(__wrap_strdup, s, password);
    will_return(__wrap_strdup, NULL);

    error = um_user_set_password(user, password);
    assert_int_equal(error, -1);
}

static void test_user_set_gecos_correct(void **state)
{
    (void)state;

    int error = 0;
    char *gecos = "Some User Info";
    um_user_t *user = um_user_new();

    expect_string(__wrap_strdup, s, gecos);
    will_return(__wrap_strdup, gecos);

    error = um_user_set_gecos(user, gecos);
    assert_int_equal(error, 0);
}

static void test_user_set_gecos_incorrect(void **state)
{
    (void)state;

    int error = 0;
    char *gecos = "Some User Info";
    um_user_t *user = um_user_new();

    expect_string(__wrap_strdup, s, gecos);
    will_return(__wrap_strdup, NULL);

    error = um_user_set_gecos(user, gecos);
    assert_int_equal(error, -1);
}

static void test_user_set_home_path_correct(void **state)
{
    (void)state;
}

static void test_user_set_home_path_incorrect(void **state)
{
    (void)state;
}

static void test_user_set_shell_path_correct(void **state)
{
    (void)state;
}

static void test_user_set_shell_path_incorrect(void **state)
{
    (void)state;
}
