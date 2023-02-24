CC=clang
CFLAGS=-std=c11 -pedantic -Wall -Wextra -O2
OUT=main

all: primes-m primes-i-m

debug:
	make run "CFLAGS=$(CFLAGS) -g -fsanitize=address"

run: all
	ulimit -s 30000 ; ./primes ; ./primes-i

primes: primes-m
primes-i: primes-i-m

primes-m:
	make primes-g OUT=primes

primes-i-m:
	make primes-g OUT=primes-i "CFLAGS=$(CFLAGS) -DUSE_INLINE"

primes-g: primes.o error.o
	$(CC) $(CFLAGS) -o $(OUT) primes.o error.o

primes.c: bitset.h
bitset.h: error.c
error.c: error.h

primes.o: primes.c
	$(CC) $(CFLAGS) -c -o primes.o primes.c

error.o: error.c
	$(CC) $(CFLAGS) -c -o error.o error.c

clean:
	rm primes.o error.o primes primes-i