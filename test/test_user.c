#include "umgmt/types.h"
#include "umgmt/user.h"
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include <umgmt.h>

static void test_user_new_correct(void **state);
static void test_user_new_incorrect(void **state);

int main(void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_user_new_correct),
        cmocka_unit_test(test_user_new_incorrect),
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
