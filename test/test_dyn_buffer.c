#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include <umgmt.h>
#include "common.h"
#include "umgmt/dyn_buffer.h"
#include "umgmt/types.h"

static void test_dyn_byte_buffer_new_correct(void **state);

int main(void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_dyn_byte_buffer_new_correct),
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
