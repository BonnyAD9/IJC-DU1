#define BITSET_NO_MACRO
#include "bitset.h"

#include "../B/error.h" // error_exit

bitset_t bitset_alloc(bitset_index_t len) {
    bitset_index_t size = (len + BITS_PER_UL - 1) / BITS_PER_UL;
    bitset_t set = calloc(size, sizeof(bitset_index_t));
    if (!set)
        fprintf(stderr, u8"bitset_alloc: Chyba alokace paměti");
    *set = len;
    return set;
}

void bitset_free(bitset_t set) {
    free(set);
}

bitset_index_t bitset_size(bitset_t set) {
    return *set;
}

void bitset_setbit(bitset_t set, bitset_index_t index, _Bool bit) {
    if (index >= bitset_size(set)) {
        error_exit(
            "bitset_setbit: Index %lu mimo rozsah 0..%lu",
            index, bitset_size(set)
        );
    }

    bitset_index_t mask = 1UL << index % BITS_PER_UL;
    if (bit)
        set[index / BITS_PER_UL + 1] |= mask;
    else
        set[index / BITS_PER_UL + 1] &= ~mask;
}

_Bool bitset_getbit(bitset_t set, bitset_index_t index) {
    if (index >= bitset_size(set)) {
        error_exit(
            "bitset_getbit: Index %lu mimo rozsah 0..%lu",
            index, bitset_size(set)
        );
    }
    return set[index / BITS_PER_UL + 1] >> index % BITS_PER_UL & 1;
}
