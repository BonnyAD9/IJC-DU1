# makefile for debugging

main:
	clang -g -o main.out -std=c11 -O3 -DUSE_INLINE main.c bitset.c ../B/error.c