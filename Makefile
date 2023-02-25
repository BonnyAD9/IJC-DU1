# Makefile
# Řešení IJC-DU1, příklad a) a b), 25.2. 2023
# Autor: Jakub Antonín Štigler, FIT
# možnosti překladu:
#   make          # zkompiluje vše
#   make all      # zkompiluje vše
#   make debug    # zkompiluje vše s debugovacímy symboly a ASANem
#   make run      # zkompiluje vše a spustí se zvášenou velikostí stacku
#   make primes   # zkompiluje úlohu A s makry
#   make primes-i # zkompiluje úlohu A bez maker
#   make clean    # smaže všechny binární soubory
# zmena kompileru (výchozí je cc):
#   make CC=gcc

CFLAGS=-std=c11 -pedantic -Wall -Wextra -O2
OUT=main

all: primes-m primes-i-m

debug:
	make all "CFLAGS=$(CFLAGS) -g -fsanitize=address"

run: all
	ulimit -s 30000 ; ./primes ; ./primes-i

primes: primes-m
primes-i: primes-i-m

primes-m:
	make primes-g OUT=primes

primes-i-m:
	make primes-g OUT=primes-i "CFLAGS=$(CFLAGS) -DUSE_INLINE"

primes-g: primes.o error.o
	$(CC) $(CFLAGS) -lm -o $(OUT) primes.o error.o

primes.c: bitset.h
bitset.h: error.c
error.c: error.h

primes.o: primes.c
	$(CC) $(CFLAGS) -c -o primes.o primes.c

error.o: error.c
	$(CC) $(CFLAGS) -c -o error.o error.c

clean:
	rm primes.o error.o primes primes-i