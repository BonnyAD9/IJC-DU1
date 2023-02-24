// bitset.h
// Řešení IJC-DU1, příklad a), 24.2. 2023
// Autor: Jakub Antonín Štigler, FIT
// Přeloženo: clang 15.0.7
//  C standard: C11
//  Funguje i s gcc 12.2.1 (make CC=gcc)

#ifndef BITSET_INCLUDED
#define BITSET_INCLUDED

#include <limits.h> // CHAR_BIT, ULONG_MAX
#include <stdlib.h> // calloc, free
#include <assert.h> // assert

#include "error.h" // error_exit


// general definitions

typedef unsigned long bitset_index_t;
typedef bitset_index_t *bitset_t;

// number of bits in one item of bitset
#define BITS_PER_UL (sizeof(bitset_index_t) * CHAR_BIT)


// macros used by the functions, inline functions and macros.
// basically the bitset implementation only in macros to avoid code repetition

// all arguments are evaluated only once
#define bitset_free__(set) free(set)

// all arguments are evaluated only once
#define bitset_size__(set) (*(set))

// all arguments are evaluated only once
#define bitset_setbit__(set, index, value) do { \
    bitset_t set__ = (set); \
    bitset_index_t index__ = (index); \
    if (index__ > bitset_size(set__)) \
        error_exit( \
            "bitset_setbit: Index %lu mimo rozsah 0..%lu", \
            index__, \
            bitset_size(set__) \
        ); \
    if (value) \
        set__[index__ / BITS_PER_UL + 1] |= 1UL << index__ % BITS_PER_UL; \
    else \
        set__[index__ / BITS_PER_UL + 1] &= ~(1UL << index__ % BITS_PER_UL); \
} while (0)

// all arguments are evaluated only once
#define bitset_getbit__(set, index, result) do { \
    bitset_t set__ = (set); \
    bitset_index_t index__ = (index); \
    if (index__ >= bitset_size(set__)) \
        error_exit( \
            "bitset_getbit: Index %lu mimo rozsah 0..%lu", \
            index__, \
            bitset_size(set__) \
        ); \
    (result) = set__[index__ / BITS_PER_UL + 1] >> index__ % BITS_PER_UL & 1; \
} while (0)


// interface inline functions (use the macros as implementation)

// bitset_create and bitset_alloc cannot be created with a function

static inline void bitset_free(bitset_t set) {
    bitset_free__(set);
}

static inline bitset_index_t bitset_size(bitset_t set) {
    return bitset_size__(set);
}

static inline void bitset_setbit(bitset_t set, bitset_index_t index, _Bool bit) {
    bitset_setbit__(set, index, bit);
}

static inline _Bool bitset_getbit(bitset_t set, bitset_index_t index) {
    _Bool res;
    bitset_getbit__(set, index, res);
    return res;
}


// interface macros

// this macro creates variable and thus cannot be used as a single statement
// for example in if
#define bitset_create(jmeno_pole, velikost) bitset_index_t jmeno_pole[ \
        ((velikost) + BITS_PER_UL - 1) / BITS_PER_UL + 1 \
    ] = { velikost }; _Static_assert( \
        (velikost) > 0 && (velikost) < ULONG_MAX, \
        "length must be positive"\
    )

// this macro creates variable and thus cannot be used as a single statement
// for example in if
#define bitset_alloc(set_name, len) bitset_t set_name; do { \
    bitset_index_t len__ =  len; \
    assert(len__ < ULONG_MAX); \
    set_name = calloc( \
        (len__ + BITS_PER_UL - 1) / BITS_PER_UL + 1, \
        sizeof(bitset_index_t) \
    ); \
    if (!set_name) \
        error_exit(u8"bitset_alloc: Chyba alokace paměti"); \
    *set_name = len__;\
} while (0)

#ifndef USE_INLINE

#define bitset_free(jmeno_pole) bitset_free__(jmeno_pole)

#define bitset_size(jmeno_pole) bitset_size__(jmeno_pole)

#define bitset_setbit(jmeno_pole, index, vyraz) \
    bitset_setbit__(jmeno_pole, index, vyraz)

// calls the inline function to ensure that each argument is evaluated
// only once
#define bitset_getbit(jmeno_pole, index) \
    bitset_getbit(jmeno_pole, index)

#endif // #ifndef USE_INLINE

#endif // #ifndef BITSET_INCLUDED
