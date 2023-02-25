// primes.c
// Řešení IJC-DU1, příklad a), 25.2. 2023
// Autor: Jakub Antonín Štigler, FIT
// Přeloženo: clang 15.0.7
//  C standard: C11
//  Funguje i s gcc 12.2.1 (make CC=gcc)

#include <stdio.h> // printf, fprintf, stderr
#include <time.h> // clock_t, clock, CLOCKS_PER_SEC
#include <math.h> // sqrt

#include "bitset.h" // bitset_create, bitset_size, bitset_getbit, bitset_setbit,
                    // bitset_index_t, bitset_t

void call_eratosthnes(void);
void eratosthenes(bitset_t pole);

int main(void) {
    clock_t start = clock();

    call_eratosthnes();

    fprintf(stderr, "Time=%.3g\n", (double)(clock() - start) / CLOCKS_PER_SEC);
}

void call_eratosthnes(void) {
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

// finds primes. 'bitset_get(set, p) == 1' => p is prime
// expects that 'set' is filled with zeros
void eratosthenes(bitset_t set) {
    bitset_setbit(set, 2, 1); // 2 is prime

    // set all odd numbers as prime candidates
    for (bitset_index_t i = 3; i < bitset_size(set); i += 2) {
        bitset_setbit(set, i, 1);
    }

    bitset_index_t lim = sqrt(bitset_size(set));

    // filter primes
    for (bitset_index_t i = 3; i < lim; ++i) {
        if (!bitset_getbit(set, i))
            continue;

        // filter out all multiples of the prime 'i'
        // (we can start form 'i * i' because all previous multiples of 'i'
        //  are already filtered by the previos primes)
        for (bitset_index_t j = i * i; j < bitset_size(set); j += i)
            bitset_setbit(set, j, 0);
    }
}
