#pragma once

#include <stdbool.h>

#include "slice2.h"

#define MAX_SYMBOLS 17053

typedef struct statement statement;

typedef struct symbol_table
{
    uint64_t size;
    uint64_t cur;

    struct bin // bins in the hashmap
    {
        Slice *key;
        uint64_t value;
    } *bins;
} hash_map;

typedef struct function_table
{
    uint64_t size;
    uint64_t cur;

    struct declare* bins;
} hash_map_func;

typedef struct {
    struct map* var_map;
} emitter_t;

typedef struct
{
    char const *const program;
    char const *current;
    bool *visited;
    bool *visited_func;
    hash_map* symbol_table;
    hash_map_func* function_table;
    statement** ast;
    uint32_t size_ast;
    uint32_t cur_ast;
} Interpreter;

bool consume(Interpreter *in, const char *str);