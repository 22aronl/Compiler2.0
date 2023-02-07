#include <ctype.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include "slice2.h"

Slice slice_construct(char const *const s, size_t const l) {
    Slice sl = {s, l};
    return sl;
}

Slice slice_construct2(char const *const start, char const *const end) {
    Slice sl = {start, (size_t)(end - start)};
    return sl;
}

int slice_eq(const Slice* sl, char const *p) {
    for(size_t i = 0; i < sl->len; i++) {
        if(p[i] != sl->start[i])
            return 0;
    }
    return p[sl->len] == 0;
}

int slice_eq2(const Slice* s1, const Slice* s2) {
    if(s1->len != s2->len)
        return 0;
    for(size_t i = 0; i < s1->len; i++) {
        if(s1->start[i] != s2->start[i])
            return 0;
    }
    return 1;
}

int isIdentifier(const Slice* s1) {
    if(s1->len == 0)
        return 0;
    if(!isalpha(s1->start[0]))
        return 0;
    for(size_t i = 1; i < s1->len; i++)
        if(!isalnum(s1->start[i]))
            return 0;
    return 1;
}

void print(const Slice* s1) {
    for (size_t i = 0; i < s1->len; i++) {
      printf("%c", s1->start[i]);
    }
}

size_t sliceHash(const Slice* key) {
    size_t out = 5381;
    for (size_t i = 0; i < key->len; i++) {
      char const c = key->start[i];
      out = out * 33 + c;
    }
    return out;
}