#include "common.h"

void *__wrap_malloc(size_t size)
{
    check_expected(size);
    return mock_ptr_type(void *);
}

char *__wrap_strdup(const char *s)
{
    check_expected(s);
    return mock_ptr_type(char *);
}