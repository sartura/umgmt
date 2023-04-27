#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdio.h>

#include "common.h"
#include "umgmt/dyn_buffer.h"
#include "umgmt/types.h"

// new
static void test_dyn_byte_buffer_new_correct(void **state);

// alloc
static void test_dyn_byte_buffer_alloc_correct(void **state);
static void test_dyn_byte_buffer_alloc_incorrect(void **state);

int main(void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_dyn_byte_buffer_new_correct),
        cmocka_unit_test(test_dyn_byte_buffer_alloc_correct),
        cmocka_unit_test(test_dyn_byte_buffer_alloc_incorrect),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}

static void test_dyn_byte_buffer_new_correct(void **state)
{
    (void)state;

    um_dyn_byte_buffer_t buffer = um_dyn_byte_buffer_new();

    // assert all fields are null
    assert_ptr_equal(buffer.buffer, NULL);
    assert_int_equal(buffer.size, 0);
}

static void test_dyn_byte_buffer_alloc_correct(void **state)
{
    (void)state;

    int error = 0;

    um_dyn_byte_buffer_t buffer = um_dyn_byte_buffer_new();
    const int n = 10;
    const int size = sizeof(byte_t) * n;
    byte_t *expected_ptr = (byte_t *)__real_malloc(size);

    // allocate the buffer
    expect_value(__wrap_malloc, size, size);
    will_return(__wrap_malloc, expected_ptr);

    // call alloc
    error = um_dyn_byte_buffer_alloc(&buffer, n);

    // assert
    assert_int_equal(error, 0);
    assert_int_equal(buffer.size, n);
    assert_ptr_equal(buffer.buffer, expected_ptr);

    // free data
    um_dyn_byte_buffer_free(&buffer);
}

static void test_dyn_byte_buffer_alloc_incorrect(void **state)
{
    (void)state;

    int error = 0;

    um_dyn_byte_buffer_t buffer = um_dyn_byte_buffer_new();
    const int n = 10;
    const int size = sizeof(byte_t) * n;
    byte_t *expected_ptr = NULL;

    // allocate the buffer
    expect_value(__wrap_malloc, size, size);
    will_return(__wrap_malloc, expected_ptr);

    // call alloc
    error = um_dyn_byte_buffer_alloc(&buffer, n);

    // assert
    assert_int_equal(error, -1);
    assert_int_equal(buffer.size, 0);
    assert_ptr_equal(buffer.buffer, NULL);
}
