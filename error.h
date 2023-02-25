// error.h
// Řešení IJC-DU1, příklad a) a b), 25.2. 2023
// Autor: Jakub Antonín Štigler, FIT
// Přeloženo: clang 15.0.7
//  C standard: C11
//  Funguje i s gcc 12.2.1 (make CC=gcc)

#ifndef ERROR_INCLUDED
#define ERROR_INCLUDED

void warning(const char *fmt, ...);

_Noreturn void error_exit(const char *fmt, ...);

#endif // #ifndef ERROR_INCLUDED
