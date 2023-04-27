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
}

static void test_shadow_password_set_algorithm_incorrect(void **state)
{
    (void)state;
}

static void test_shadow_password_set_salt_correct(void **state)
{
    (void)state;
}

static void test_shadow_password_set_salt_incorrect(void **state)
{
    (void)state;
}

static void test_shadow_password_set_hash_correct(void **state)
{
    (void)state;
}

static void test_shadow_password_set_hash_incorrect(void **state)
{
    (void)state;
}
