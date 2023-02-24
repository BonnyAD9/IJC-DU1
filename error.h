// error.h
// Řešení IJC-DU1, příklad a) a b), 24.2. 2023
// Autor: Jakub Antonín Štigler, FIT
// Přeloženo: clang 15.0.7
//  C standard: C11
//  Funguje i s gcc 12.2.1 (make CC=gcc)

_Noreturn void error_exit(const char *fmt, ...);
