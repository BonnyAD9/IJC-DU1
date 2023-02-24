#define BITSET_NO_PUBLIC // avoid name collisions
#include "bitset.h"

// also define the non inline functions for builds with no optimizations (-O0)

void bitset_free(bitset_t set) {
    bitset_free__(set);
}

bitset_index_t bitset_size(bitset_t set) {
    return bitset_size__(set);
}

void bitset_setbit(bitset_t set, bitset_index_t index, _Bool bit) {
    bitset_setbit__(set, index, bit);
}

_Bool bitset_getbit(bitset_t set, bitset_index_t index) {
    _Bool res;
    bitset_getbit__(set, index, res);
    return res;
}