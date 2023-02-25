# Makefile
# Řešení IJC-DU1, příklad a) a b), 25.2. 2023
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


# targets run by the user

all: general primes-m primes-i-m steg-decode

debug: general
	make $(TARGET) "CFLAGS=$(CFLAGS) -O0 -g -fsanitize=address" DIR=obj/debug/

run: general primes-m primes-i-m
	ulimit -s 30000 ; ./primes ; ./primes-i

primes: general primes-m
primes-i: general primes-i-m
steg-decode: general steg-decode-m

debug-run-all: general
	make primes "CFLAGS=$(CFLAGS) -O0 -g -fsanitize=address" DIR=obj/debug/
	make primes-i "CFLAGS=$(CFLAGS) -O0 -g -fsanitize=address" DIR=obj/debug/
	make steg-decode "CFLAGS=$(CFLAGS) -O0 -g -fsanitize=address" DIR=obj/debug/
	ulimit -s 30000 ; ./primes ; ./primes-i
	./steg-decode du1-obrazek.ppm

clean:
	rm obj/release/*.o obj/debug/*.o primes primes-i steg-decode || true

# target for creating directories for object files
general:
	mkdir -p obj/debug
	mkdir -p obj/release


# targets for different binaries

primes-m:
	make primes-g OUT=primes

primes-i-m:
	make primes-g OUT=primes-i "CFLAGS=$(CFLAGS) -DUSE_INLINE"

steg-decode-m: $(DIR)primes-nm.o $(DIR)steg-decode.o $(DIR)error.o $(DIR)ppm.o
	$(CC) $(CFLAGS) -lm -o steg-decode $^

# general target to compile primes
primes-g: $(DIR)primes.o $(DIR)error.o
	$(CC) $(CFLAGS) -lm -o $(OUT) $^


# targets for object files

$(DIR)primes.o: primes.c
	$(CC) $(CFLAGS) -c -o $@ primes.c

$(DIR)primes-nm.o: primes.c
	$(CC) $(CFLAGS) -DPRIMES_NO_MAIN -c -o $@ primes.c

$(DIR)error.o: error.c
	$(CC) $(CFLAGS) -c -o $@ error.c

$(DIR)steg-decode.o: steg-decode.c
	$(CC) $(CFLAGS) -c -o $@ steg-decode.c

$(DIR)ppm.o: ppm.c
	$(CC) $(CFLAGS) -c -o $@ ppm.c

# file dependencies
primes.c: bitset.h
bitset.h: error.c
error.c: error.h
steg-decode.c: primes.c bitset.h error.c ppm.c
ppm.c: ppm.h error.c

# disable implicit rules
.SUFFIXES:
