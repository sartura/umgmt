#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdio.h>
#include <string.h>

#include "common.h"
#include "umgmt/dyn_buffer.h"
#include "umgmt/types.h"

// new
static void test_dyn_byte_buffer_new_correct(void **state);

// alloc
static void test_dyn_byte_buffer_alloc_correct(void **state);
static void test_dyn_byte_buffer_alloc_incorrect(void **state);

// copy
static void test_dyn_byte_buffer_copy_correct(void **state);
static void test_dyn_byte_buffer_copy_incorrect(void **state);

int main(void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_dyn_byte_buffer_new_correct),     cmocka_unit_test(test_dyn_byte_buffer_alloc_correct),
        cmocka_unit_test(test_dyn_byte_buffer_alloc_incorrect), cmocka_unit_test(test_dyn_byte_buffer_copy_correct),
        cmocka_unit_test(test_dyn_byte_buffer_copy_incorrect),
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

static void test_dyn_byte_buffer_copy_correct(void **state)
{
    (void)state;

    int error = 0;

    const char *buffer_data = "Hello World!";
    const size_t buffer_size = strlen(buffer_data);

    um_dyn_byte_buffer_t src = (um_dyn_byte_buffer_t){
        .buffer = (byte_t *)buffer_data,
        .size = buffer_size,
    };
    um_dyn_byte_buffer_t dst = um_dyn_byte_buffer_new();

    const size_t expected_size = buffer_size;
    const byte_t *expected_ptr = (byte_t *)__real_malloc(buffer_size);

    // setup malloc to use real_malloc
    expect_value(__wrap_malloc, size, expected_size);
    will_return(__wrap_malloc, expected_ptr);

    // call copy
    error = um_dyn_byte_buffer_copy(&src, &dst);

    // assert
    assert_int_equal(error, 0);
    assert_int_equal(dst.size, buffer_size);
    assert_memory_equal(dst.buffer, buffer_data, buffer_size);

    // free data
    um_dyn_byte_buffer_free(&dst);
}

static void test_dyn_byte_buffer_copy_incorrect(void **state)
{
    (void)state;

    int error = 0;

    const char *buffer_data = "Hello World!";
    const size_t buffer_size = strlen(buffer_data);

    um_dyn_byte_buffer_t src = (um_dyn_byte_buffer_t){
        .buffer = (byte_t *)buffer_data,
        .size = buffer_size,
    };
    um_dyn_byte_buffer_t dst = um_dyn_byte_buffer_new();

    const size_t expected_size = buffer_size;
    const byte_t *expected_ptr = NULL;

    // setup malloc to use real_malloc
    expect_value(__wrap_malloc, size, expected_size);
    will_return(__wrap_malloc, expected_ptr);

    // call copy
    error = um_dyn_byte_buffer_copy(&src, &dst);

    // assert
    assert_int_equal(error, -1);
    assert_int_equal(dst.size, 0);
    assert_ptr_equal(dst.buffer, NULL);
}
