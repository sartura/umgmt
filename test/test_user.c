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

static void test_user_set_password_hash_correct(void **state);
static void test_user_set_password_hash_incorrect(void **state);

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
        cmocka_unit_test(test_user_set_password_hash_correct),
        cmocka_unit_test(test_user_set_password_hash_incorrect),
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

    // char* user data
    assert_null(um_user_get_name(user));
    assert_null(um_user_get_password(user));
    assert_null(um_user_get_gecos(user));
    assert_null(um_user_get_home_path(user));
    assert_null(um_user_get_shell_path(user));

    // integer user data
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

    int error = 0;
    char *home_path = "/home/user1";
    um_user_t *user = um_user_new();

    expect_string(__wrap_strdup, s, home_path);
    will_return(__wrap_strdup, home_path);

    error = um_user_set_home_path(user, home_path);
    assert_int_equal(error, 0);
}

static void test_user_set_home_path_incorrect(void **state)
{
    (void)state;

    int error = 0;
    char *home_path = "/home/user1";
    um_user_t *user = um_user_new();

    expect_string(__wrap_strdup, s, home_path);
    will_return(__wrap_strdup, NULL);

    error = um_user_set_home_path(user, home_path);
    assert_int_equal(error, -1);
}

static void test_user_set_shell_path_correct(void **state)
{
    (void)state;

    int error = 0;
    char *shell_path = "/usr/bin/bash";
    um_user_t *user = um_user_new();

    expect_string(__wrap_strdup, s, shell_path);
    will_return(__wrap_strdup, shell_path);

    error = um_user_set_shell_path(user, shell_path);
    assert_int_equal(error, 0);
}

static void test_user_set_shell_path_incorrect(void **state)
{
    (void)state;

    int error = 0;
    char *shell_path = "/usr/bin/bash";
    um_user_t *user = um_user_new();

    expect_string(__wrap_strdup, s, shell_path);
    will_return(__wrap_strdup, NULL);

    error = um_user_set_shell_path(user, shell_path);
    assert_int_equal(error, -1);
}

static void test_user_set_password_hash_correct(void **state)
{
    (void)state;

    int error = 0;
    char *password_hash = "0255a4237b05193cfbd43edb3d11c6296f7ea6825315e83d168f7b815674d2027a5bfed3582554db15982545a24b"
                          "09cb7b8323cf595c947e927ba610497f3537";
    um_user_t *user = um_user_new();

    expect_string(__wrap_strdup, s, password_hash);
    will_return(__wrap_strdup, password_hash);

    error = um_user_set_password_hash(user, password_hash);
    assert_int_equal(error, 0);
}

static void test_user_set_password_hash_incorrect(void **state)
{
    (void)state;

    int error = 0;
    char *password_hash = "0255a4237b05193cfbd43edb3d11c6296f7ea6825315e83d168f7b815674d2027a5bfed3582554db15982545a24b"
                          "09cb7b8323cf595c947e927ba610497f3537";
    um_user_t *user = um_user_new();

    expect_string(__wrap_strdup, s, password_hash);
    will_return(__wrap_strdup, NULL);

    error = um_user_set_password_hash(user, password_hash);
    assert_int_equal(error, -1);
}
