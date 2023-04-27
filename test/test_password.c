#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdio.h>
#include <string.h>

#include "common.h"
#include "umgmt/password.h"
#include "umgmt/types.h"

// new
static void test_shadow_password_new_correct(void **state);

// test set algorithm
static void test_shadow_password_set_algorithm_correct(void **state);
static void test_shadow_password_set_algorithm_incorrect(void **state);

// test set salt
static void test_shadow_password_set_salt_correct(void **state);
static void test_shadow_password_set_salt_incorrect(void **state);

// test set hash
static void test_shadow_password_set_hash_correct(void **state);
static void test_shadow_password_set_hash_incorrect(void **state);

// wrappers
int __wrap_um_dyn_byte_buffer_copy(um_dyn_byte_buffer_t *src, um_dyn_byte_buffer_t *dst);

int main(void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_shadow_password_new_correct),
        cmocka_unit_test(test_shadow_password_set_algorithm_correct),
        cmocka_unit_test(test_shadow_password_set_algorithm_incorrect),
        cmocka_unit_test(test_shadow_password_set_salt_correct),
        cmocka_unit_test(test_shadow_password_set_salt_incorrect),
        cmocka_unit_test(test_shadow_password_set_hash_correct),
        cmocka_unit_test(test_shadow_password_set_hash_incorrect),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}

static void test_shadow_password_new_correct(void **state)
{
    (void)state;

    um_shadow_password_t password = um_shadow_password_new();

    // buffers already tested in test_dyn_buffer.c
    // test algorithm field
    assert_ptr_equal(password.algorithm, NULL);
}

static void test_shadow_password_set_algorithm_correct(void **state)
{
    (void)state;

    const char *alg_id = "sha512";
    int error = 0;

    um_shadow_password_t shp = um_shadow_password_new();

    // setup strdup wrapper
    expect_string(__wrap_strdup, s, alg_id);
    will_return(__wrap_strdup, alg_id);

    error = um_shadow_password_set_algorithm_id(&shp, alg_id);

    assert_int_equal(error, 0);
    assert_string_equal(shp.algorithm, alg_id);
}

static void test_shadow_password_set_algorithm_incorrect(void **state)
{
    (void)state;

    const char *alg_id = "sha256";
    int error = 0;

    um_shadow_password_t shp = um_shadow_password_new();

    // setup strdup wrapper
    expect_string(__wrap_strdup, s, alg_id);
    will_return(__wrap_strdup, NULL);

    error = um_shadow_password_set_algorithm_id(&shp, alg_id);

    assert_int_equal(error, -1);
    assert_ptr_equal(shp.algorithm, NULL);
}

static void test_shadow_password_set_salt_correct(void **state)
{
    (void)state;

    const char *salt = "abcdefgh";
    int error = 0;

    um_shadow_password_t shp = um_shadow_password_new();

    // setup buffer copy wrapper
    expect_value(__wrap_um_dyn_byte_buffer_copy, dst, &shp.salt);
    will_return(__wrap_um_dyn_byte_buffer_copy, 0);

    // call function
    error = um_shadow_password_set_salt(&shp, (byte_t *)salt, strlen(salt));

    // assert valid functionality
    assert_int_equal(error, 0);
}

static void test_shadow_password_set_salt_incorrect(void **state)
{
    (void)state;

    const char *salt = "abcdefgh";
    int error = 0;

    um_shadow_password_t shp = um_shadow_password_new();

    // setup buffer copy wrapper
    expect_value(__wrap_um_dyn_byte_buffer_copy, dst, &shp.salt);
    will_return(__wrap_um_dyn_byte_buffer_copy, -1);

    // call function
    error = um_shadow_password_set_salt(&shp, (byte_t *)salt, strlen(salt));

    // assert valid functionality
    assert_int_equal(error, -1);
}

static void test_shadow_password_set_hash_correct(void **state)
{
    (void)state;
}

static void test_shadow_password_set_hash_incorrect(void **state)
{
    (void)state;
}

int __wrap_um_dyn_byte_buffer_copy(um_dyn_byte_buffer_t *src, um_dyn_byte_buffer_t *dst)
{
    // can't check source - temp variable in setter functions
    // check only destination pointer
    check_expected(dst);

    return mock_type(int);
}