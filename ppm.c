// ppm.c
// Řešení IJC-DU1, příklad b), 25.2. 2023
// Autor: Jakub Antonín Štigler, FIT
// Přeloženo: clang 15.0.7
//  C standard: C11
//  Funguje i s gcc 12.2.1 (make CC=gcc)

#include "ppm.h"

#include <stdio.h> // FILE, fscnaf
#include <ctype.h> // isspace

#include "error.h" // warning

_Bool _ppm_read_header(FILE *in, unsigned *w, unsigned *h) {
    char c;
    if (fscanf(in, "P6 %u %u 255%c", w, h, c) != 3) {
        warning("Invalid/unsupported file header");
        return 0;
    }

    if (!isspace(c)) {
        warning("Expected whitespace after header, found '%c'", c);
        return 0;
    }

    return 1;
}

struct ppm *ppm_read(const char *filename) {
    FILE *in = fopen(filename, "rb");
    if (!in) {
        warning("Failed to open file '%s'", filename);
        return NULL;
    }

    unsigned w, h;
    if (!_ppm_read_header(in, &w, &h))
        return NULL;

    if (w > PPM_MAX_WH || h > PPM_MAX_WH) {
        warning("Image is too large");
        return NULL;
    }

    size_t pcount = w * h;

    struct ppm *p = malloc(sizeof(unsigned) * 2 + 3 * pcount);
    p->xsize = w;
    p->ysize = h;

    if (fread(p->data, 3, pcount, in) != pcount) {
        fclose(in);
        ppm_free(p);
        warning("File is too short");
        return NULL;
    }

    fclose(in);
    return p;
}

void ppm_free(struct ppm *p) {
    free(p);
}
