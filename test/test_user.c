#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

static void test_user(void **state);

int main(void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_user),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}

static void test_user(void **state)
{
    (void)state;
}