#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

static void test_db(void **state);

int main(void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_db),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}

static void test_db(void **state)
{
    (void)state;
}