CFLAGS:=-std=c11 -pedantic -Wall -Wextra -O2
LDFLAGS:=-lm

.PRECIOUS: primes-i.o
.PHONY: all clean run

all: primes primes-i steg-decode

D:=$(shell $(CC) -MM *.c > dep.d)
-include dep.d

run: primes primes-i
	ulimit -s 30000 ; ./primes ; ./primes-i

primes: error.o eratosthenes.o
primes-i: error.o eratosthenes-i.o
steg-decode: eratosthenes.o error.o ppm.o

clean:
	-rm *.o primes primes-i steg-decode

%-i.o: %.c eratosthenes.h bitset.h error.h
	$(CC) $(CFLAGS) -c -DUSE_INLINE -o $@ $<
