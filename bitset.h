#include <limits.h> // CHAR_BIT
#include <stdio.h> // vfprintf, stderr
#include <stdlib.h> // exit, calloc

typedef unsigned long bitset_index_t;
typedef bitset_index_t *bitset_t;

#define BITS_PER_UL (sizeof(unsigned long) * CHAR_BIT)

bitset_t bitset_alloc(bitset_index_t len);
void bitset_free(bitset_t set);
bitset_index_t bitset_size(bitset_t set);
void bitset_setbit(bitset_t set, bitset_index_t index, _Bool bit);
_Bool bitset_getbit(bitset_t set, bitset_index_t index);

// inline functions
#ifdef USE_INLINE

inline bitset_t bitset_alloc(bitset_index_t len) {
    bitset_index_t size = (len + BITS_PER_UL - 1) / BITS_PER_UL;
    bitset_t set = calloc(size, sizeof(bitset_index_t));
    if (!set)
        fprintf(stderr, u8"bitset_alloc: Chyba alokace paměti");
    *set = len;
    return set;
}

inline void bitset_free(bitset_t set) {
    free(set);
}

inline bitset_index_t bitset_size(bitset_t set) {
    return *set;
}

inline void bitset_setbit(bitset_t set, bitset_index_t index, _Bool bit) {
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

inline _Bool bitset_getbit(bitset_t set, bitset_index_t index) {
    if (index >= bitset_size(set)) {
        error_exit(
            "bitset_getbit: Index %lu mimo rozsah 0..%lu",
            index, bitset_size(set)
        );
    }
    return set[index / BITS_PER_UL + 1] >> index % BITS_PER_UL & 1;
}

#endif USE_INLINE

// interface macros
#ifndef BITSET_NO_MACRO
#ifndef USE_INLINE

#define bitset_create(jmeno_pole, velikost) unsigned long jmeno_pole[ \
    ((velikost) + BITS_PER_UL - 1) / BITS_PER_UL \
] = { 0 }; _Static_assert((velikost) > 0, "velikost must be positive")

#define bitset_alloc(jmeno_pole, velikost) unsigned long *jmeno_pole = \
    bitset_alloc((assert(velikost < ULONG_MAX), velikost))

#define bitset_free(jmeno_pole) bitset_free(jmeno_pole)

#define bitset_size(jmeno_pole) bitset_size(jmeno_pole)

#define bitset_setbit(jmeno_pole, index, vyraz) \
    bitset_setbit(jmeno_pole, index, vyraz)

#define bitset_getbit(jmeno_pole, index) bitset_getbit(jmeno_pole, index)

#endif // USE_INLINE
#endif // BITSET_NO_MACRO
