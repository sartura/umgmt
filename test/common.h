#ifndef UMTM_COMMON_UTEST_H
#define UMTM_COMMON_UTEST_H

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

// malloc
void *__wrap_malloc(size_t size);
extern void *__real_malloc(size_t size);

// strdup
char *__wrap_strdup(const char *s);
extern char *__real_strdup(const char *s);

#endif // UMTM_COMMON_UTEST_H