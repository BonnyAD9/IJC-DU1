// eratosthenes.c
// Řešení IJC-DU1, příklad a), 16.3. 2023
// Autor: Jakub Antonín Štigler, FIT
// Přeloženo: clang 15.0.7
//  C standard: C11
//  Funguje i s gcc 12.2.1 (make CC=gcc)

#include "eratosthenes.h" // bitset_t, bitset_index_t, bitset_setbit,
                          // bitset_size, bitset_getbit

#include <math.h> // sqrt

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
    for (bitset_index_t i = 3; i < lim; i += 2) {
        if (!bitset_getbit(set, i))
            continue;

        // filter out all multiples of the prime 'i'
        // (we can start form 'i * i' because all previous multiples of 'i'
        //  are already filtered by the previos primes)
        for (bitset_index_t j = i * i; j < bitset_size(set); j += i + i)
            bitset_setbit(set, j, 0);
    }
}
