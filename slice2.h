#pragma once
#include <ctype.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

typedef struct {
    char const * start;
    size_t len;
} Slice;

Slice* slice_construct(char const *const s, size_t const l);
Slice slice_construct2(char const *const start, char const *const end);
int slice_eq(const Slice* sl, char const *p);
int slice_eq2(const Slice* s1, const Slice* s2);
int isIdentifier(const Slice* s1);
void print(const Slice* s1);
size_t sliceHash(const Slice* key);