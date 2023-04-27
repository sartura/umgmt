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
static void test_shadow_password_new_correct(void **state);

int main(void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_shadow_password_new_correct),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}

static void test_shadow_password_new_correct(void **state)
{
    (void)state;
}