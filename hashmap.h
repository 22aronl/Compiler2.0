#pragma once

#include <stdint.h>

#include "slice2.h"
#include "main.h"


#define MAX_SYMBOLS_FOR_FUNC 577

struct map {
    hash_map* map;
    bool* visited;
    uint32_t size;
    bool main;
};


uint16_t get_map_offset(struct map*, Slice* var);
void add_map_offset(struct map*, Slice*, uint32_t);

void addSymbol(Interpreter*, struct map*, Slice*, uint64_t, bool force_local);
uint64_t getSymbol(Interpreter*, struct map*, Slice *);
void add_function(Interpreter* in, struct declare* function);
struct declare* get_function(Interpreter* in, Slice* name);