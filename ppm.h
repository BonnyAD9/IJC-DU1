// ppm.h
// Řešení IJC-DU1, příklad b), 25.2. 2023
// Autor: Jakub Antonín Štigler, FIT
// Přeloženo: clang 15.0.7
//  C standard: C11
//  Funguje i s gcc 12.2.1 (make CC=gcc)

#ifndef PPM_INCLUDED
#define PPM_INCLUDED

#include <limits.h>

#define PPM_MAX_WH 16000

struct ppm {
    unsigned xsize;
    unsigned ysize;
    char data[];
};

struct ppm *ppm_read(const char *filename);

void ppm_free(struct ppm *p);

#endif // #ifndef PPM_INCLUDED
