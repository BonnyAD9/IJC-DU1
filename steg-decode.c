// steg-decode.c
// Řešení IJC-DU1, příklad b), 25.2. 2023
// Autor: Jakub Antonín Štigler, FIT
// Přeloženo: clang 15.0.7
//  C standard: C11
//  Funguje i s gcc 12.2.1 (make CC=gcc)

#include <stdio.h> // FILE
#include <stdlib.h> // EXIT_FAILURE

#include "ppm.h" // ppm_read, struct ppm, ppm_free
#include "error.h" // error_exit
#include "bitset.h" // bitset_t, bitset_alloc, bitset_size, bitset_free

void eratosthenes(bitset_t set); // implemented in primes.c

void decode_file(FILE *out, const char *infile);
void decode(FILE *out, struct ppm *img, bitset_t set);

int main(int argv, char **argc) {
    (void)argv;
    (void)argc;

    decode_file(stdout, "du1-obrazek.ppm");
}

void decode_file(FILE *out, const char *infile) {
    struct ppm *img = ppm_read(infile);
    if (!img)
        error_exit("Cannot read ppm file");

    bitset_alloc(set, img->xsize * img->ysize);

    decode(out, img, set);

    bitset_free(set);
    ppm_free(img);
}

void decode(FILE *out, struct ppm *img, bitset_t set) {
    eratosthenes(set);

    for (bitset_index_t i = 101; i < bitset_size(set); ++i) {
        int byte = 0;
        for (int j = 0; j < 8; ++i) {
            if (bitset_getbit(set, i)) {
                byte |= (img->data[i] & 1) << j;
                ++j;
            }
        }

        if (!byte) {
            fprintf(out, "\n");
            return;
        }

        fprintf(out, "%c", byte);
    }

    // todo utf-8 check
    error_exit("The message is not ended with the NULL character");
}
