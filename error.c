// error.c
// Řešení IJC-DU1, příklad a) a b), 25.2. 2023
// Autor: Jakub Antonín Štigler, FIT
// Přeloženo: clang 15.0.7
//  C standard: C11
//  Funguje i s gcc 12.2.1 (make CC=gcc)

#include "error.h"

#include <stdio.h> // vfprintf, stderr
#include <stdarg.h> // va_list, va_start, va_end
#include <stdlib.h> // exit, EXIT_FAILURE

void warning(const char *fmt, ...) {
    va_list l;
    va_start(l, fmt);

    fprintf(stderr, "Warning: ");
    vfprintf(stderr, fmt, l);
    fprintf(stderr, "\n");

    va_end(l);
}

_Noreturn void error_exit(const char *fmt, ...) {
    va_list l;
    va_start(l, fmt);

    fprintf(stderr, "Error: ");
    vfprintf(stderr, fmt, l);
    fprintf(stderr, "\n");

    va_end(l);
    exit(EXIT_FAILURE);
}
