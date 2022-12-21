#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

static void test_group(void **state);

int main(void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_group),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}

static void test_group(void **state)
{
    (void)state;
}