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

// test converters
static void test_shadow_password_algorithm_to_id(void **state);
static void test_shadow_password_algorithm_id2str(void **state);

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
        cmocka_unit_test(test_shadow_password_algorithm_to_id),
        cmocka_unit_test(test_shadow_password_algorithm_id2str),
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

    const char *hash = "b94d27b9934d3e08a52e52d7da7dabfac484efe37a5380ee9088f7ace2efcde9";
    int error = 0;

    um_shadow_password_t shp = um_shadow_password_new();

    // setup buffer copy wrapper
    expect_value(__wrap_um_dyn_byte_buffer_copy, dst, &shp.hash);
    will_return(__wrap_um_dyn_byte_buffer_copy, 0);

    // call function
    error = um_shadow_password_set_hash(&shp, (byte_t *)hash, strlen(hash));

    // assert valid functionality
    assert_int_equal(error, 0);
}

static void test_shadow_password_set_hash_incorrect(void **state)
{
    (void)state;

    const char *hash = "b94d27b9934d3e08a52e52d7da7dabfac484efe37a5380ee9088f7ace2efcde9";
    int error = 0;

    um_shadow_password_t shp = um_shadow_password_new();

    // setup buffer copy wrapper
    expect_value(__wrap_um_dyn_byte_buffer_copy, dst, &shp.hash);
    will_return(__wrap_um_dyn_byte_buffer_copy, -1);

    // call function
    error = um_shadow_password_set_hash(&shp, (byte_t *)hash, strlen(hash));

    // assert valid functionality
    assert_int_equal(error, -1);
}

static void test_shadow_password_algorithm_to_id(void **state)
{
    (void)state;

    char *alg_name = "";
    um_hash_algorithm_t alg_id = um_hash_algorithm_unknown;

    alg_name = "md5";
    alg_id = um_shadow_password_algorithm_to_id(alg_name);
    assert_int_equal(alg_id, um_hash_algorithm_md5);

    alg_name = "blowfish";
    alg_id = um_shadow_password_algorithm_to_id(alg_name);
    assert_int_equal(alg_id, um_hash_algorithm_blowfish);

    alg_name = "bcrypt";
    alg_id = um_shadow_password_algorithm_to_id(alg_name);
    assert_int_equal(alg_id, um_hash_algorithm_bcrypt);

    alg_name = "sha256";
    alg_id = um_shadow_password_algorithm_to_id(alg_name);
    assert_int_equal(alg_id, um_hash_algorithm_sha256);

    alg_name = "sha512";
    alg_id = um_shadow_password_algorithm_to_id(alg_name);
    assert_int_equal(alg_id, um_hash_algorithm_sha512);

    alg_name = "random";
    alg_id = um_shadow_password_algorithm_to_id(alg_name);
    assert_int_equal(alg_id, um_hash_algorithm_unknown);
}

static void test_shadow_password_algorithm_id2str(void **state)
{
    (void)state;

    um_hash_algorithm_t alg_id = um_hash_algorithm_unknown;
    const char *alg_name = NULL;

    alg_id = um_hash_algorithm_md5;
    alg_name = um_shadow_password_algorithm_id2str(alg_id);
    assert_string_equal(alg_name, "1");

    alg_id = um_hash_algorithm_blowfish;
    alg_name = um_shadow_password_algorithm_id2str(alg_id);
    assert_string_equal(alg_name, "2a");

    alg_id = um_hash_algorithm_bcrypt;
    alg_name = um_shadow_password_algorithm_id2str(alg_id);
    assert_string_equal(alg_name, "2b");

    alg_id = um_hash_algorithm_sha256;
    alg_name = um_shadow_password_algorithm_id2str(alg_id);
    assert_string_equal(alg_name, "5");

    alg_id = um_hash_algorithm_sha512;
    alg_name = um_shadow_password_algorithm_id2str(alg_id);
    assert_string_equal(alg_name, "6");

    alg_id = um_hash_algorithm_unknown;
    alg_name = um_shadow_password_algorithm_id2str(alg_id);
    assert_ptr_equal(alg_name, NULL);
}

int __wrap_um_dyn_byte_buffer_copy(um_dyn_byte_buffer_t *src, um_dyn_byte_buffer_t *dst)
{
    // can't check source - temp variable in setter functions
    // check only destination pointer
    check_expected(dst);

    return mock_type(int);
}