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

#define START_PRIME 101

char *decode_file(const char *infile);
void decode(char *out, struct ppm *img, bitset_t set);
unsigned char *utf8_check(unsigned char *s);

int main(int argv, char **argc) {
    (void)argv;
    (void)argc;

    char *msg = decode_file("du1-obrazek.ppm");

    printf("%s\n", msg);

    free(msg);
}

char *decode_file(const char *infile) {
    struct ppm *img = ppm_read(infile);
    if (!img)
        error_exit("Cannot read ppm file");

    // calculate the max message size: (img size) / (8 * 3) + 1
    // 8 because 1 byte in message is from 8 bytes in image
    // 3 because less than 1/3 of numbers are primes
    // + 1 to avoid rounding error
    size_t msg_size = (img->xsize * img->ysize) / 24 + 1;
    char *msg = malloc(msg_size);
    if (!msg)
        error_exit(
            "Failed to allocate space for the message of size %zu",
            msg_size
        );

    bitset_alloc(set, img->xsize * img->ysize);

    decode(msg, img, set);

    bitset_free(set);
    ppm_free(img);

    if (utf8_check((unsigned char *)msg)) {
        free(msg);
        error_exit("The message is not valid utf-8:\n%s", msg);
    }

    return msg;
}

// expects that 'set' has all bits set to 0
void decode(char *out, struct ppm *img, bitset_t set) {
    eratosthenes(set);

    for (bitset_index_t i = START_PRIME; i < bitset_size(set); ++i) {
        int byte = 0;
        for (int j = 0; j < 8; ++i) {
            if (bitset_getbit(set, i)) {
                byte |= (img->data[i] & 1) << j;
                ++j;
            }
        }
        *out++ = byte;

        if (!byte)
            return;

    }

    error_exit("The message is not ended with the NULL character");
}

// function from https://www.cl.cam.ac.uk/~mgk25/ucs/utf8_check.c
// formatted to fit the code style of this project

/*
 * The utf8_check() function scans the '\0'-terminated string starting
 * at s. It returns a pointer to the first byte of the first malformed
 * or overlong UTF-8 sequence found, or NULL if the string contains
 * only correct UTF-8. It also spots UTF-8 sequences that could cause
 * trouble if converted to UTF-16, namely surrogate characters
 * (U+D800..U+DFFF) and non-Unicode positions (U+FFFE..U+FFFF). This
 * routine is very likely to find a malformed sequence if the input
 * uses any other encoding than UTF-8. It therefore can be used as a
 * very effective heuristic for distinguishing between UTF-8 and other
 * encodings.
 *
 * I wrote this code mainly as a specification of functionality; there
 * are no doubt performance optimizations possible for certain CPUs.
 *
 * Markus Kuhn <http://www.cl.cam.ac.uk/~mgk25/> -- 2005-03-30
 * License: http://www.cl.cam.ac.uk/~mgk25/short-license.html
 */

unsigned char *utf8_check(unsigned char *s) {
    while (*s) {
        if (*s < 0x80)
            /* 0xxxxxxx */
            s++;
        else if ((s[0] & 0xe0) == 0xc0) {
            /* 110XXXXx 10xxxxxx */
            if ((s[1] & 0xc0) != 0x80 ||
                (s[0] & 0xfe) == 0xc0)                        /* overlong? */
                return s;
            else
                s += 2;
        } else if ((s[0] & 0xf0) == 0xe0) {
            /* 1110XXXX 10Xxxxxx 10xxxxxx */
            if ((s[1] & 0xc0) != 0x80 ||
                (s[2] & 0xc0) != 0x80 ||
                (s[0] == 0xe0 && (s[1] & 0xe0) == 0x80) ||    /* overlong? */
                (s[0] == 0xed && (s[1] & 0xe0) == 0xa0) ||    /* surrogate? */
                (s[0] == 0xef && s[1] == 0xbf &&
                 (s[2] & 0xfe) == 0xbe))                /* U+FFFE or U+FFFF? */
                return s;
            else
                s += 3;
        } else if ((s[0] & 0xf8) == 0xf0) {
            /* 11110XXX 10XXxxxx 10xxxxxx 10xxxxxx */
            if ((s[1] & 0xc0) != 0x80 ||
                (s[2] & 0xc0) != 0x80 ||
                (s[3] & 0xc0) != 0x80 ||
                (s[0] == 0xf0 && (s[1] & 0xf0) == 0x80) ||    /* overlong? */
                (s[0] == 0xf4 && s[1] > 0x8f) || s[0] > 0xf4) /* > U+10FFFF? */
                return s;
            else
                s += 4;
        } else
            return s;
    }

    return NULL;
}
