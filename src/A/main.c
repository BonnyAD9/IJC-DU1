#include "bitset.h" // bitset_alloc, bitset_size, bitset_getbit, bitset_setbit,
                    // bitset_free, bitset_index_t, bitset_t
#include <stdio.h> // printf

void Eratosthenes(bitset_t pole);

int main() {
    bitset_alloc(set, 3000);

    Eratosthenes(set);

    bitset_index_t pi = 0;
    for (bitset_index_t i = 0; i < bitset_size(set); ++i) {
        if (bitset_getbit(set, i))
            printf("%lu %lu\n", ++pi, i);
    }

    bitset_free(set);
}

// finds primes. 'bitset_get(set, p) == 1' => p is prime
// expects that 'set' is filled with zeros
void Eratosthenes(bitset_t set) {
    bitset_setbit(set, 2, 1); // 2 is prime

    // set all odd numbers as prime candidates
    for (bitset_index_t i = 3; i < bitset_size(set); i += 2) {
        bitset_setbit(set, i, 1);
    }

    // filter primes
    for (bitset_index_t i = 3; i < bitset_size(set); ++i) {
        if (!bitset_getbit(set, i))
            continue;

        // filter out all multiples of the prime 'i'
        // (we can start form 'i * i' because all previous multiples of 'i'
        //  are already filtered by the previos primes)
        for (bitset_index_t j = i * i; j < bitset_size(set); j += i)
            bitset_setbit(set, j, 0);
    }
}