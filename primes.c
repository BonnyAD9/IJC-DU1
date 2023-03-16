// primes.c
// Řešení IJC-DU1, příklad a), 16.3. 2023
// Autor: Jakub Antonín Štigler, FIT
// Přeloženo: clang 15.0.7
//  C standard: C11
//  Funguje i s gcc 12.2.1 (make CC=gcc)

#include <stdio.h> // printf, fprintf, stderr
#include <time.h> // clock_t, clock, CLOCKS_PER_SEC

#include "bitset.h" // bitset_create, bitset_size, bitset_getbit,
                    // bitset_index_t, bitset_t
#include "eratosthenes.h" // eratosthenes

void call_eratosthenes(void);

int main(void) {
    clock_t start = clock();

    call_eratosthenes();

    fprintf(stderr, "Time=%.3g\n", (double)(clock() - start) / CLOCKS_PER_SEC);
}

void call_eratosthenes(void) {
    bitset_create(set, 230000000); // 230 mil is surely not prime

    eratosthenes(set);

    // get the index of the 10th prime from the end
    bitset_index_t pi = bitset_size(set) - 1;
    for (int n = 0; n < 10; --pi)
        n += (bitset_getbit(set, pi));

    // print the last 10 primes
    for (; pi < bitset_size(set); ++pi) {
        if (bitset_getbit(set, pi))
            printf("%lu\n", pi);
    }

    //bitset_free(set);
}
