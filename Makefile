# Makefile
# Řešení IJC-DU1, příklad a) a b), 16.3. 2023
# Autor: Jakub Antonín Štigler, FIT
# makefile je udělaná pro linux
# možnosti překladu:
#   make                     # stejné jako 'make all'
#   make all                 # zkompiluje vše
#   make debug               # stejné jako 'make debug TARGET=all'
#   make debug TARGET=<rule> # stejné jako 'make <rule>' ale pro debugování
#   make run                 # zkompiluje úlohu A a spustí se
#                            # zvětšeným zásobníkem
#   make primes              # zkompiluje úlohu A s makry
#   make primes-i            # zkompiluje úlohu A s inline funkcemi
#   make steg-decode         # zkompiluje úlohu B
#   make clean               # smaže všechny binární soubory
#   make debug-run-all       # zkompiluje vše jako debug a vše spustí
# zmena kompileru (výchozí je cc):
#   make CC=gcc

# variables that can be changed by the user
CFLAGS=-std=c11 -pedantic -Wall -Wextra -O2
# CC=clang
TARGET=all

# variables modified internaly
OUT=main
DIR=obj/release/

.DEFAULT_GOAL=all

error.o: error.c error.h
eratosthenes.c: eratosthenes.h bitset.h error.h
ppm.o: ppm.c ppm.h error.h
primes.o: eratosthenes.h bitset.h error.h
primes-i.o: eratosthenes.h bitset.h error.h
steg-decode.o: steg-decode.c ppm.h error.h bitset.h eratosthenes.h

# targets run by the user

all:  primes-m primes-i-m steg-decode

debug:
	make $(TARGET) "CFLAGS=$(CFLAGS) -O0 -g -fsanitize=address" DIR=obj/debug/

run:  primes-m primes-i-m
	ulimit -s 30000 ; ./primes ; ./primes-i

primes:  primes-m
primes-i:  primes-i-m
steg-decode:  steg-decode-m

debug-run-all:
	make primes "CFLAGS=$(CFLAGS) -O0 -g -fsanitize=address" DIR=obj/debug/
	make primes-i "CFLAGS=$(CFLAGS) -O0 -g -fsanitize=address" DIR=obj/debug/
	make steg-decode "CFLAGS=$(CFLAGS) -O0 -g -fsanitize=address" DIR=obj/debug/
	ulimit -s 30000 ; ./primes ; ./primes-i
	./steg-decode du1-obrazek.ppm

clean:
	rm *.o || true
	rm primes primes-i steg-decode || true


# targets for different binaries

primes-m: primes.o error.o eratosthenes.o
	$(CC) $(CFLAGS) -o primes $^ -lm

primes-i-m: primes-i.o error.o eratosthenes.o
	$(CC) $(CFLAGS) -o primes-i $^ -lm

steg-decode-m: eratosthenes.o steg-decode.o error.o ppm.o
	$(CC) $(CFLAGS) -o steg-decode $^ -lm

# targets for object files

primes-i.o: primes.c
	$(CC) $(CFLAGS) -c -DUSE_INLINE -o $@ primes.c

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

# disable implicit rules
.SUFFIXES:
