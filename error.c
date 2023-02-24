#include <stdio.h> // vfprintf, stderr
#include <stdarg.h> // va_list, va_start, va_end
#include <stdlib.h> // exit, EXIT_FAILURE

_Noreturn void error_exit(const char fmt, ...) {
    va_list l;
    va_start(l, fmt);
    vfprintf(stderr, fmt, l);
    va_end(l);
    exit(EXIT_FAILURE);
}