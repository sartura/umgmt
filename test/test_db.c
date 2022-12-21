#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include <umgmt.h>

#include "common.h"
#include "umgmt/db.c"
#include "umgmt/db.h"

#define UM_DB_T_SIZE sizeof(um_db_t)

static void test_db_new_correct(void **state);
static void test_db_new_incorrect(void **state);

int main(void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_db_new_correct),
        cmocka_unit_test(test_db_new_incorrect),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}

static void test_db_new_correct(void **state)
{
    (void)state;

    um_db_t *db = NULL;

    assert_null(db);

    expect_value(__wrap_malloc, size, UM_DB_T_SIZE);
    will_return(__wrap_malloc, __real_malloc(UM_DB_T_SIZE));

    db = um_db_new();
    assert_non_null(db);

    // assert fields are set to NULL
    assert_null(um_db_get_user_list_head(db));
    assert_null(um_db_get_group_list_head(db));

    um_db_free(db);
}

static void test_db_new_incorrect(void **state)
{
    (void)state;

    um_db_t *db = NULL;

    assert_null(db);

    expect_value(__wrap_malloc, size, UM_DB_T_SIZE);
    will_return(__wrap_malloc, NULL);

    db = um_db_new();
    assert_null(db);
}